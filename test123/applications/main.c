/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-09     RT-Thread    first version
 */

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <u8g2_port.h>


#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* PLEASE DEFINE the LED0 pin for your board, such as: PA5 */
#define LED0_PIN    GET_PIN(C, 13)
#define ADC_DEV_CHANNEL     7           /* ADC ͨ�� */
#define REFER_VOLTAGE       330         /* �ο���ѹ 3.3V,���ݾ��ȳ���100����2λС��*/
#define CONVERT_BITS        (1 << 12)   /* ת��λ��Ϊ12λ */

extern rt_adc_device_t adc_dev;
extern u8g2_t u8g2;
int main(void)
{
    rt_uint32_t value, vol;
    int count = 1;
    char *show_str;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        /* set LED0 pin level to high or low */
        rt_pin_write(LED0_PIN, count % 2);

        /* ��ȡ����ֵ */
        value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
        /* ת��Ϊ��Ӧ��ѹֵ */
        vol = value * REFER_VOLTAGE / CONVERT_BITS;
        rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);

        u8g2_ClearBuffer(&u8g2);
        u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
        u8g2_DrawStr(&u8g2, 0, 20, "Get Data:");
        sprintf(show_str,"%d", vol/2);
        u8g2_DrawStr(&u8g2, 60, 40, show_str);
        u8g2_SendBuffer(&u8g2);

        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
