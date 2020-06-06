/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-05     LD       the first version
 */
#include "RS422.h"

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

static void Key_Init()
{
    rt_pin_mode(KEY_PIN_1, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_PIN_2, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_PIN_3, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY_PIN_4, PIN_MODE_INPUT_PULLUP);

}

static int Read_key()
{
    int Key_num=0;
    if (rt_pin_read(KEY_PIN_1) == PIN_LOW)
        Key_num |= 1 << 0;
    if (rt_pin_read(KEY_PIN_2) == PIN_LOW)
        Key_num |= 1 << 1;
    if (rt_pin_read(KEY_PIN_3) == PIN_LOW)
        Key_num |= 1 << 2;
    if (rt_pin_read(KEY_PIN_4) == PIN_LOW)
        Key_num |= 1 << 3;
    return Key_num;
}

static void mb_slave_poll(void *parameter)
{
    rt_pin_mode(RS422_RE_PIN, PIN_MODE_OUTPUT);
//    rt_pin_mode(RS422_DE_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(RS422_RE_PIN, PIN_LOW);
//    rt_pin_write(RS422_DE_PIN, PIN_LOW);

    Key_Init();

    int Addr_m=Read_key();
    eMBInit(MB_RTU, Addr_m, PORT_NUM, PORT_BAUDRATE, PORT_PARITY);
    eMBEnable();

    while (1)
    {
        rt_kprintf("按键的值为：%d！\n",Read_key());
        eMBPoll();
        rt_thread_mdelay(MB_POLL_CYCLE_MS);
    }
}

static int mb_slave_usart3()
{
    static rt_uint8_t is_init = 0;
    rt_thread_t tid1 = RT_NULL;

    if (is_init > 0)
    {
        rt_kprintf("modbus is error\n");
        return -RT_ERROR;
    }
    tid1 = rt_thread_create(MB_POLL_UART_NAME, mb_slave_poll, RT_NULL, MB_POLL_STACK_SIZE, MB_POLL_THREAD_PRIORITY, MB_POLL_THREAD_TICK);


    if (tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else
    {
        goto __exit;
    }

    is_init = 1;
    return RT_EOK;

__exit:
    if (tid1)
        rt_thread_delete(tid1);

    return -RT_ERROR;
}

/* 加入到 初始化 列表中 */
INIT_APP_EXPORT(mb_slave_usart3);
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(mb_slave_usart3, uart3_RS422 device Init function);
