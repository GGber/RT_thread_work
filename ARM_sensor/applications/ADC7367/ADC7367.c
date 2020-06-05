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

#define Conversion_out      0x03   //转换超时
#define Conversion_now      0x02   //正在转换
#define Conversion_free     0x01   //空闲状态
#define Conversion_end      0x04   //转换结束
#define AD_k                1.21420087 //AD转换系数

rt_thread_t thread_AD;

uint8_t Conversion_sta=Conversion_free;  //AD数据转换初始状态
uint16_t AD_data_A;             //数据A
uint16_t AD_data_B;             //数据B
uint16_t AD_data_A_last;             //数据A上一个值
uint16_t AD_data_B_last;             //数据B上一个值

/*
 * 函数名称：ADC7367_BUSY_Rt
 * 函数功能：busy信号下降沿中断函数
 * */
static void ADC7367_BUSY_Rt()
{
    Conversion_sta=Conversion_end;//数据转换结束

}

/*
 * 函数名称：ADC7367_conversion
 * 函数功能：开启一次数据的转换
 * 函数返回：
 * */
static void ADC7367_conversion()
{
    rt_pin_write(ADC7367_CNVST_PIN, PIN_LOW);   //CNVST引脚给低
    rt_hw_us_delay(1);                          //延时1us
    rt_pin_write(ADC7367_CNVST_PIN, PIN_HIGH);  //CNVST引脚给高
}
/*
 * 函数名称：ADC7367_read_data
 * 函数功能：读一次AD的14位数据
 * */
static void ADC7367_read_data()
{
    AD_data_A=0;
    AD_data_B=0;
    rt_pin_write(ADC7367_CLK_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_CS_PIN, PIN_LOW);

    if(rt_pin_read(ADC7367_OUTA_PIN))   AD_data_A|=1<<13;//获取最高位
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
 * 函数名称：ADC7367_thread_entry
 * 函数功能：ADC7367线程
 * */
static void ADC7367_thread_entry(void *parameter)
{
    int AD_A_data_Int=0;

    rt_pin_mode(ADC7367_OUTA_PIN, PIN_MODE_INPUT_PULLDOWN);     //下拉输入
    rt_pin_mode(ADC7367_OUTB_PIN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(ADC7367_BUSY_PIN, PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(ADC7367_CS_PIN, PIN_MODE_OUTPUT);              //推挽输出
    rt_pin_mode(ADC7367_CLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_R0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_R1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_CNVST_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(ADC7367_ADDR_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(ADC7367_CNVST_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_CLK_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_CS_PIN, PIN_HIGH);
    rt_pin_write(ADC7367_R0_PIN, PIN_LOW);          //模拟输入范围选择+-10V
    rt_pin_write(ADC7367_R1_PIN, PIN_LOW);
    rt_pin_write(ADC7367_ADDR_PIN, PIN_LOW);        //选择通道A1和B1

    rt_pin_attach_irq(ADC7367_BUSY_PIN, PIN_IRQ_MODE_FALLING, ADC7367_BUSY_Rt, RT_NULL); // Busy引脚中断（下降沿触发)
    rt_pin_irq_enable(ADC7367_BUSY_PIN, PIN_IRQ_ENABLE);

    rt_thread_mdelay(100);
    while (1)
    {
        switch (Conversion_sta) {

            case Conversion_free:
                rt_timer_start(timer1);  //重启定时器
                LOG_I("ADC start conversion！\n");
                ADC7367_conversion();   //转换一次数据
                break;

            case Conversion_out:
                LOG_W("ADC conversion out time！,number:%d\n",time1_outnum);
                Conversion_sta=Conversion_free;
                break;

            case Conversion_end:            //数据转换结束，开始读数据
                LOG_I("ADC conversion over!\n");
                ADC7367_read_data();

                if(AD_data_A&0x2000)    //判断正负电压
                {
                    AD_A_data_Int=((~AD_data_A)&0x1FFF)*AD_k;   //负数就取补码
                    LOG_I("Data A(V): -%d V\n",AD_A_data_Int);
                    usSRegInBuf[3]= AD_A_data_Int | 0x8000;
                }else {
                    AD_A_data_Int=(AD_data_A&0x1FFF)*AD_k;
                    LOG_I("Data A(V): +%d V\n",AD_A_data_Int);
                    usSRegInBuf[3]= AD_A_data_Int;
                }

//                LOG_I("Data A(V): %04x V\n",AD_data_A);
//                LOG_I("Data B(I): %04x A\n",AD_data_B);
                time1_outnum=0;                   //定时器 超时次数清0
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

    /* 创建 serial 动态线程 */
    thread_AD = rt_thread_create("serial_ADC7367", ADC7367_thread_entry, RT_NULL,
                                            ADC7367_stack_size,
                                            ADC7367_priority,
                                            ADC7367_tick);
    /* 创建成功则启动线程 */
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


/* 加入到 初始化 列表中 */
INIT_APP_EXPORT(ADC7367_init);
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(ADC7367_init, ADC7367_init device Init function);
