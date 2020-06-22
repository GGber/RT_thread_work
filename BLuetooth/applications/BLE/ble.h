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

#define KEY_PIN         GET_PIN(B, 5)  //�������Ŷ���
#define BLE_PIN_EN      GET_PIN(A, 1)  //����EN���Ŷ���
#define BLE_PIN_KEY     GET_PIN(A, 4)  //����AT�����л���      (�͵�ƽΪAT)
#define BLE_PIN_STA     GET_PIN(A, 5)  //��������״̬��    (δ����Ϊ�ͣ�������Ϊ��)
#define BLE_PIN_REST    GET_PIN(A, 6)  //������λ����     ���͵�ƽ����5ms��Ч��

#define MAX_BUFF_LEN    4              //������յ�������ݳ���

#define STA_EN      1
#define STA_DIS     0

/******������յ��Ĺ�����******/
#define BLE_CODE_TIME    0x01       //����ǩ��ʱ��

 /* ���ڽ�����Ϣ������ */
extern unsigned char BLE_RX_Buffer_A[MAX_BUFF_LEN];//ǩ������
extern unsigned int BLE_RX_NUM_A;

extern unsigned char BLE_RX_Buffer_B[MAX_BUFF_LEN];//��������������
extern unsigned int BLE_RX_NUM_B;


void BLE_PIN_Init();

#endif /* APPLICATIONS_BLE_BLE_H_ */
