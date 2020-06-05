/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-08     Shine       the first version
 */

#include <rtthread.h>

#define DBG_TAG "master"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "mb.h"
#include "mb_m.h"
#include "user_mb_app.h"

#define PORT_NUM        5
#define PORT_BAUDRATE   9600
#define PORT_PARITY     MB_PAR_NONE

#define MB_POLL_THREAD_PRIORITY  10
#define MB_SEND_THREAD_PRIORITY  RT_THREAD_PRIORITY_MAX - 1

#define MB_SEND_REG_START  2
#define MB_SEND_REG_NUM    2

#define MB_POLL_CYCLE_MS   10

extern USHORT usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
extern rt_uint16_t local_data_read[1005];
extern rt_uint16_t local_data_write[1005];

static uint16_t slave_online[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };     //��¼�ӻ�����״̬

#define ADDR_MOUDLE             1   //ģ���ַ
#define LEN_MOUDLE              64  //ģ�鳤��

#define ADDR_SET_MODE           80  //���ù���ģʽ
#define ADDR_SET_THRESHOLD      81  //������ֵ
#define LEN_SET_THRESHOLD       8   //������ֵ����
#define ADDR_SET_OUT            89  //�������ֵ
#define ADDR_SET_UPDATE_ALL     92  //���ø���ȫ��ģ��
#define ADDR_SET_UPDATE_ONE     93  //���ø��µ���ģ��
#define ADDR_SET_GET_CH         94  //���òɼ�ͨ��
#define ADDR_SET_RESET_MODULE   98  //���ø�λ����ģ��

#define CMD_RESET               0x5555
#define CMD_UPDATE              0x5555
/*
 * ��ͨѶ����ʱ�����ô˺��������������¼
 * */
static void mb_error_append(uint8_t farm_addr)
{
    if ((slave_online[farm_addr - 1] & 0xF000) != 0xF000)                      //��֤�����Ա�־
        slave_online[farm_addr - 1] = 0xF000;
    slave_online[farm_addr - 1]++;                                              //�����Դ���
}

static void mb_error_clear(uint8_t farm_addr)
{
    slave_online[farm_addr - 1] = 0;
}

static void send_thread_entry(void *parameter)
{
    rt_thread_mdelay(1000);
    eMBMasterReqErrCode error_code = MB_MRE_NO_ERR;

    uint16_t slave_need_reset_number[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };        //��Ҫ��λ�ɼ���ı��
    uint16_t slave_new_get_ch[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };               //����Ĳɼ�ͨ��
    uint16_t slave_need_update_one[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };          //��Ҫ���²ɼ����豸���
    uint16_t slave_need_update_all[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };          //�����豸����Ҫ���²ɼ��ı�־

    uint16_t slave_mode[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };                     //��ذ�ģʽ
    uint16_t slave_threshold[MB_MASTER_TOTAL_SLAVE_NUM][8] = { 0 };             //��ذ���ֵ
    uint16_t slave_need_update_threshold[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };    //��հ���Ҫ���±�־
    uint16_t slave_out_value[MB_MASTER_TOTAL_SLAVE_NUM] = { 0 };                //��ذ����ֵ

    uint32_t master_thread_cycle = 0;
    LOG_I("send_thread_entry begin.");

    while (1)
    {
        /* Test Modbus Master */
        /*������*/
        for (uint8_t farm_addr = 1; farm_addr <= 5; farm_addr++)
        {
            if (slave_online[farm_addr - 1] >= 0xF005)           //������Գ���5�Σ��Ͳ�����
                continue;
            error_code = eMBMasterReqReadHoldingRegister(farm_addr, 0, 100, RT_WAITING_FOREVER);  //��100���Ĵ���
            if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
            {
                LOG_W("get[%d] err:%d", farm_addr, error_code);
                mb_error_append(farm_addr);
            }
            else
            {
                //LOG_I("get[%d]:%d", slave_addr, usMRegHoldBuf[slave_addr - 1][ADDR_ONLINE]);
                LOG_I("get[%d] data ok.", farm_addr);
                mb_error_clear(farm_addr);
            }
            rt_thread_mdelay(10);
        }

        /*��������ת��read*/
        rt_enter_critical();
        for (uint8_t farm_addr = 1; farm_addr <= 5; farm_addr++)
        {
            for (int module_addr = 1; module_addr <= 100; module_addr++)
            {
                local_data_read[100 * (farm_addr - 1) + module_addr] = usMRegHoldBuf[farm_addr - 1][module_addr]; //usMRegHoldBuf[0]�洢��ַΪ1������
            }
            //LOG_D("local_data_read[1][%d]=>0x%x %d", farm_addr, local_data_read[farm_addr], local_data_read[farm_addr]);
        }
        rt_exit_critical();

        /*��������ת��write*/
        rt_enter_critical();
        for (uint8_t farm_addr = 1; farm_addr <= 5; farm_addr++)
        {
            slave_mode[farm_addr - 1] = local_data_write[100 * (farm_addr - 1) + ADDR_SET_MODE];
            local_data_write[100 * (farm_addr - 1) + ADDR_SET_MODE] = 0;
            slave_need_update_threshold[farm_addr - 1] = 0;
            for (int threshold = 1; threshold <= LEN_SET_THRESHOLD; threshold++)
            {
                slave_threshold[farm_addr - 1][threshold - 1] = local_data_write[100 * (farm_addr - 1) + (threshold - 1)
                        + ADDR_SET_MODE];
                local_data_write[100 * (farm_addr - 1) + (threshold - 1) + ADDR_SET_MODE] = 0;
                slave_need_update_threshold[farm_addr - 1] |= slave_threshold[farm_addr - 1][threshold - 1];
                //rt_kprintf("thres[%d]=>%d ", threshold, slave_threshold[farm_addr - 1][threshold-1]);
            }
            //rt_kprintf("need[1]=>%d\n", slave_need_update_threshold[0]);

            slave_out_value[farm_addr - 1] = local_data_write[100 * (farm_addr - 1) + ADDR_SET_OUT];
            local_data_write[100 * (farm_addr - 1) + ADDR_SET_OUT] = 0;

            slave_need_update_all[farm_addr - 1] = local_data_write[100 * (farm_addr - 1) + ADDR_SET_UPDATE_ALL];
            local_data_write[100 * (farm_addr - 1) + ADDR_SET_UPDATE_ALL] = 0;

            slave_need_update_one[farm_addr - 1] = local_data_write[100 * (farm_addr - 1) + ADDR_SET_UPDATE_ONE];
            local_data_write[100 * (farm_addr - 1) + ADDR_SET_UPDATE_ONE] = 0;

            slave_new_get_ch[farm_addr - 1] = local_data_write[100 * (farm_addr - 1) + ADDR_SET_GET_CH];
            local_data_write[100 * (farm_addr - 1) + ADDR_SET_GET_CH] = 0;

            slave_need_reset_number[farm_addr - 1] = local_data_write[100 * (farm_addr - 1) + ADDR_SET_RESET_MODULE];
            local_data_write[100 * (farm_addr - 1) + ADDR_SET_RESET_MODULE] = 0;

        }
        rt_exit_critical();

        /*д����*/
        for (uint8_t farm_addr = 1; farm_addr <= 5; farm_addr++)
        {
            if (slave_online[farm_addr - 1] >= 0xF005)      //������Գ���5�Σ��Ͳ�����
                continue;

            /*���¼�����е������״̬*/
            if (slave_need_update_all[farm_addr - 1] == CMD_RESET)
            {
                error_code = eMBMasterReqWriteHoldingRegister(farm_addr, ADDR_SET_UPDATE_ALL,
                CMD_UPDATE,
                RT_WAITING_FOREVER);
                if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
                {
                    LOG_W("update_farm[%d] err:%d", farm_addr, error_code);
                    mb_error_append(farm_addr);
                    continue;
                }
                else
                {
                    LOG_I("updata farm[%d] success", farm_addr);
                    mb_error_clear(farm_addr);
                }
                rt_thread_mdelay(10);
            }
            /*���¼�鵥��״̬*/
            if (slave_need_update_one[farm_addr - 1] != 0)
            {
                error_code = eMBMasterReqWriteHoldingRegister(farm_addr, ADDR_SET_UPDATE_ONE,
                        slave_need_update_one[farm_addr - 1],
                        RT_WAITING_FOREVER);
                if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
                {
                    LOG_W("update_farm[%d][%d] err:%d", farm_addr, slave_need_update_one[farm_addr - 1], error_code);
                    mb_error_append(farm_addr);
                    continue;
                }
                else
                {
                    LOG_I("updata farm[%d][%d] success", farm_addr, slave_need_reset_number[farm_addr - 1]);
                    mb_error_clear(farm_addr);
                }
                rt_thread_mdelay(10);
            }
            /*���òɼ�ͨ��*/
            if (slave_new_get_ch[farm_addr - 1] != 0)
            {
                uint16_t ch_buffer[65];
                for (int i = 0; i < 65; i++)
                    ch_buffer[i] = slave_new_get_ch[farm_addr - 1];

                error_code = eMBMasterReqWriteHoldingRegister(farm_addr, ADDR_SET_GET_CH,
                        slave_new_get_ch[farm_addr - 1],
                        RT_WAITING_FOREVER);
                if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
                {
                    LOG_W("set ch[%d] err:%d", farm_addr, error_code);
                    mb_error_append(farm_addr);
                    continue;
                }
                else
                {
                    LOG_I("set ch[%d] success", farm_addr);
                    mb_error_clear(farm_addr);
                }
                rt_thread_mdelay(10);
            }
            /*��λģ��*/
            if (slave_need_reset_number[farm_addr - 1] != 0)
            {
                error_code = eMBMasterReqWriteHoldingRegister(farm_addr, ADDR_SET_RESET_MODULE,
                        slave_need_reset_number[farm_addr - 1],
                        RT_WAITING_FOREVER);
                if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
                {
                    LOG_W("reset farm[%d][%d] err:%d", farm_addr, slave_need_reset_number[farm_addr - 1], error_code);
                    mb_error_append(farm_addr);
                    continue;
                }
                else
                {
                    LOG_I("reset farm[%d][%d] success", farm_addr, slave_need_reset_number[farm_addr - 1]);
                    mb_error_clear(farm_addr);
                }
                rt_thread_mdelay(10);
            }

            /*���ôӻ�ģʽ*/
            if (slave_mode[farm_addr - 1] != 0)
            {
                error_code = eMBMasterReqWriteHoldingRegister(farm_addr, ADDR_SET_MODE, slave_mode[farm_addr - 1],
                RT_WAITING_FOREVER);
                if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
                {
                    LOG_W("set farm mode[%d] err:%d", farm_addr, error_code);
                    mb_error_append(farm_addr);
                    continue;
                }
                else
                {
                    LOG_I("set farm mode[%d] success", farm_addr);
                    mb_error_clear(farm_addr);
                }
                rt_thread_mdelay(10);
            }

            /*������ֵ*/
            if (slave_need_update_threshold[farm_addr - 1] != 0)
            {
                for (int reg_threshold = 1; reg_threshold <= LEN_SET_THRESHOLD; reg_threshold++)
                {
                    LOG_D("[%d]%d=>%d", farm_addr, reg_threshold, slave_threshold[farm_addr - 1][reg_threshold - 1]);
                    error_code = eMBMasterReqWriteHoldingRegister(farm_addr, ADDR_SET_THRESHOLD + reg_threshold - 1,
                            slave_threshold[farm_addr - 1][reg_threshold - 1],
                            RT_WAITING_FOREVER);
                    if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
                    {
                        LOG_W("set farm threshold[%d][%d] err:%d", farm_addr, reg_threshold, error_code);
                        mb_error_append(farm_addr);
                        continue;
                    }
                    else
                    {
                        LOG_I("set farm mode[%d][%d] success", farm_addr, reg_threshold);
                        mb_error_clear(farm_addr);
                    }
                    rt_thread_mdelay(10);
                }
//                error_code = eMBMasterReqWriteMultipleHoldingRegister(farm_addr, ADDR_SET_THRESHOLD, LEN_SET_THRESHOLD,
//                        slave_threshold[farm_addr - 1],
//                        RT_WAITING_FOREVER);
//                if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
//                {
//                    LOG_W("set threshold[%d] err:%d", farm_addr, error_code);
//                    mb_error_append(farm_addr);
//                    continue;
//                }
//                else
//                {
//                    LOG_I("set threshold[%d] success", farm_addr);
//                    mb_error_clear(farm_addr);
//                }
//                rt_thread_mdelay(10);
            }

            /*�������ֵ*/
            if (slave_out_value[farm_addr - 1] != 0)
            {
                error_code = eMBMasterReqWriteHoldingRegister(farm_addr, ADDR_SET_OUT, slave_out_value[farm_addr - 1],
                RT_WAITING_FOREVER);
                if (error_code != MB_MRE_NO_ERR)    //�����ȡ����
                {
                    LOG_W("set out value[%d] err:%d", farm_addr, error_code);
                    mb_error_append(farm_addr);
                    continue;
                }
                else
                {
                    LOG_I("set out value[%d] success", farm_addr);
                    mb_error_clear(farm_addr);
                }
                rt_thread_mdelay(10);
            }

        }
        LOG_I("run times:%d", ++master_thread_cycle);
        rt_thread_mdelay(10);   //��֤�����дӻ���������ʱ���̲߳���while(1)����
    }
}
static void mb_master_poll(void *parameter)
{
    eMBMasterInit(MB_RTU, PORT_NUM, PORT_BAUDRATE, PORT_PARITY);
    eMBMasterEnable();

    while (1)
    {
        eMBMasterPoll();
        rt_thread_mdelay(MB_POLL_CYCLE_MS);
    }
}

static int mb_master_samlpe(int argc, char **argv)
{
    static rt_uint8_t is_init = 0;
    rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;

    if (is_init > 0)
    {
        rt_kprintf("sample is running\n");
        return -RT_ERROR;
    }
    tid1 = rt_thread_create("md_m_poll", mb_master_poll, RT_NULL, 512, MB_POLL_THREAD_PRIORITY, 10);
    if (tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else
    {
        goto __exit;
    }

    tid2 = rt_thread_create("md_m_send", send_thread_entry, RT_NULL, 2048, MB_SEND_THREAD_PRIORITY, 10);
    if (tid2 != RT_NULL)
    {
        rt_thread_startup(tid2);
    }
    else
    {
        goto __exit;
    }

    is_init = 1;
    return RT_EOK;

    __exit: if (tid1)
        rt_thread_delete(tid1);
    if (tid2)
        rt_thread_delete(tid2);

    return -RT_ERROR;
}

MSH_CMD_EXPORT(mb_master_samlpe, run a modbus master sample);
INIT_APP_EXPORT(mb_master_samlpe);
