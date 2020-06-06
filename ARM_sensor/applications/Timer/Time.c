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

#define Time1_out_data      5000     //定时超时时间为5s,判断读取数据是否正常
#define Conversion_out      0x03   //转换超时
#define Conversion_now      0x02   //正在转换
#define Conversion_free     0x01   //空闲状态
#define Conversion_end      0x04   //转换结束
#define Time_AD_out         10   //超时的次数
/* 定时器的控制块 */
rt_timer_t timer1;
int time1_outnum=0;                 //定义超时的次数变量


extern rt_thread_t thread_AD;
extern uint8_t Conversion_sta;
/* 定时器 1 超时函数 */
static void time1_out()
{

    time1_outnum++;
    if(time1_outnum>Time_AD_out)
    {
//        rt_thread_detach(thread_AD);
        rt_thread_delete(thread_AD);
        rt_timer_stop(timer1);
        LOG_I("Have deleted AD thread\n");
        usSRegInBuf[6]= 0x0001;//AD炸了
    }else {
        usSRegInBuf[6]= 0x0000;
    }

    Conversion_sta=Conversion_out;
}

int timer_thread(void)
{
    /* 创建定时器 1  周期定时器 */
     timer1=rt_timer_create("timer_AD", time1_out, RT_NULL, Time1_out_data, RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_HARD_TIMER);//1s中断一次

    return 0;
}

INIT_APP_EXPORT(timer_thread);
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(timer_thread, timer_thread_controlAD);
