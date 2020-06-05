/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-13     shine       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>

#define DBG_TAG "Zigbee"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define SAMPLE_UART_NAME                "uart2"
#define CMD_START                       0xAA
#define CMD_END                         0xC9

#define ONE_DATA_MAXLEN                 5

/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

#include "user_mb_app.h"
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];
/* �������ݻص����� */
static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    if (size > 0)
    {
        rt_sem_release(&rx_sem);
    }
    return RT_EOK;
}

static char uart_sample_get_char(void)
{
    char ch;

    while (rt_device_read(serial, 0, &ch, 1) == 0)
    {
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    }

    return ch;
}

/* ���ݽ����߳� */
static void data_parsing(void *parameter)
{
    char ch;
    char data[ONE_DATA_MAXLEN];
    static char recv_len = 0;

    while (1)
    {

        if (rt_pin_read(ADDR8) == PIN_LOW) //����ģʽֱ���˳�
        {
            rt_thread_mdelay(100);
            continue;
        }

        LOG_I("Zigbee  begin\n");

        ch = uart_sample_get_char();
//        LOG_I("Zigbee get CH:%x\n",ch);
        //rt_device_write(serial, 0, &ch, 1);

        if (recv_len == 0 && ch == CMD_START)
        {
            data[recv_len++] = ch;
            continue;
        }
        else if (recv_len < ONE_DATA_MAXLEN - 1)
        {
            data[recv_len++] = ch;
        }
        else if (recv_len == ONE_DATA_MAXLEN - 1 && ch == CMD_END)
        {
            data[recv_len++] = ch;
            /*parse data begin...*/
            uint8_t module_addr = data[1] & 0x7F;
            uint16_t module_value = data[2] * 256 + data[3];
            usSRegHoldBuf[module_addr] = module_value;
            /*parse data end...*/
            recv_len = 0;
            LOG_I("ADDR:%x\n",module_addr);
            LOG_I("value:%x\n",module_value);
//            LOG_I("Zigbee Get char %x\n",data);
        }
        rt_thread_mdelay(10);
    }
}

static int uart_data_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread!\r\n";

    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* ����ϵͳ�еĴ����豸 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    rt_kprintf("find %s SUCCESS!\n", uart_name);

    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "zigbee_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial, uart_rx_ind);
//    /* �����ַ��� */
//    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("zigbee",data_parsing, RT_NULL, 1024, 9, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

/* ������ msh �����б��� */
MSH_CMD_EXPORT(uart_data_sample, uart device sample);
INIT_APP_EXPORT(uart_data_sample);
