/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-22     35166       the first version
 */
#include "Time.h"

#define Time1_out_data      5000     //��ʱ��ʱʱ��Ϊ5s,�ж϶�ȡ�����Ƿ�����
#define Conversion_out      0x03   //ת����ʱ
#define Conversion_now      0x02   //����ת��
#define Conversion_free     0x01   //����״̬
#define Conversion_end      0x04   //ת������
#define Time_AD_out         10   //��ʱ�Ĵ���
/* ��ʱ���Ŀ��ƿ� */
rt_timer_t timer1;
int time1_outnum=0;                 //���峬ʱ�Ĵ�������


extern rt_thread_t thread_AD;
extern uint8_t Conversion_sta;
/* ��ʱ�� 1 ��ʱ���� */
static void time1_out()
{

    time1_outnum++;
    if(time1_outnum>Time_AD_out)
    {
//        rt_thread_detach(thread_AD);
        rt_thread_delete(thread_AD);
        rt_timer_stop(timer1);
        LOG_I("Have deleted AD thread\n");
        usSRegInBuf[6]= 0x0001;//ADը��
    }else {
        usSRegInBuf[6]= 0x0000;
    }

    Conversion_sta=Conversion_out;
}

int timer_thread(void)
{
    /* ������ʱ�� 1  ���ڶ�ʱ�� */
     timer1=rt_timer_create("timer_AD", time1_out, RT_NULL, Time1_out_data, RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_HARD_TIMER);//1s�ж�һ��

    return 0;
}

INIT_APP_EXPORT(timer_thread);
/* ������ msh �����б��� */
MSH_CMD_EXPORT(timer_thread, timer_thread_controlAD);
