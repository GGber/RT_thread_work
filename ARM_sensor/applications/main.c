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
#include "mb.h"
#include "user_mb_app.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/**************LEDºê¶¨Òå*******************/
#define LED0_PIN    GET_PIN(B, 8)
#define LED1_PIN    GET_PIN(B, 9)

/********************Modbus¼Ä´æÆ÷***********************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];
extern USHORT usSRegInBuf[S_REG_INPUT_NREGS];

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
//        LOG_I("main is running\n");
        rt_pin_write(LED0_PIN|LED1_PIN, count);
        count=!count;
        rt_thread_mdelay(1000);

    }

    return RT_EOK;
}

MSH_CMD_EXPORT(main,LED test function);
