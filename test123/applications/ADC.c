/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-30     35166       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>

#define ADC_DEV_NAME        "adc1"      /* ADC �豸���� */
#define ADC_DEV_CHANNEL     7           /* ADC ͨ�� */
#define REFER_VOLTAGE       330         /* �ο���ѹ 3.3V,���ݾ��ȳ���100����2λС��*/
#define CONVERT_BITS        (1 << 12)   /* ת��λ��Ϊ12λ */

rt_adc_device_t adc_dev;

static void adc_vol_farm(int argc, char *argv[])
{

    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* �����豸 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);

    /* ʹ���豸 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);

    /* ��ȡ����ֵ */
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
    rt_kprintf("the value is :%d \n", value);

    /* ת��Ϊ��Ӧ��ѹֵ */
    vol = value * REFER_VOLTAGE / CONVERT_BITS;
    rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);

//    /* �ر�ͨ�� */
//    ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

}
INIT_APP_EXPORT(adc_vol_farm);
/* ������ msh �����б��� */
MSH_CMD_EXPORT(adc_vol_farm, adc voltage convert sample);
