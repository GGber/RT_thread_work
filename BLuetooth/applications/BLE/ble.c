/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-20     35166       the first version
 */
#include "ble.h"

#define DBG_TAG "BLE"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include<string.h>

struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* 用于发送消息的数组 */
unsigned char BLE_TX_Buffer[MAX_BUFF_LEN]={0xAA,0x11,0x22,0xEE}; //普通发送模式，发送数组
char *BLE_AT_CLOSE = "AT+DIS?\r\n";                              //AT中断指令

/* 用于接收消息的数组 */
unsigned char BLE_RX_Buffer_A[MAX_BUFF_LEN];
unsigned int BLE_RX_NUM_A=0;

unsigned char BLE_RX_Buffer_B[MAX_BUFF_LEN];
unsigned int BLE_RX_NUM_B=0;

/* 用于定时签到 */
static rt_timer_t Timer_sign;       //1s中断一次


/*
 * 函数名：Serial_Buffer_clear()
 *功能：清空串口接收数组
 **/
static void Serial_Buffer_clear()
{
    BLE_RX_NUM_A=0;
    BLE_RX_NUM_B=0;
}

/*
 * 函数名：BLE_Send_AT()
 *功能：BLE发送AT指令
 *参数：str:指令,size:数组个数
 **/
void BLE_Send_AT(char *str,int size)
{
    rt_pin_write(BLE_PIN_KEY, PIN_LOW);//进入AT指令
    rt_thread_mdelay(1);
    rt_device_write(serial, 0,str,size);
}

/*
 * 函数名：BLE_Send_NOAT()
 *功能：BLE发送透传数据
 *参数：str:数据,size:数组个数
 *备注：默认为AT模式，只发不收，可以防止误收问题。
 **/
void BLE_Send_NOAT(unsigned char *str,int size)
{
    rt_pin_write(BLE_PIN_KEY, PIN_HIGH);//进入透传
    rt_thread_mdelay(1);
    rt_device_write(serial, 0,str,size);
}

/*
 * 函数名：KEY_ENT_Function
 *功能：按键中断服务函数
 *描述：用于唤醒BLE开启签到
 * */
void KEY_ENT_Function()
{
    Beep_sta=BEEP_SOUND_ONE;
    Sign_sta=SIGN_STA_SIGN_START;         //开启签到
    Sign_number=0;                        //初始化签到人数
    Sign_time_run=Sign_time_set;          //初始化签到运行时间
    rt_pin_write(BLE_PIN_REST, PIN_HIGH); //开启BLE模块
    rt_timer_start(Timer_sign);           //开启签到定时器
}

/*
 * 函数名：BLE_CONN_Function()
 *功能：BLE连接成功
 *描述：手机连接上之后会出发BLE状态脚拉高，进入中断后向手机发送功能码：0x11、0x22
 **/
void BLE_CONN_Function()
{
    Serial_Buffer_clear();
//    LOG_I("connect success!");
    BLE_Send_NOAT(BLE_TX_Buffer, MAX_BUFF_LEN);
}

/*
 * 函数名：BLE_RXB_ANA()
 *功能：接收配置字符串解析
 **/
static void BLE_RXB_ANA()
{
    int16_t Code = BLE_RX_Buffer_B[1];

    Serial_Buffer_clear();
    switch(Code)
    {
        case BLE_CODE_TIME:
            Sign_time_set = BLE_RX_Buffer_B[2];
            Sign_sta = SIGN_STA_SET_OK;
            break;
        case BLE_CODE_BEEP_EN:
            BEEP_EN = BLE_RX_Buffer_B[2];
            Sign_sta = SIGN_STA_SET_OK;
            break;

        default: break;
    }
}

/*
 * 函数名：BLE_RXA_ANA()
 *功能：接收配置字符串解析
 *描述：成功接收到手机返回的消息帧之后，发送断开的AT指令
 **/
static void BLE_RXA_ANA()
{
    Serial_Buffer_clear();
    BLE_Send_AT(BLE_AT_CLOSE, 9);
}

/*
 * 函数名：BLE_RST()
 *功能：BLE硬件复位一次(5ms以上的低电平)
 **/
void BLE_RST()
{
    rt_pin_write(BLE_PIN_REST, PIN_LOW);
    rt_thread_mdelay(10);
    rt_pin_write(BLE_PIN_REST, PIN_HIGH);
    rt_thread_mdelay(500);// Tips: Because BLE_PIN_STA will be RISING_FALLIN among ble reset, so add 500ms to presure the reset status end.
                          // Later, we can enable the PIN_IRQ_MODE_RISING interrupt for BLE_PIN_STA GPIO.
}


/*
 * 函数名：BLE_PIN_Init()
 *功能：BLE引脚初始化
 **/
void BLE_PIN_Init()
{

    rt_pin_mode(BLE_PIN_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(BLE_PIN_KEY, PIN_MODE_OUTPUT);
    rt_pin_mode(BLE_PIN_REST, PIN_MODE_OUTPUT);
    rt_pin_mode(KEY_PIN, PIN_MODE_INPUT);
    rt_pin_mode(BLE_PIN_STA, PIN_MODE_INPUT);

    rt_thread_mdelay(1);
    rt_pin_write(BLE_PIN_KEY, PIN_LOW); //Default :AT-Mode.
    rt_pin_write(BLE_PIN_REST, PIN_LOW);
    rt_pin_write(BLE_PIN_EN, PIN_HIGH);

    rt_pin_attach_irq(KEY_PIN, PIN_IRQ_MODE_FALLING, KEY_ENT_Function, RT_NULL);
    rt_pin_attach_irq(BLE_PIN_STA, PIN_IRQ_MODE_RISING, BLE_CONN_Function, RT_NULL);

    /* 使能中断 */
    rt_pin_irq_enable(BLE_PIN_STA, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY_PIN, PIN_IRQ_ENABLE);

}

/*定时器超时函数*/
static void Sign_time_out()
{
    if(Sign_time_run>0)
        Sign_time_run--;
    else {
        Sign_time_run=0;
        rt_pin_write(BLE_PIN_REST, PIN_LOW);    //关闭BLE模块
        rt_timer_stop(Timer_sign);              //签到结束，停止定时器
    }
}
/*
 * 函数名：BLE_Time_Init()
 *功能：创建定时器用于签到时间设置
 **/
void BLE_Time_Init()
{
    Timer_sign = rt_timer_create("Timer_sign", Sign_time_out,RT_NULL, 1000,RT_TIMER_FLAG_PERIODIC);
}

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_ble_thread_entry(void *parameter)
{
    char ch;

    BLE_PIN_Init();

    BLE_Time_Init();

    while (1)
    {
            /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
            while (rt_device_read(serial, -1, &ch, 1) != 1)
            {
                /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
                rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
            }

//            LOG_I("%x",ch);

            /*******************包头包尾分别为AA EE：签到模式返回帧******************/
            if( BLE_RX_NUM_A == 0 && ch =='\xAA' )
            {
                BLE_RX_Buffer_A[BLE_RX_NUM_A]=ch;
                BLE_RX_NUM_A++;
//                LOG_I("RX AA");
            }
            else if( BLE_RX_NUM_A > 0 && BLE_RX_NUM_A < MAX_BUFF_LEN-1)
            {
                BLE_RX_Buffer_A[BLE_RX_NUM_A]=ch;
                BLE_RX_NUM_A++;
//                LOG_I("RX Number:%d",BLE_RX_NUM_A);
            }
            else if( BLE_RX_NUM_A == MAX_BUFF_LEN-1 && ch =='\xEE')
            {
                BLE_RX_Buffer_A[BLE_RX_NUM_A]=ch;
//                LOG_I("RX over");
                Sign_number++;                          //签到人数增加一个
                BLE_RXA_ANA();
            }

            /*****************包头包尾分别为BB EE：配置模式返回帧**********************/
            if( BLE_RX_NUM_B == 0 && ch =='\xBB' )
            {
                BLE_RX_Buffer_B[BLE_RX_NUM_B]=ch;
                BLE_RX_NUM_B++;
            }

            else if( BLE_RX_NUM_B > 0 && BLE_RX_NUM_B < MAX_BUFF_LEN-1)
            {
                BLE_RX_Buffer_B[BLE_RX_NUM_B]=ch;
                BLE_RX_NUM_B++;
            }

            else if( BLE_RX_NUM_B == MAX_BUFF_LEN-1 && ch =='\xEE')
            {
                BLE_RX_Buffer_B[BLE_RX_NUM_B]=ch;
                BLE_RXB_ANA();
            }


    }
}
static int uart_thread_init(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;

    /* 查找系统中的串口设备 */
    serial = rt_device_find(UART_NAME);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", UART_NAME);
        return RT_ERROR;
    }


    /*修改串口配置参数 */
    config.baud_rate = BAUD_RATE_9600;        //修改波特率为 9600
    config.data_bits = DATA_BITS_8;           //数据位 8
    config.stop_bits = STOP_BITS_1;           //停止位 1
    config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    config.parity    = PARITY_NONE;           //无奇偶校验位

    /*控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial_ble", serial_ble_thread_entry, RT_NULL, UART_BLE_THREAD_SIZE, UART_BLE_THREAD_PAR, UART_BLE_THREAD_TACK);
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

INIT_APP_EXPORT(uart_thread_init);
