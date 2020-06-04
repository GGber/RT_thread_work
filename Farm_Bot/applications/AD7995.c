#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include "user_mb_app.h"

#define DBG_TAG "AD7995"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define AD7991_0        0x28
#define AD7991_1        0x29
#define AD7995_0        0x28
#define AD7995_1        0x29
#define AD7999_1        0x29

#define AD7995_THREAD_PRIORITY  11

#define SCL_HIGH  rt_pin_write(IIC_SCL,1)
#define SCL_LOW   rt_pin_write(IIC_SCL,0)
#define SDA_OUT   rt_pin_mode(IIC_SDA, PIN_MODE_OUTPUT)
#define SDA_IN    rt_pin_mode(IIC_SDA, PIN_MODE_INPUT)
#define READ_SDA  rt_pin_read(IIC_SDA)
#define SDA_HIGH  rt_pin_write(IIC_SDA,1)
#define SDA_LOW   rt_pin_write(IIC_SDA,0)

#define ZERO_VALUE  5       //最低限度值
#define Sampling_num 11      //采样的个数

typedef struct//定义通道采集到的数据
{
    rt_uint8_t num;
    rt_uint16_t AD_value[Sampling_num];
    rt_uint16_t value_now;
}AD7995_value;

AD7995_value AD_CH1;
AD7995_value AD_CH2;
AD7995_value AD_CH3;
AD7995_value AD_CH0;

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

/*
 * IIC初始化
 * */
static void AD7995_Init(void)
{
    rt_pin_mode(IIC_SCL, PIN_MODE_OUTPUT);
    rt_pin_mode(IIC_SDA, PIN_MODE_OUTPUT);
    SCL_HIGH;
    SDA_HIGH;
}

/*
 * 产生IIC起始信号
 * */
static void IIC_START(void)
{
    SDA_OUT;
    SCL_HIGH;
    SDA_HIGH;
    rt_hw_us_delay(4);
    SDA_LOW;
    rt_hw_us_delay(4);
    SCL_LOW;
}

/*
 * 产生IIC停止信号
 * */
static void IIC_STOP(void)
{
    SDA_OUT;
    SCL_LOW;
    SDA_LOW;
    rt_hw_us_delay(4);
    SCL_HIGH;
    SDA_HIGH;
    rt_hw_us_delay(4);
}

//产生ACK应答
static void IIC_Ack(void)
{
    SCL_LOW;
    SDA_OUT;
    SDA_LOW;
    rt_hw_us_delay(2);
    SCL_HIGH;
    rt_hw_us_delay(2);
    SCL_LOW;
}

//不产生ACK应答
static void IIC_NAck(void)
{
    SCL_LOW;
    SDA_OUT;
    SDA_HIGH;
    rt_hw_us_delay(2);
    SCL_HIGH;
    rt_hw_us_delay(2);
    SCL_LOW;
}

//读2个字节，ack=1时，发送ACK，ack=0，发送nACK
static rt_uint16_t IIC_Read_Byte(rt_uint8_t ack)
{
    rt_uint8_t i;
    rt_uint16_t receive=0;
    SDA_IN;//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        SCL_LOW;
        rt_hw_us_delay(2);
        SCL_HIGH;
        if(READ_SDA==PIN_HIGH)receive|=1<<(15-i);
            rt_hw_us_delay(2);
    }

    IIC_Ack(); //发送ACK

    for(i=0;i<8;i++ )
    {
        SCL_LOW;
        rt_hw_us_delay(2);
        SCL_HIGH;
        if(READ_SDA==PIN_HIGH)receive|=1<<(7-i);
            rt_hw_us_delay(2);
   }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}


//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
static void IIC_Send_Byte(rt_uint8_t txd)
{
    rt_uint8_t t;
    SDA_OUT;
    SCL_LOW;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>>7)
            SDA_HIGH;
        else
            SDA_LOW;
        txd<<=1;
        rt_hw_us_delay(2);
        SCL_HIGH;
        rt_hw_us_delay(2);
        SCL_LOW;
    }
}
//在AD7995指定地址读出一个数据
//ReadAddr:开始读数的地址
//返回值  :读到的数据
static rt_uint16_t AD7995_ReadOneByte(rt_uint8_t ReadAddr)
{
    rt_uint16_t temp=0;
    ReadAddr=ReadAddr<<1|0x01;  //读数据
    IIC_START();
    IIC_Send_Byte(ReadAddr);//发送地址
    IIC_Ack();
    temp=IIC_Read_Byte(1);    //0不继续转换
    IIC_STOP();//产生一个停止条件
    return temp;
}

//在AD7995指定地址写入一个数据
//WriteAddr  :写入数据的目的地址
//DataToWrite:要写入的数据
static void AD7995_WriteOneByte(rt_uint8_t WriteAddr,rt_uint8_t DataToWrite)
{
    WriteAddr=WriteAddr<<1; //写数据
    IIC_START();
    IIC_Send_Byte(WriteAddr);     //发送地址
    IIC_Ack();
    IIC_Send_Byte(DataToWrite);     //发送字节
    IIC_Ack();
    IIC_STOP();//产生一个停止条件
    rt_thread_mdelay(10);
}
//冒泡排序，获取其中一个数
//str  :  排序的数组
//返回   : 在2/3处的值
static rt_uint16_t Value_MaxMin(rt_uint16_t *str)//由大到小排序
{
    rt_uint8_t i,j;
    rt_uint16_t max;
    for(i=0;i<Sampling_num;i++)//冒泡排序
    {
        for(j=i;j<Sampling_num;j++)
        {
            if(str[i]<str[j])
            {
                max=str[j];
                str[j]=str[i];
                str[i]=max;
            }
        }
    }
    return str[Sampling_num*2/3];
}

/*数据清零函数
 * */
static void Clear_AD(void)
{
    AD_CH0.num=0;
    AD_CH1.num=0;
    AD_CH2.num=0;
    AD_CH3.num=0;
    AD_CH0.value_now=0;
    AD_CH1.value_now=0;
    AD_CH2.value_now=0;
    AD_CH3.value_now=0;
    memset(AD_CH0.AD_value,0,Sampling_num);
    memset(AD_CH1.AD_value,0,Sampling_num);
    memset(AD_CH2.AD_value,0,Sampling_num);
    memset(AD_CH3.AD_value,0,Sampling_num);
}

rt_uint16_t AD_last_value;//上一个AD值
rt_uint16_t AD_Now_value;//当前AD值

static void AD7995_poll(void *parameter)
{
    rt_uint16_t AD7995_Recive;  //原始值
    rt_uint16_t current=0;      //计算值
    rt_uint8_t Curnum=0;        //通道编号
    rt_uint8_t AD_Number=0;     //AD个数
    rt_uint8_t Tim_Dell=0;      //处理计时
    rt_uint8_t Getnum=0;        //通道个数
    rt_uint32_t sum=0;          //通道累加总和

    AD7995_Init();
    AD7995_WriteOneByte(AD7995_0,0x17);//读取第1个通道
    AD_Number=1;                //循环次数

    while (1)
    {

        if(usSRegHoldBuf[9])//设置AD采集通道
        {
             Clear_AD();
             AD_Number=0;
             AD7995_WriteOneByte(AD7995_0,usSRegHoldBuf[9]<<4|0x07);//取低4位，低位表示通道0
             if(usSRegHoldBuf[9]&1) AD_Number++;
             if(usSRegHoldBuf[9]&2) AD_Number++;
             if(usSRegHoldBuf[9]&4) AD_Number++;
             if(usSRegHoldBuf[9]&8) AD_Number++;
             //LOG_I("打开通道个数:%d，写入字：%x\r\n",AD_Number,usSRegHoldBuf[9]<<4|0x07);
             usSRegHoldBuf[9]=0;
        }

        for(int i=0;i<AD_Number;i++)
        {
            AD7995_Recive=AD7995_ReadOneByte(AD7995_0);             //读取AD值
            current=(AD7995_Recive&0x0fff)>>2;                      //当前电压值
            Curnum=(AD7995_Recive&0x3000)>>12;                      //当前编号
            if(current>=ZERO_VALUE)                                 //排除干扰
                {
                    switch(Curnum)
                        {
                            case 0: AD_CH0.AD_value[AD_CH0.num]=current;AD_CH0.num++; break;
                            case 1: AD_CH1.AD_value[AD_CH1.num]=current;AD_CH1.num++; break;
                            case 2: AD_CH2.AD_value[AD_CH2.num]=current;AD_CH2.num++; break;
                            case 3: AD_CH3.AD_value[AD_CH3.num]=current;AD_CH3.num++; break;

                        }
                }
            rt_thread_mdelay(1);
        }

        Tim_Dell++;
        if(Tim_Dell>=Sampling_num)//超出预设值就开始处理数据
        {
            Tim_Dell=0;
            if(AD_CH0.num>=Sampling_num)
            {
                AD_CH0.value_now=Value_MaxMin(AD_CH0.AD_value);
                usSRegHoldBuf[0]=AD_CH0.value_now;
                sum+=AD_CH0.value_now;
                Getnum++;
//                rt_kprintf("IN  000\r\n");
            }else usSRegHoldBuf[0]=0;

            if(AD_CH1.num>=Sampling_num)
            {
                AD_CH1.value_now=Value_MaxMin(AD_CH1.AD_value);
                sum+=AD_CH1.value_now;
                usSRegHoldBuf[1]=AD_CH1.value_now;
                Getnum++;
//                rt_kprintf("IN  001\r\n");
            }else usSRegHoldBuf[1]=0;

            if(AD_CH2.num>=Sampling_num)
            {
                AD_CH2.value_now=Value_MaxMin(AD_CH2.AD_value);
                sum+=AD_CH2.value_now;
                usSRegHoldBuf[2]=AD_CH2.value_now;
                Getnum++;
//                rt_kprintf("IN  002\r\n");
            }else usSRegHoldBuf[2]=0;

            if(AD_CH3.num>=Sampling_num)
            {
                AD_CH3.value_now=Value_MaxMin(AD_CH3.AD_value);
                sum+=AD_CH3.value_now;
                usSRegHoldBuf[3]=AD_CH3.value_now;
                Getnum++;
//                rt_kprintf("IN  003\r\n");
            }else usSRegHoldBuf[3]=0;

            AD_Now_value=sum/Getnum;
            if(AD_Now_value>ZERO_VALUE)//确保不会上传0值
            {
                usSRegHoldBuf[10]=AD_Now_value;
                AD_last_value=AD_Now_value;
            }else {
                AD_Now_value=AD_last_value;
            }

            //rt_kprintf("通道1：%d,通道2：%d,个数：%d,平均值为:%d\r\n",AD_CH0.value_now,AD_CH1.value_now,Getnum,usSRegHoldBuf[10]);

            sum=0;
            Getnum=0;
            Clear_AD();
        }
        rt_thread_mdelay(10);
    }
}

int AD7995_samlpe(void)
{
    static rt_uint8_t is_init = 0;
    rt_thread_t tid1 = RT_NULL;

    if (is_init > 0)
    {
        rt_kprintf("sample is running\n");
        return -RT_ERROR;
    }
    tid1 = rt_thread_create("AD7995_poll", AD7995_poll, RT_NULL, 512, AD7995_THREAD_PRIORITY, 100);
    if (tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else
    {
        goto __exit;
    }

    is_init = 1;
    return RT_EOK;

    __exit: if (tid1)
        rt_thread_delete(tid1);

    return -RT_ERROR;
}


INIT_DEVICE_EXPORT(AD7995_samlpe);
