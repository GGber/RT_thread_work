/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-20     35166       the first version
 */
#include "beep.h"

#define DBG_TAG "BEEP"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int BEEP_EN=1;  //使能蜂鸣器
int Beep_sta=0; //蜂鸣器状态
/*
 * 函数名：Beep_Sound_1
 *功能：蜂鸣器响一下
 **/
static void Beep_Sound_1()
{
    rt_pin_write(BEEP_PIN,PIN_HIGH);
    rt_thread_mdelay(200);
    rt_pin_write(BEEP_PIN,PIN_LOW);
}

/*
 * 函数名：Beep_Init()
 *功能：蜂鸣器初始化
 * */
void Beep_Init()
{
    rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);
}

static void Beep_thread_entry(void *parameter)
{
    Beep_Init();

    while(1)
    {
        if(BEEP_EN)//如果开启BEEP
        {
            switch (Beep_sta) {

                case BEEP_SOUND_FREE : break;

                case BEEP_SOUND_ONE:   Beep_Sound_1();  Beep_sta=BEEP_SOUND_FREE;
                break;

                default:
                    break;
            }
        }
        rt_thread_mdelay(500);
    }
}
static int Beep_thread_init(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("Beep_thread", Beep_thread_entry, RT_NULL, BEEP_THREAD_SIZE, BEEP_THREAD_PAR, BEEP_THREAD_TACK);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

INIT_APP_EXPORT(Beep_thread_init);
