#include <rtthread.h>
#include "board.h"
#include "mb.h"
#include "user_mb_app.h"
#include <rtdevice.h>

#define DBG_TAG "slave"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define MB_POLL_THREAD_PRIORITY  10
#define MB_SEND_THREAD_PRIORITY  RT_THREAD_PRIORITY_MAX - 1

#define MB_POLL_CYCLE_MS 200

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

#define MODE_MANUAL     1
#define MODE_AUTO       2

#define READ_CH_CHECKED 0
#define ADDR_CHECKED    1

#define MOS_CH_ON       1   //输出有效电平
#define MOS_CH_OFF      0
/*
 * 读输入通道
 * */
static rt_uint32_t read_in_value(void)
{
    rt_uint32_t ret = 0;
    if (rt_pin_read(DIN1) == READ_CH_CHECKED)
        ret |= 1 << 0;
    if (rt_pin_read(DIN2) == READ_CH_CHECKED)
        ret |= 1 << 1;
    if (rt_pin_read(DIN3) == READ_CH_CHECKED)
        ret |= 1 << 2;
    if (rt_pin_read(DIN4) == READ_CH_CHECKED)
        ret |= 1 << 3;
    if (rt_pin_read(DIN5) == READ_CH_CHECKED)
        ret |= 1 << 4;
    if (rt_pin_read(DIN6) == READ_CH_CHECKED)
        ret |= 1 << 5;
    if (rt_pin_read(DIN7) == READ_CH_CHECKED)
        ret |= 1 << 6;
    if (rt_pin_read(DIN8) == READ_CH_CHECKED)
        ret |= 1 << 7;

    if (rt_pin_read(FLOW1) == READ_CH_CHECKED)
        ret |= 1 << 8;
    if (rt_pin_read(FLOW2) == READ_CH_CHECKED)
        ret |= 1 << 9;
    if (rt_pin_read(FLOW3) == READ_CH_CHECKED)
        ret |= 1 << 10;
    if (rt_pin_read(FLOW4) == READ_CH_CHECKED)
        ret |= 1 << 11;
    if (rt_pin_read(FLOW5) == READ_CH_CHECKED)
        ret |= 1 << 12;
    if (rt_pin_read(FLOW6) == READ_CH_CHECKED)
        ret |= 1 << 13;
    if (rt_pin_read(FLOW7) == READ_CH_CHECKED)
        ret |= 1 << 14;
    if (rt_pin_read(FLOW8) == READ_CH_CHECKED)
        ret |= 1 << 15;
    return ret;
}

/*
 * 读拨码开关值
 * */
static rt_uint8_t read_addr_value(void)
{
    rt_uint32_t ret = 0;
    if (rt_pin_read(ADDR1) == ADDR_CHECKED)
        ret |= 1 << 0;
    if (rt_pin_read(ADDR2) == ADDR_CHECKED)
        ret |= 1 << 1;
    if (rt_pin_read(ADDR3) == ADDR_CHECKED)
        ret |= 1 << 2;
    if (rt_pin_read(ADDR4) == ADDR_CHECKED)
        ret |= 1 << 3;
    if (rt_pin_read(ADDR5) == ADDR_CHECKED)
        ret |= 1 << 4;
    if (rt_pin_read(ADDR6) == ADDR_CHECKED)
        ret |= 1 << 5;
    if (rt_pin_read(ADDR7) == ADDR_CHECKED)
        ret |= 1 << 6;
    if (rt_pin_read(ADDR8) == ADDR_CHECKED)
        ret |= 1 << 7;
    return ret;
}

/*
 * 读mos值
 * */
static rt_uint32_t read_mos_value(void)
{
    rt_uint32_t ret = 0;
    if (rt_pin_read(MOS1) == MOS_CH_ON)
        ret |= 1 << 0;
    if (rt_pin_read(MOS2) == MOS_CH_ON)
        ret |= 1 << 1;
    if (rt_pin_read(MOS3) == MOS_CH_ON)
        ret |= 1 << 2;
    if (rt_pin_read(MOS4) == MOS_CH_ON)
        ret |= 1 << 3;
    if (rt_pin_read(MOS5) == MOS_CH_ON)
        ret |= 1 << 4;
    if (rt_pin_read(MOS6) == MOS_CH_ON)
        ret |= 1 << 5;
    if (rt_pin_read(MOS7) == MOS_CH_ON)
        ret |= 1 << 6;
    if (rt_pin_read(MOS8) == MOS_CH_ON)
        ret |= 1 << 7;
    return ret;
}

/*
 * 写mos值
 * */
static void write_mos_value(rt_uint32_t value)
{
    rt_pin_write(MOS1, value & (1 << 0) ? MOS_CH_ON : MOS_CH_OFF);
    rt_pin_write(MOS2, value & (1 << 1) ? MOS_CH_ON : MOS_CH_OFF);
    rt_pin_write(MOS3, value & (1 << 2) ? MOS_CH_ON : MOS_CH_OFF);
    rt_pin_write(MOS4, value & (1 << 3) ? MOS_CH_ON : MOS_CH_OFF);
    rt_pin_write(MOS5, value & (1 << 4) ? MOS_CH_ON : MOS_CH_OFF);
    rt_pin_write(MOS6, value & (1 << 5) ? MOS_CH_ON : MOS_CH_OFF);
    rt_pin_write(MOS7, value & (1 << 6) ? MOS_CH_ON : MOS_CH_OFF);
    rt_pin_write(MOS8, value & (1 << 7) ? MOS_CH_ON : MOS_CH_OFF);

}

static void send_thread_entry(void *parameter)
{
    USHORT *usRegHoldingBuf;
    usRegHoldingBuf = usSRegHoldBuf;

    rt_uint32_t minvalue[10];                                                   //记录最小值
    rt_uint32_t invalue;                                                        //輸入通道值
    rt_uint32_t addrvalue;                                                      //拨码值
    rt_uint32_t mosvalue;
    rt_uint32_t resetvalue;
    rt_uint32_t slave_thread_cycle = 0;
    LOG_I("send_thread_entry begin.");
    while (1)
    {
        /* Test Modbus Master */
        invalue = read_in_value();
        addrvalue = read_addr_value();
        mosvalue = read_mos_value();

        LOG_I("ADDR:%02x\n",addrvalue);
        rt_enter_critical();
        /*write*/
        if (usSRegHoldBuf[80] == MODE_AUTO)                                         //自动模式
        {
            for (int i = 1; i <= 8; i++)
            {
                minvalue[i] = 0xF000;                                               //如果当前排的所有采集点都不在线，湿度为0xF000，不加水
                for (int j = 1; j <= 8; j++)
                {
                    rt_uint32_t curvalue = usSRegHoldBuf[(i - 1) * 8 + j];          //获取当前值
                    if ((curvalue & 0xF000) == 0xF000)                              //通道不在线就跳过
                        continue;
                    minvalue[i] = curvalue < minvalue[i] ? curvalue : minvalue[i];  //比较最小值
                }

                if (minvalue[i] < usRegHoldingBuf[80 + i] * 0.9)        //湿度不足
                {
                    mosvalue |= 1 << (i - 1);                           //打开MOS
                }
                else if (minvalue[i] > usRegHoldingBuf[80 + i] * 1.1)   //湿度够了
                {
                    mosvalue &= ~(1 << (i - 1));                        //关闭MOS
                }
            }
        }
        else if (usSRegHoldBuf[80] == MODE_MANUAL)                                  //手动模式，mos输出等于上位机发的寄存器的值
        {
            mosvalue = usRegHoldingBuf[89];
        }
        else                                                                        //其他模式mos不动作
        {
            mosvalue = 0;
        }
        usRegHoldingBuf[89] = mosvalue;
        usRegHoldingBuf[90] = invalue;
        usRegHoldingBuf[91] = addrvalue;
        /*read*/
        resetvalue = usSRegHoldBuf[99];
        rt_exit_critical();
        if (resetvalue == 0x5555)   //本机复位
            rt_hw_cpu_reset();
        write_mos_value(mosvalue);
        LOG_I("run times:%d", ++slave_thread_cycle);
        rt_thread_mdelay(500);
    }
}

static void mb_slave_poll(void *parameter)
{
    //通过拨码开关设置地址
    unsigned char addr = read_addr_value();
    addr=addr&0x7f;
    LOG_I("slave address:%d", addr);
    eMBInit(MB_RTU, addr, MODBUS_SLAVE_PORT_NUM, MODBUS_SLAVE_PORT_BAUDRATE, MODBUS_SLAVE_PORT_PARITY);
    eMBEnable();
    while (1)
    {
        eMBPoll();
        rt_thread_mdelay(MB_POLL_CYCLE_MS);
    }
}

//static int mb_slave_samlpe(int argc, char **argv)
int mb_slave_samlpe(void)
{
    static rt_uint8_t is_init = 0;
    rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;

    if (is_init > 0)
    {
        rt_kprintf("sample is running\n");
        return -RT_ERROR;
    }
    tid1 = rt_thread_create("md_s_poll", mb_slave_poll, RT_NULL, 512, MB_POLL_THREAD_PRIORITY, 10);
    if (tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else
    {
        goto __exit;
    }

    tid2 = rt_thread_create("md_s_send", send_thread_entry, RT_NULL, 1024, MB_SEND_THREAD_PRIORITY, 10);
    if (tid2 != RT_NULL)
    {
        rt_thread_startup(tid2);
    }
    else
    {
        goto __exit;
    }

    is_init = 1;
    return RT_EOK;

    __exit: if (tid1)
        rt_thread_delete(tid1);
    if (tid2)
        rt_thread_delete(tid2);

    return -RT_ERROR;
}
//MSH_CMD_EXPORT(mb_slave_samlpe, run a modbus slave sample);
INIT_APP_EXPORT(mb_slave_samlpe);
