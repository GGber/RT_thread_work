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

#define LED_0 GET_PIN(C,13)

int main(void)
{
    int count = 1;
//    rt_pin_mode(LED_0, PIN_MODE_OUTPUT);

    while (1)
    {
//        LOG_I("Go into Main\n");
//        rt_pin_write(LED_0, 1);
//        rt_thread_mdelay(500);
//        rt_pin_write(LED_0, 0);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
