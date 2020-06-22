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
#include "beep.h"
#include "oled.h"
#include "ble.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* PLEASE DEFINE the LED0 pin for your board, such as: PA5 */
#define LED0_PIN    GET_PIN(A, 11)


int main(void)
{
    int count = 1;


    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    Beep_Init();                // 初始化蜂鸣器

    OLED_Init();                //OLED初始化

//    BLE_PIN_Init();

    while (1)
    {
        /* set LED0 pin level to high or low */
        rt_pin_write(LED0_PIN, count);
        count=!count;

        OLED_Show_str();

        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
