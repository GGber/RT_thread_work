/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-09     RT-Thread    first version
 */

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "port.h"
#include "user_mb_app.h"

/*
 * modbus主从之间数据拷贝的线程
 *
 * */
extern USHORT usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];    //主机寄存器
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];                               //从机寄存器

/*
 * 引脚模式初始化
 * */
static int pin_init(void)
{
    rt_pin_mode(DIN1, PIN_MODE_INPUT);
    rt_pin_mode(DIN2, PIN_MODE_INPUT);
    rt_pin_mode(DIN3, PIN_MODE_INPUT);
    rt_pin_mode(DIN4, PIN_MODE_INPUT);
    rt_pin_mode(DIN5, PIN_MODE_INPUT);
    rt_pin_mode(DIN6, PIN_MODE_INPUT);
    rt_pin_mode(DIN7, PIN_MODE_INPUT);
    rt_pin_mode(DIN8, PIN_MODE_INPUT);

    rt_pin_mode(ADDR1, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADDR2, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADDR3, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADDR4, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADDR5, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADDR6, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADDR7, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADDR8, PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(FLOW1, PIN_MODE_INPUT);
    rt_pin_mode(FLOW2, PIN_MODE_INPUT);
    rt_pin_mode(FLOW3, PIN_MODE_INPUT);
    rt_pin_mode(FLOW4, PIN_MODE_INPUT);
    rt_pin_mode(FLOW5, PIN_MODE_INPUT);
    rt_pin_mode(FLOW6, PIN_MODE_INPUT);
    rt_pin_mode(FLOW7, PIN_MODE_INPUT);
    rt_pin_mode(FLOW8, PIN_MODE_INPUT);

    rt_pin_write(MOS1, PIN_LOW);
    rt_pin_write(MOS2, PIN_LOW);
    rt_pin_write(MOS3, PIN_LOW);
    rt_pin_write(MOS4, PIN_LOW);
    rt_pin_write(MOS5, PIN_LOW);
    rt_pin_write(MOS6, PIN_LOW);
    rt_pin_write(MOS7, PIN_LOW);
    rt_pin_write(MOS8, PIN_LOW);

    rt_pin_mode(MOS1, PIN_MODE_OUTPUT);
    rt_pin_mode(MOS2, PIN_MODE_OUTPUT);
    rt_pin_mode(MOS3, PIN_MODE_OUTPUT);
    rt_pin_mode(MOS4, PIN_MODE_OUTPUT);
    rt_pin_mode(MOS5, PIN_MODE_OUTPUT);
    rt_pin_mode(MOS6, PIN_MODE_OUTPUT);
    rt_pin_mode(MOS7, PIN_MODE_OUTPUT);
    rt_pin_mode(MOS8, PIN_MODE_OUTPUT);
    return 0;
}
INIT_DEVICE_EXPORT(pin_init);

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(DIN8, PIN_MODE_OUTPUT);     //把DIN8设置成输出，作为输出指示灯玩



    LOG_D("main running...");
    while (1)
    {
        count++;
        /* set LED0 pin level to high or low */
        rt_pin_write(DIN8, count % 2);
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}

