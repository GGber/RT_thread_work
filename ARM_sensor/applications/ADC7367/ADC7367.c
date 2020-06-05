/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-05     LD       the first version
 */
#include "ADC7367.h"

#define Conversion_out      0x03   //ת����ʱ
#define Conversion_now      0x02   //����ת��
#define Conversion_free     0x01   //����״̬
#define Conversion_end      0x04   //ת������
#define AD_k                1.21420087 //ADת��ϵ��

rt_thread_t thread_AD;

uint8_t Conversion_sta=Conversion_free;  //AD����ת����ʼ״̬
uint16_t AD_data_A;             //����A
uint16_t AD_data_B;             //����B
uint16_t AD_data_A_last;             //����A��һ��ֵ
uint16_t AD_data_B_last;             //����B��һ��ֵ

/*
 * �������ƣ�ADC7367_BUSY_Rt
 * �������ܣ�busy�ź��½����жϺ���
 * */
static void ADC7367_BUSY_Rt()
{
    Conversion_sta=Conversion_end;//����ת������

}

/*
 * �������ƣ�ADC7367_conversion
 * �������ܣ�����һ�����ݵ�ת��
 * �������أ�
 * */
static void ADC7367_conversion()
{
    rt_pin_write(ADC7367_CNVST_PIN, PIN_LOW);   //CNVST���Ÿ���
    rt_hw_us_delay(1);                          //��ʱ1us
    rt_pin_write(ADC7367_CNVST_PIN, PIN_HIGH);  //CNVST���Ÿ���
}
/*
 * �������ƣ�ADC7367_read_data
 * �������ܣ���һ��AD��14λ����
 * */
static void ADC7367_read_data()
{
    AD_data_A=0;
    AD_data_B=0;
    rt_pin_write(ADC7367_CLK_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_CS_PIN, PIN_LOW);

    if(rt_pin_read(ADC7367_OUTA_PIN))   AD_data_A|=1<<13;//��ȡ���λ
    if(rt_pin_read(ADC7367_OUTB_PIN))   AD_data_B|=1<<13;
    rt_hw_us_delay(1);
    for(int8_t i=12;i>=0;i--)
    {
        rt_pin_write(ADC7367_CLK_PIN, PIN_LOW);

        if(rt_pin_read(ADC7367_OUTA_PIN))   AD_data_A|=1<<i;
        if(rt_pin_read(ADC7367_OUTB_PIN))   AD_data_B|=1<<i;

        rt_hw_us_delay(1);
        rt_pin_write(ADC7367_CLK_PIN, PIN_HIGH);
        rt_hw_us_delay(1);
    }
    rt_pin_write(ADC7367_CS_PIN, PIN_HIGH);

}

/*
 * �������ƣ�ADC7367_thread_entry
 * �������ܣ�ADC7367�߳�
 * */
static void ADC7367_thread_entry(void *parameter)
{
    int AD_A_data_Int=0;

    rt_pin_mode(ADC7367_OUTA_PIN, PIN_MODE_INPUT_PULLDOWN);     //��������
    rt_pin_mode(ADC7367_OUTB_PIN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADC7367_BUSY_PIN, PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(ADC7367_CS_PIN, PIN_MODE_OUTPUT);              //�������
    rt_pin_mode(ADC7367_CLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_R0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_R1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_CNVST_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_ADDR_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(ADC7367_CNVST_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_CLK_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_CS_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_R0_PIN, PIN_LOW);          //ģ�����뷶Χѡ��+-10V
    rt_pin_write(ADC7367_R1_PIN, PIN_LOW);
    rt_pin_write(ADC7367_ADDR_PIN, PIN_LOW);        //ѡ��ͨ��A1��B1

    rt_pin_attach_irq(ADC7367_BUSY_PIN, PIN_IRQ_MODE_FALLING, ADC7367_BUSY_Rt, RT_NULL); // Busy�����жϣ��½��ش���)
    rt_pin_irq_enable(ADC7367_BUSY_PIN, PIN_IRQ_ENABLE);

    rt_thread_mdelay(100);
    while (1)
    {
        switch (Conversion_sta) {

            case Conversion_free:
                rt_timer_start(timer1);  //������ʱ��
                LOG_I("ADC start conversion��\n");
                ADC7367_conversion();   //ת��һ������
                break;

            case Conversion_out:
                LOG_W("ADC conversion out time��,number:%d\n",time1_outnum);
                Conversion_sta=Conversion_free;
                break;

            case Conversion_end:            //����ת����������ʼ������
                LOG_I("ADC conversion over!\n");
                ADC7367_read_data();

                if(AD_data_A&0x2000)    //�ж�������ѹ
                {
                    AD_A_data_Int=((~AD_data_A)&0x1FFF)*AD_k;   //������ȡ����
                    LOG_I("Data A(V): -%d V\n",AD_A_data_Int);
                    usSRegInBuf[3]= AD_A_data_Int | 0x8000;
                }else {
                    AD_A_data_Int=(AD_data_A&0x1FFF)*AD_k;
                    LOG_I("Data A(V): +%d V\n",AD_A_data_Int);
                    usSRegInBuf[3]= AD_A_data_Int;
                }

//                LOG_I("Data A(V): %04x V\n",AD_data_A);
//                LOG_I("Data B(I): %04x A\n",AD_data_B);
                time1_outnum=0;                   //��ʱ�� ��ʱ������0
                Conversion_sta=Conversion_free;
                rt_thread_mdelay(1000);
                break;

            default:

                break;
        }

        rt_hw_us_delay(1);

    }
}




static int ADC7367_init()
{
    rt_err_t ret = RT_EOK;

    /* ���� serial ��̬�߳� */
    thread_AD = rt_thread_create("serial_ADC7367", ADC7367_thread_entry, RT_NULL,
                                            ADC7367_stack_size,
                                            ADC7367_priority,
                                            ADC7367_tick);
    /* �����ɹ��������߳� */
    if (thread_AD != RT_NULL)
    {
        rt_thread_startup(thread_AD);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}


/* ���뵽 ��ʼ�� �б��� */
INIT_APP_EXPORT(ADC7367_init);
/* ������ msh �����б��� */
MSH_CMD_EXPORT(ADC7367_init, ADC7367_init device Init function);
