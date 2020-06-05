/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-05     LD       the first version
 */
#ifndef APPLICATIONS_RS422_RS422_H_
#define APPLICATIONS_RS422_RS422_H_

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>

#include "mb.h"
#include "user_mb_app.h"

#define DBG_TAG "RS422_PLC"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/********************Modbus配置宏定义***********************/
#define SLAVE_ADDR      0x01
#define PORT_NUM        3                   //串口3

#define PORT_BAUDRATE   9600

#define PORT_PARITY     MB_PAR_NONE         //无校验
#define MB_POLL_CYCLE_MS 100                 //轮训时间

/********************线程配置宏定义***********************/

#define MB_POLL_UART_NAME  "uart3_422"
#define MB_POLL_STACK_SIZE  1024
#define MB_POLL_THREAD_PRIORITY  11
#define MB_POLL_THREAD_TICK  10

/********************RS422引脚宏定义***********************/
#define RS422_RE_PIN    GET_PIN(B, 1)
#define RS422_DE_PIN    GET_PIN(B, 2)

/*********************按键引脚宏定义**********************/
#define KEY_PIN_1       GET_PIN(D,2)
#define KEY_PIN_2       GET_PIN(C,12)
#define KEY_PIN_3       GET_PIN(C,11)
#define KEY_PIN_4       GET_PIN(C,10)

#endif /* APPLICATIONS_RS422_RS422_H_ */
