/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-20     35166       the first version
 */
#include "beep.h"

/*
 * 函数名：Beep_Init()
 *功能：蜂鸣器初始化
 * */
void Beep_Init()
{
    rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);
}

/*
 * 函数名：Beep_Sound_1
 *功能：蜂鸣器响一下
 **/
void Beep_Sound_1()
{
    rt_pin_write(BEEP_PIN,PIN_HIGH);
    rt_thread_mdelay(100);
    rt_pin_write(BEEP_PIN,PIN_LOW);
}

