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

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial_RS232;

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static char Serial_232_Get_char()
{
    char ch;
    /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
    while (rt_device_read(serial_RS232, -1, &ch, 1) != 1)
    {
        /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    }
    return ch;
}
static void serial_thread_entry(void *parameter)
{
   char ch;

    while (1)
    {
        ch=Serial_232_Get_char();
        /* 读取到的数据通过串口错位输出 （需要修改的地方）*/
//        ch = ch + 1;
        rt_device_write(serial_RS232, 0, &ch, 1);
    }
}

static int uart2_init()
{
    rt_err_t ret = RT_EOK;
//    char str[] = "hello RT-Thread!\r\n";

    /* 查找系统中的串口设备 */
    serial_RS232 = rt_device_find(USE_UART_NAME);
    if (!serial_RS232)
    {
        rt_kprintf("find %s failed!\n", USE_UART_NAME);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial_RS232, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial_RS232, uart_input);
    /* 发送字符串 */
//    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* 创建 serial 动态线程 */
    rt_thread_t thread = rt_thread_create("USART2_DSP", serial_thread_entry, RT_NULL,
                                            USART2_stack_size,
                                            USART2_priority,
                                            USART2_tick);
    /* 创建成功则启动线程 */
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

/* 加入到 初始化 列表中 */
INIT_APP_EXPORT(uart2_init);
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(uart2_init, uart2_RS232 device Init function);
