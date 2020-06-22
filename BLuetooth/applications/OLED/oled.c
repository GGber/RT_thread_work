/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-20     35166       the first version
 */
#include "oled.h"


int Sign_number=0;
int Sign_time_run=0;
int Sign_time_set=100;      //默认签到时间100s
int Sign_sta=0;


u8g2_t u8g2;
void OLED_Init(void)
{
    // Initialization
    u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_rt_gpio_and_delay);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, OLED_I2C_PIN_SCL);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, OLED_I2C_PIN_SDA);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
}
/*
 * 函数名：OLED_Show_str()
 *功能：OLED显示函数
 **/
void OLED_Show_str()
{
    static int Show_time=0;
    char Oled_str[20];
    u8g2_ClearBuffer(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_ncenB10_tr);
    u8g2_DrawStr(&u8g2, 10, 12, "Sign in system");   //title

    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    sprintf(Oled_str, "Number : %d",Sign_number);  //number
    u8g2_DrawStr(&u8g2, 1, 30, Oled_str);

    sprintf(Oled_str, "Time : %d",Sign_time_run);  //Time
    u8g2_DrawStr(&u8g2, 1, 45, Oled_str);


    switch(Sign_sta)
    {

        case SIGN_STA_SHOW_CLOSE: break;
        case SIGN_STA_SIGN_START: u8g2_DrawStr(&u8g2, 70, 50, "START"); break;
        case SIGN_STA_SET_OK:     u8g2_DrawStr(&u8g2, 70, 50, "S_OK");  break;
        case SIGN_STA_SET_ERROR:  u8g2_DrawStr(&u8g2, 70, 50, "S_ERR"); break;

        default:break;
    }

    if(Sign_sta)
    {
        Show_time++;
        if(Show_time>1)
        {
            Show_time=0;
            Sign_sta=SIGN_STA_SHOW_CLOSE;
        }
    }


    u8g2_SendBuffer(&u8g2);
}

