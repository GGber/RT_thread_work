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

int BEEP_EN=1;  //ʹ�ܷ�����
int Beep_sta=0; //������״̬
/*
 * ��������Beep_Sound_1
 *���ܣ���������һ��
 **/
static void Beep_Sound_1()
{
    rt_pin_write(BEEP_PIN,PIN_HIGH);
    rt_thread_mdelay(200);
    rt_pin_write(BEEP_PIN,PIN_LOW);
}

/*
 * ��������Beep_Init()
 *���ܣ���������ʼ��
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
        if(BEEP_EN)//�������BEEP
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

    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("Beep_thread", Beep_thread_entry, RT_NULL, BEEP_THREAD_SIZE, BEEP_THREAD_PAR, BEEP_THREAD_TACK);
    /* �����ɹ��������߳� */
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
