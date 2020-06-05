#include <rtthread.h>
#include "board.h"
#include "mb.h"
#include "mb_m.h"
#include "user_mb_app.h"

#include <board.h>
#include <rtdevice.h>

#define DBG_TAG "master"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define MB_POLL_THREAD_PRIORITY  10
#define MB_SEND_THREAD_PRIORITY  RT_THREAD_PRIORITY_MAX - 2

#define MB_POLL_CYCLE_MS   100

extern USHORT usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

#define MASTER_RUN_TIMEOUT  RT_WAITING_FOREVER

#define ADDR_SET_CHANNEL    9       //设置采集通道的地址
#define ADDR_GET_AVERAGE    10      //获取平均值的地址
#define ADDR_RESET          11      //复位地址
#define CMD_RESET           0x5555  //复位指令
static void send_thread_entry(void *parameter)
{
    eMBMasterReqErrCode error_code = MB_MRE_NO_ERR;

    uint16_t slave_need_reset_number = 0;   //需要复位采集点的编号
    uint16_t slave_new_get_ch = 0;          //重设的采集通道
    uint16_t slave_need_update_one = 0;     //需要重新采集的设备编号
    uint16_t slave_need_update_all = 0;     //所有设备都需要重新采集的标志
    uint32_t master_thread_cycle = 0;
    LOG_I("send_thread_entry begin.");
    while (1)
    {
        /* Test Modbus Master */

        if (rt_pin_read(ADDR8) == PIN_HIGH) //无线模式直接退出
        {
            rt_thread_mdelay(100);
            continue;
        }
        LOG_I("Modbus  begin\n");

        /*查询传感器的值*/
        for (unsigned char slave_addr = 1; slave_addr <= 64; slave_addr++)
        {
            if (usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] >= 0xF005)  //如果重试超过5次，就不管了
            {
                continue;
            }
            error_code = eMBMasterReqReadHoldingRegister(slave_addr, ADDR_GET_AVERAGE, 1, MASTER_RUN_TIMEOUT); //读传感器的值
            if (error_code != MB_MRE_NO_ERR)    //如果读取错误
            {
                LOG_W("get[%d] err:%d", slave_addr, error_code);
                rt_enter_critical();
                if ((usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] & 0xF000) != 0xF000)                  //保证有重试标志
                    usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] = 0xF000;
                usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE]++;                                           //加重试次数
                rt_exit_critical();
            }
            else
            {
                LOG_I("get[%d]:%d", slave_addr, usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE]);
            }
            rt_thread_mdelay(20);
        }

        rt_enter_critical();
        /*read*/
        slave_need_update_all = usSRegHoldBuf[92];
        usSRegHoldBuf[92] = 0;
        slave_need_update_one = usSRegHoldBuf[93];
        usSRegHoldBuf[93] = 0;
        slave_new_get_ch = usSRegHoldBuf[94];
        usSRegHoldBuf[94] = 0;
        slave_need_reset_number = usSRegHoldBuf[98];
        usSRegHoldBuf[98] = 0;
        /*write*/
        usSRegHoldBuf[97] = master_thread_cycle;
        /*本地数据转储*/
        for (int i = 1; i <= 64; i++)
        {
            usSRegHoldBuf[i] = usMRegHoldBuf[i - 1][ADDR_GET_AVERAGE];    //usMRegHoldBuf[0]存储地址为1的数据
        }
        rt_exit_critical();

        rt_enter_critical();
        /*重新检查所有点的在线状态*/
        if (slave_need_update_all == 0x5555)
        {
            for (int slave_addr = 1; slave_addr <= 64; slave_addr++)
            {
                if ((usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] & 0xF000) == 0xF000)
                    usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] = 0;
            }
        }
        /*重新检查单板状态*/
        if (slave_need_update_one != 0)
        {
            if (slave_need_update_one < MB_MASTER_TOTAL_SLAVE_NUM)
                usMRegHoldBuf[slave_need_update_one - 1][ADDR_GET_AVERAGE] = 0;
        }
        rt_exit_critical();
        /*设置采集通道*/
        if (slave_new_get_ch != 0)
        {
            for (int slave_addr = 1; slave_addr <= 64; slave_addr++)
            {
                if (usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] >= 0xF005)  //如果重试超过5次，就不管了
                    continue;
                error_code = eMBMasterReqWriteHoldingRegister(slave_addr, ADDR_SET_CHANNEL, slave_new_get_ch,
                MASTER_RUN_TIMEOUT); //设置采集通道

                if (error_code != MB_MRE_NO_ERR)    //如果读取错误
                {
                    LOG_W("set_channel[%d] err:%d", error_code);
                    rt_enter_critical();
                    if ((usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] & 0xF000) != 0xF000) //保证有重试标志
                        usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] |= 0xF000;
                    usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE]++;                                //加重试次数
                    rt_exit_critical();
                }
                else
                {
                    LOG_I("set_channel[%d]:%d", slave_addr, slave_new_get_ch);
                }
                rt_thread_mdelay(20);
            }
        }
        /*复位*/
        if (slave_need_reset_number != 0)
        {
            int slave_addr = slave_need_reset_number;
            for (int i = 0; i < 1; i++) //循环一次保证内层代码的通用性
            {
                if (slave_need_reset_number > MB_MASTER_TOTAL_SLAVE_NUM)
                    continue;
                if (usMRegHoldBuf[slave_need_reset_number - 1][ADDR_GET_AVERAGE] >= 0xF005)  //如果重试超过5次，就不管了
                    continue;
                error_code = eMBMasterReqWriteHoldingRegister(slave_addr, ADDR_RESET, CMD_RESET,
                MASTER_RUN_TIMEOUT); //复位命令

                if (error_code != MB_MRE_NO_ERR)    //如果读取错误
                {
                    LOG_W("reset[%d] err:%d", error_code);
                    rt_enter_critical();
                    if ((usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] & 0xF000) != 0xF000) //保证有重试标志
                        usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE] |= 0xF000;
                    usMRegHoldBuf[slave_addr - 1][ADDR_GET_AVERAGE]++;                                //加重试次数
                    rt_exit_critical();
                }
                else
                {
                    LOG_D("reset[%d]:%d", slave_addr, slave_new_get_ch);
                }
            }
            rt_thread_mdelay(10);
        }

        LOG_I("run times:%d", ++master_thread_cycle);
        rt_thread_mdelay(10);   //保证当所有从机都不在线时，线程不会while(1)跑死
    }
}

static void mb_master_poll(void *parameter)
{
    eMBMasterInit(MB_RTU, MODBUS_MASTER_PORT_NUM, MODBUS_MASTER_PORT_BAUDRATE, MODBUS_MASTER_PORT_PARITY);
    eMBMasterEnable();

    while (1)
    {
        eMBMasterPoll();
        rt_thread_mdelay(MB_POLL_CYCLE_MS);
    }
}

int mb_master_samlpe(void)
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

    tid2 = rt_thread_create("md_m_send", send_thread_entry, RT_NULL, 1024, MB_SEND_THREAD_PRIORITY, 10);
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
