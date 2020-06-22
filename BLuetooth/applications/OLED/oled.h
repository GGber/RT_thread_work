/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-20     35166       the first version
 */
#ifndef APPLICATIONS_OLED_OLED_H_
#define APPLICATIONS_OLED_OLED_H_
#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <rthw.h>
#include <u8g2_port.h>

#define OLED_I2C_PIN_SCL                    23
#define OLED_I2C_PIN_SDA                    22

#define SIGN_STA_SHOW_CLOSE      0x00
#define SIGN_STA_SIGN_START      0x01
#define SIGN_STA_SET_OK          0x02
#define SIGN_STA_SET_ERROR       0x03

extern int Sign_number;         //��ǩ��������
extern int Sign_sta;            //��ǰǩ��״̬
extern int Sign_time_run;       //��ǩ����ʱ��
extern int Sign_time_set;       //����ǩ����ʱ��

void OLED_Init(void);
void OLED_Show_str();


#endif /* APPLICATIONS_OLED_OLED_H_ */
