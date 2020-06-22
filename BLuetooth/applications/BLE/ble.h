/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-20     35166       the first version
 */
#ifndef APPLICATIONS_BLE_BLE_H_
#define APPLICATIONS_BLE_BLE_H_

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include "oled.h"
#include "beep.h"

#define UART_NAME       "uart2"
#define UART_BLE_THREAD_SIZE    2048
#define UART_BLE_THREAD_PAR     20
#define UART_BLE_THREAD_TACK    10

#define KEY_PIN         GET_PIN(B, 5)  //按键引脚定义
#define BLE_PIN_EN      GET_PIN(A, 1)  //蓝牙EN引脚定义
#define BLE_PIN_KEY     GET_PIN(A, 4)  //蓝牙AT命令切换脚      (低电平为AT)
#define BLE_PIN_STA     GET_PIN(A, 5)  //蓝牙连接状态脚    (未连接为低，连接上为高)
#define BLE_PIN_REST    GET_PIN(A, 6)  //蓝牙复位引脚     （低电平持续5ms有效）

#define MAX_BUFF_LEN    4              //定义接收的最大数据长度

#define STA_EN      1
#define STA_DIS     0

/******定义接收到的功能码******/
#define BLE_CODE_TIME    0x01       //设置签到时间

 /* 用于接收消息的数组 */
extern unsigned char BLE_RX_Buffer_A[MAX_BUFF_LEN];//签到数组
extern unsigned int BLE_RX_NUM_A;

extern unsigned char BLE_RX_Buffer_B[MAX_BUFF_LEN];//功能码设置数组
extern unsigned int BLE_RX_NUM_B;


void BLE_PIN_Init();

#endif /* APPLICATIONS_BLE_BLE_H_ */
