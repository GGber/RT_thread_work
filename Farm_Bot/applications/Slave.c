#include <rtthread.h>
#include "board.h"
#include "mb.h"
#include "user_mb_app.h"
#include <rtdevice.h>

#define DBG_TAG "slave"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define MB_POLL_THREAD_PRIORITY  10

#define MB_POLL_CYCLE_MS 5

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

#define ADDR_CHECKED    1

unsigned char addr; //定义地址

/*
 * 读拨码开关值
 * */
static rt_uint8_t read_addr_value(void)
{
    rt_uint8_t ret = 0;
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

static void mb_slave_poll(void *parameter)
{
    //通过拨码开关设置地址
    addr = read_addr_value();
    LOG_I("slave address:%d", addr);
    eMBInit(MB_RTU, addr, MODBUS_SLAVE_PORT_NUM, MODBUS_SLAVE_PORT_BAUDRATE, MODBUS_SLAVE_PORT_PARITY);
    eMBEnable();

    while (1)
    {
       if(!rt_pin_read(ADDR8))
       {
           eMBPoll();
           //LOG_I("Go into Modbus\n");
       }


        rt_thread_mdelay(MB_POLL_CYCLE_MS);
    }
}

int mb_slave_samlpe(void)
{
    static rt_uint8_t is_init = 0;
    rt_thread_t tid1 = RT_NULL;

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

    is_init = 1;
    return RT_EOK;

    __exit: if (tid1)
        rt_thread_delete(tid1);


    return -RT_ERROR;
}
//MSH_CMD_EXPORT(mb_slave_samlpe, run a modbus slave sample);
INIT_APP_EXPORT(mb_slave_samlpe);
