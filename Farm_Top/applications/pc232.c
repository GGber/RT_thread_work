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
#include <string.h>
#include "user_mb_app.h"

#define DBG_TAG "rs232"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define SAMPLE_UART_NAME                "uart4"
#define DATA_CMD_START                  0xAA
#define DATA_CMD_READ                   0xBB
#define DATA_CMD_WRITE                  0xCC

#define DATA_BUFFERSZ                   1002    //���ݰ�����

rt_uint16_t local_data_read[1005];   //��������
rt_uint16_t local_data_write[1005];  //��������

/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static rt_device_t serial;
extern USHORT usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
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
static void data_parsing(void)
{
    char ch;
    int recv_len = 0;
    char recv_cmd[DATA_BUFFERSZ + 5];   //���ջ�����
    char send_buf[DATA_BUFFERSZ + 5];   //��������buffer

    while (1)
    {
        ch = uart_sample_get_char();
//        rt_device_write(serial, 0, &ch, 1);

        if (recv_len == 0 && ch == DATA_CMD_START)
        {
            recv_cmd[recv_len++] = ch;
        }
        else if (recv_len > 0 && recv_len < DATA_BUFFERSZ - 1)
        {
            recv_cmd[recv_len++] = ch;
        }
        else if (recv_len == DATA_BUFFERSZ - 1 && ch == DATA_CMD_READ)
        {
            recv_cmd[recv_len++] = ch;
            //LOG_HEX("read", 16, recv_cmd, recv_len);
            /*parsing begin*/
            LOG_D("process read cmd...");
            memset(send_buf, 0, sizeof(send_buf));
            send_buf[0] = DATA_CMD_START;
            send_buf[DATA_BUFFERSZ - 1] = DATA_CMD_READ;
            memcpy(send_buf + 1, local_data_read, DATA_BUFFERSZ - 2);
            rt_device_write(serial, 0, send_buf, DATA_BUFFERSZ);
            //LOG_HEX("read", 32, local_data_read, 1002);
            /*parsing end*/
            recv_len = 0;
        }
        else if (recv_len == DATA_BUFFERSZ - 1 && ch == DATA_CMD_WRITE)
        {
            recv_cmd[recv_len++] = ch;
            /*parsing begin*/
            LOG_D("process write cmd...");

            rt_enter_critical();
            memcpy(local_data_write, recv_cmd + 1, DATA_BUFFERSZ);  //copy out
            rt_exit_critical();

            memset(send_buf, 0, sizeof(send_buf));
            send_buf[0] = DATA_CMD_START;
            send_buf[DATA_BUFFERSZ - 1] = DATA_CMD_WRITE;
            memcpy(send_buf + 1, local_data_write, DATA_BUFFERSZ - 2);
            rt_device_write(serial, 0, send_buf, DATA_BUFFERSZ);
            //LOG_HEX("write", 32, local_data_write, 1002);
            for (int i = 0; i <= 100; i++)
            {
                rt_kprintf("[%d]=>%d\t", i, local_data_write[i]);
                if (i % 10 == 0)
                    rt_kprintf("\n");
            }
            /*parsing end*/
            recv_len = 0;
        }
        else
        {
            recv_len = 0;
        }
//        rt_thread_mdelay(1000);
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

    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial, uart_rx_ind);
    /* �����ַ��� */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial", (void (*)(void *parameter)) data_parsing, RT_NULL, 4096, 25, 10);
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
