/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-05     LD       the first version
 */
#ifndef APPLICATIONS_ADC7367_ADC7367_H_
#define APPLICATIONS_ADC7367_ADC7367_H_

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include "string.h"
#include "mb.h"
#include "user_mb_app.h"

#define DBG_TAG "ADC7367"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/********************AD时钟测试频率配置宏定义***********************/
#define AD_TEST_CLK_PIN   GET_PIN(B, 6)

/********************线程配置宏定义***********************/
#define ADC7367_stack_size   1024
#define ADC7367_priority     13
#define ADC7367_tick         10

/********************ADC7367引脚宏定义***********************/
#define ADC7367_OUTA_PIN    GET_PIN(B, 12)
#define ADC7367_OUTB_PIN    GET_PIN(C, 6)
#define ADC7367_CS_PIN      GET_PIN(A, 8)
#define ADC7367_R0_PIN      GET_PIN(B, 14)
#define ADC7367_R1_PIN      GET_PIN(B, 13)
#define ADC7367_CLK_PIN     GET_PIN(C, 9)
#define ADC7367_CNVST_PIN   GET_PIN(C, 8)
#define ADC7367_BUSY_PIN    GET_PIN(C, 7)
#define ADC7367_ADDR_PIN    GET_PIN(B, 15)

/********************定时器宏定义***********************/
#define HWTIMER_DEV_NAME   "timer2"     /* 定时器名称 */
extern int time1_outnum;                 //定义超时的次数
extern rt_timer_t timer1;

/********************Modbus寄存器***********************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];
extern USHORT usSRegInBuf[S_REG_INPUT_NREGS];
#endif /* APPLICATIONS_ADC7367_ADC7367_H_ */
