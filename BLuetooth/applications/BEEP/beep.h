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

#define BEEP_PIN    GET_PIN(B, 10)  //·äÃùÆ÷Òý½Å¶¨Òå


void Beep_Init();
void Beep_Sound_1();

#endif /* APPLICATIONS_BEEP_BEEP_H_ */
