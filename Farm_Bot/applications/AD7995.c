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

#define ZERO_VALUE  5       //����޶�ֵ
#define Sampling_num 11      //�����ĸ���

typedef struct//����ͨ���ɼ���������
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
 * IIC��ʼ��
 * */
static void AD7995_Init(void)
{
    rt_pin_mode(IIC_SCL, PIN_MODE_OUTPUT);
    rt_pin_mode(IIC_SDA, PIN_MODE_OUTPUT);
    SCL_HIGH;
    SDA_HIGH;
}

/*
 * ����IIC��ʼ�ź�
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
 * ����IICֹͣ�ź�
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

//����ACKӦ��
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

//������ACKӦ��
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

//��2���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
static rt_uint16_t IIC_Read_Byte(rt_uint8_t ack)
{
    rt_uint8_t i;
    rt_uint16_t receive=0;
    SDA_IN;//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        SCL_LOW;
        rt_hw_us_delay(2);
        SCL_HIGH;
        if(READ_SDA==PIN_HIGH)receive|=1<<(15-i);
            rt_hw_us_delay(2);
    }

    IIC_Ack(); //����ACK

    for(i=0;i<8;i++ )
    {
        SCL_LOW;
        rt_hw_us_delay(2);
        SCL_HIGH;
        if(READ_SDA==PIN_HIGH)receive|=1<<(7-i);
            rt_hw_us_delay(2);
   }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}


//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
static void IIC_Send_Byte(rt_uint8_t txd)
{
    rt_uint8_t t;
    SDA_OUT;
    SCL_LOW;//����ʱ�ӿ�ʼ���ݴ���
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
//��AD7995ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
static rt_uint16_t AD7995_ReadOneByte(rt_uint8_t ReadAddr)
{
    rt_uint16_t temp=0;
    ReadAddr=ReadAddr<<1|0x01;  //������
    IIC_START();
    IIC_Send_Byte(ReadAddr);//���͵�ַ
    IIC_Ack();
    temp=IIC_Read_Byte(1);    //0������ת��
    IIC_STOP();//����һ��ֹͣ����
    return temp;
}

//��AD7995ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
static void AD7995_WriteOneByte(rt_uint8_t WriteAddr,rt_uint8_t DataToWrite)
{
    WriteAddr=WriteAddr<<1; //д����
    IIC_START();
    IIC_Send_Byte(WriteAddr);     //���͵�ַ
    IIC_Ack();
    IIC_Send_Byte(DataToWrite);     //�����ֽ�
    IIC_Ack();
    IIC_STOP();//����һ��ֹͣ����
    rt_thread_mdelay(10);
}
//ð�����򣬻�ȡ����һ����
//str  :  ���������
//����   : ��2/3����ֵ
static rt_uint16_t Value_MaxMin(rt_uint16_t *str)//�ɴ�С����
{
    rt_uint8_t i,j;
    rt_uint16_t max;
    for(i=0;i<Sampling_num;i++)//ð������
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

/*�������㺯��
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

rt_uint16_t AD_last_value;//��һ��ADֵ
rt_uint16_t AD_Now_value;//��ǰADֵ

static void AD7995_poll(void *parameter)
{
    rt_uint16_t AD7995_Recive;  //ԭʼֵ
    rt_uint16_t current=0;      //����ֵ
    rt_uint8_t Curnum=0;        //ͨ�����
    rt_uint8_t AD_Number=0;     //AD����
    rt_uint8_t Tim_Dell=0;      //�����ʱ
    rt_uint8_t Getnum=0;        //ͨ������
    rt_uint32_t sum=0;          //ͨ���ۼ��ܺ�

    AD7995_Init();
    AD7995_WriteOneByte(AD7995_0,0x17);//��ȡ��1��ͨ��
    AD_Number=1;                //ѭ������

    while (1)
    {

        if(usSRegHoldBuf[9])//����AD�ɼ�ͨ��
        {
             Clear_AD();
             AD_Number=0;
             AD7995_WriteOneByte(AD7995_0,usSRegHoldBuf[9]<<4|0x07);//ȡ��4λ����λ��ʾͨ��0
             if(usSRegHoldBuf[9]&1) AD_Number++;
             if(usSRegHoldBuf[9]&2) AD_Number++;
             if(usSRegHoldBuf[9]&4) AD_Number++;
             if(usSRegHoldBuf[9]&8) AD_Number++;
             //LOG_I("��ͨ������:%d��д���֣�%x\r\n",AD_Number,usSRegHoldBuf[9]<<4|0x07);
             usSRegHoldBuf[9]=0;
        }

        for(int i=0;i<AD_Number;i++)
        {
            AD7995_Recive=AD7995_ReadOneByte(AD7995_0);             //��ȡADֵ
            current=(AD7995_Recive&0x0fff)>>2;                      //��ǰ��ѹֵ
            Curnum=(AD7995_Recive&0x3000)>>12;                      //��ǰ���
            if(current>=ZERO_VALUE)                                 //�ų�����
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
        if(Tim_Dell>=Sampling_num)//����Ԥ��ֵ�Ϳ�ʼ��������
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
            if(AD_Now_value>ZERO_VALUE)//ȷ�������ϴ�0ֵ
            {
                usSRegHoldBuf[10]=AD_Now_value;
                AD_last_value=AD_Now_value;
            }else {
                AD_Now_value=AD_last_value;
            }

            //rt_kprintf("ͨ��1��%d,ͨ��2��%d,������%d,ƽ��ֵΪ:%d\r\n",AD_CH0.value_now,AD_CH1.value_now,Getnum,usSRegHoldBuf[10]);

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
