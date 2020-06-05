/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-22     35166       the first version
 */
#ifndef APPLICATIONS_TIMER_TIME_H_
#define APPLICATIONS_TIMER_TIME_H_

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include "mb.h"
#include "user_mb_app.h"

#define DBG_TAG "Timer"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/********************Modbus¼Ä´æÆ÷***********************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];
extern USHORT usSRegInBuf[S_REG_INPUT_NREGS];

#endif /* APPLICATIONS_TIMER_TIME_H_ */
