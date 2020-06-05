/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-01     86132       the first version
 */
#ifndef APPLICATIONS_RS232_RS232_H_
#define APPLICATIONS_RS232_RS232_H_

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>

#define DBG_TAG "USART2_DSP"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define USE_UART_NAME       "uart2"
#define USART2_stack_size   1024
#define USART2_priority     12
#define USART2_tick         10


#endif /* APPLICATIONS_RS232_RS232_H_ */
