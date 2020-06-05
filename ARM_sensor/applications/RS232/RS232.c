/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-01     86132       the first version
 */
#include "RS232.h"

/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static rt_device_t serial_RS232;

/* �������ݻص����� */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {

        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(serial_RS232, -1, &ch, 1) != 1)
        {
            /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* ��ȡ��������ͨ�����ڴ�λ��� ����Ҫ�޸ĵĵط���*/
//        ch = ch + 1;
//        rt_device_write(serial, 0, &ch, 1);
    }
}

static int uart2_init()
{
    rt_err_t ret = RT_EOK;
//    char str[] = "hello RT-Thread!\r\n";

    /* ����ϵͳ�еĴ����豸 */
    serial_RS232 = rt_device_find(USE_UART_NAME);
    if (!serial_RS232)
    {
        rt_kprintf("find %s failed!\n", USE_UART_NAME);
        return RT_ERROR;
    }

    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(serial_RS232, RT_DEVICE_FLAG_INT_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial_RS232, uart_input);
    /* �����ַ��� */
//    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* ���� serial ��̬�߳� */
    rt_thread_t thread = rt_thread_create("USART2_DSP", serial_thread_entry, RT_NULL,
                                            USART2_stack_size,
                                            USART2_priority,
                                            USART2_tick);
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

/* ���뵽 ��ʼ�� �б��� */
INIT_APP_EXPORT(uart2_init);
/* ������ msh �����б��� */
MSH_CMD_EXPORT(uart2_init, uart2_RS232 device Init function);