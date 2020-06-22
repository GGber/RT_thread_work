/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-20     35166       the first version
 */
#ifndef APPLICATIONS_BEEP_BEEP_H_
#define APPLICATIONS_BEEP_BEEP_H_

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>



#define BEEP_PIN    GET_PIN(B, 10)  //蜂鸣器引脚定义
#define BEEP_THREAD_SIZE        1024
#define BEEP_THREAD_PAR         18
#define BEEP_THREAD_TACK        10

#define BEEP_SOUND_FREE            0x00
#define BEEP_SOUND_ONE              0x01


void Beep_Init();

extern int BEEP_EN;         //BEEP使能
extern int Beep_sta;        //BEEP状态sta

#endif /* APPLICATIONS_BEEP_BEEP_H_ */
