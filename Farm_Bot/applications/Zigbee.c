#include <rtthread.h>
#include "board.h"
#include <rtdevice.h>

#include "user_mb_app.h"

#define DBG_TAG "Zigbee"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define Zigbee_THREAD_PRIORITY          12          //优先级
#define Zigbee_UART_NAME                "uart2"     //名字

static struct rt_semaphore rx_sem;
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
static rt_device_t Zigbee_serial;

extern unsigned char addr; //定义地址
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];//数据


/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}


static void Zigbee_start(void *parameter)
{
    char Send_str[8];
    Send_str[0]='\xFC';
    Send_str[1]='\x06';//数据长度
    Send_str[2]='\x02';//主播模式
    Send_str[3]='\xFE';//目标组号
    Send_str[4]='\xAA';//数据内容 包头   （包尾0xC9,Zigbee自带）
    while(1)
    {
        Send_str[5]=addr&0xFF;//当前地址
        Send_str[6]=(usSRegHoldBuf[10]>>8)&0xFF;
        Send_str[7]=usSRegHoldBuf[10]&0xFF;//数据值

        if(rt_pin_read(ADDR8))
        {
            rt_device_write(Zigbee_serial,0,Send_str, 8);
            LOG_I("Go into Zigbee\n");
        }


        rt_thread_mdelay(1000);
    }
}

int Zigbee_Usart2_Init(void)
{
    static rt_uint8_t is_init = 0;
    rt_thread_t tid1 = RT_NULL;

    if (is_init > 0)
    {
        rt_kprintf("Zigbee is running\n");
        return -RT_ERROR;
    }
    Zigbee_serial=rt_device_find(Zigbee_UART_NAME);
    if(!Zigbee_serial)
    {
        rt_kprintf("Zigbee fail\n");
        return RT_ERROR;
    }
    else
        rt_kprintf("Zigbee usart service success\n");

    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);//初始化信号量

    rt_device_control(Zigbee_serial, RT_DEVICE_CTRL_CONFIG, &config);

    rt_device_open(Zigbee_serial,RT_DEVICE_FLAG_INT_RX);

//    rt_device_set_rx_indicate(Zigbee_serial, uart_input);//回调函数

    tid1 = rt_thread_create("Zigbee_start", Zigbee_start, RT_NULL, 512, Zigbee_THREAD_PRIORITY, 10);
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
//MSH_CMD_EXPORT(mb_slave_samlpe, run a modbus slave sample);
INIT_APP_EXPORT(Zigbee_Usart2_Init);
