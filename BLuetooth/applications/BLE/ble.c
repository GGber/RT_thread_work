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

struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* ��ʼ�����ò��� */

/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* ���ڷ�����Ϣ������ */
unsigned char BLE_TX_Buffer[MAX_BUFF_LEN]={0xAA,0x11,0x22,0xEE}; //��ͨ����ģʽ����������
char *BLE_AT_CLOSE = "AT+DIS?\r\n";                              //AT�ж�ָ��

/* ���ڽ�����Ϣ������ */
unsigned char BLE_RX_Buffer_A[MAX_BUFF_LEN];
unsigned int BLE_RX_NUM_A=0;

unsigned char BLE_RX_Buffer_B[MAX_BUFF_LEN];
unsigned int BLE_RX_NUM_B=0;

/* ���ڶ�ʱǩ�� */
static rt_timer_t Timer_sign;       //1s�ж�һ��


/*
 * ��������Serial_Buffer_clear()
 *���ܣ���մ��ڽ�������
 **/
static void Serial_Buffer_clear()
{
    BLE_RX_NUM_A=0;
    BLE_RX_NUM_B=0;
}

/*
 * ��������BLE_Send_AT()
 *���ܣ�BLE����ATָ��
 *������str:ָ��,size:�������
 **/
void BLE_Send_AT(char *str,int size)
{
    rt_pin_write(BLE_PIN_KEY, PIN_LOW);//����ATָ��
    rt_thread_mdelay(1);
    rt_device_write(serial, 0,str,size);
}

/*
 * ��������BLE_Send_NOAT()
 *���ܣ�BLE����͸������
 *������str:����,size:�������
 *��ע��Ĭ��ΪATģʽ��ֻ�����գ����Է�ֹ�������⡣
 **/
void BLE_Send_NOAT(unsigned char *str,int size)
{
    rt_pin_write(BLE_PIN_KEY, PIN_HIGH);//����͸��
    rt_thread_mdelay(1);
    rt_device_write(serial, 0,str,size);
}

/*
 * ��������KEY_ENT_Function
 *���ܣ������жϷ�����
 *���������ڻ���BLE����ǩ��
 * */
void KEY_ENT_Function()
{
    Beep_sta=BEEP_SOUND_ONE;
    Sign_sta=SIGN_STA_SIGN_START;         //����ǩ��
    Sign_number=0;                        //��ʼ��ǩ������
    Sign_time_run=Sign_time_set;          //��ʼ��ǩ������ʱ��
    rt_pin_write(BLE_PIN_REST, PIN_HIGH); //����BLEģ��
    rt_timer_start(Timer_sign);           //����ǩ����ʱ��
}

/*
 * ��������BLE_CONN_Function()
 *���ܣ�BLE���ӳɹ�
 *�������ֻ�������֮������BLE״̬�����ߣ������жϺ����ֻ����͹����룺0x11��0x22
 **/
void BLE_CONN_Function()
{
    Serial_Buffer_clear();
//    LOG_I("connect success!");
    BLE_Send_NOAT(BLE_TX_Buffer, MAX_BUFF_LEN);
}

/*
 * ��������BLE_RXB_ANA()
 *���ܣ����������ַ�������
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
 * ��������BLE_RXA_ANA()
 *���ܣ����������ַ�������
 *�������ɹ����յ��ֻ����ص���Ϣ֮֡�󣬷��ͶϿ���ATָ��
 **/
static void BLE_RXA_ANA()
{
    Serial_Buffer_clear();
    BLE_Send_AT(BLE_AT_CLOSE, 9);
}

/*
 * ��������BLE_RST()
 *���ܣ�BLEӲ����λһ��(5ms���ϵĵ͵�ƽ)
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
 * ��������BLE_PIN_Init()
 *���ܣ�BLE���ų�ʼ��
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

    /* ʹ���ж� */
    rt_pin_irq_enable(BLE_PIN_STA, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY_PIN, PIN_IRQ_ENABLE);

}

/*��ʱ����ʱ����*/
static void Sign_time_out()
{
    if(Sign_time_run>0)
        Sign_time_run--;
    else {
        Sign_time_run=0;
        rt_pin_write(BLE_PIN_REST, PIN_LOW);    //�ر�BLEģ��
        rt_timer_stop(Timer_sign);              //ǩ��������ֹͣ��ʱ��
    }
}
/*
 * ��������BLE_Time_Init()
 *���ܣ�������ʱ������ǩ��ʱ������
 **/
void BLE_Time_Init()
{
    Timer_sign = rt_timer_create("Timer_sign", Sign_time_out,RT_NULL, 1000,RT_TIMER_FLAG_PERIODIC);
}

/* �������ݻص����� */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
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
            /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
            while (rt_device_read(serial, -1, &ch, 1) != 1)
            {
                /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
                rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
            }

//            LOG_I("%x",ch);

            /*******************��ͷ��β�ֱ�ΪAA EE��ǩ��ģʽ����֡******************/
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
                Sign_number++;                          //ǩ����������һ��
                BLE_RXA_ANA();
            }

            /*****************��ͷ��β�ֱ�ΪBB EE������ģʽ����֡**********************/
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

    /* ����ϵͳ�еĴ����豸 */
    serial = rt_device_find(UART_NAME);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", UART_NAME);
        return RT_ERROR;
    }


    /*�޸Ĵ������ò��� */
    config.baud_rate = BAUD_RATE_9600;        //�޸Ĳ�����Ϊ 9600
    config.data_bits = DATA_BITS_8;           //����λ 8
    config.stop_bits = STOP_BITS_1;           //ֹͣλ 1
    config.bufsz     = 128;                   //�޸Ļ����� buff size Ϊ 128
    config.parity    = PARITY_NONE;           //����żУ��λ

    /*���ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial, uart_input);

    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial_ble", serial_ble_thread_entry, RT_NULL, UART_BLE_THREAD_SIZE, UART_BLE_THREAD_PAR, UART_BLE_THREAD_TACK);
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

INIT_APP_EXPORT(uart_thread_init);
