/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-10     dell       the first version
 */


#include "main.h"
#include <u8g2_port.h>

#define OLED_I2C_PIN_SCL                    22  // PB6
#define OLED_I2C_PIN_SDA                    23  // PB7


void thread_entry_oled(void *param)
{
    u8g2_t u8g2;
    struct mesg_weather *str_wea;
    str_wea = (struct mesg_weather*)rt_malloc(sizeof(struct mesg_weather));
    struct mesg_time  *str_time;
    struct mesg_dht  *str_dht;
    char *mes = RT_NULL;
    int oled_sign = 0;


    // Initialization
    u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, OLED_I2C_PIN_SCL);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, OLED_I2C_PIN_SDA);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    // Draw Graphics
    /* full buffer example, setup procedure ends in _f */

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 0, 48, "Loading...");
    u8g2_SendBuffer(&u8g2);

    rt_kprintf("oled start ok\n");
    if (rt_mb_recv(&mb_oled_weather, (rt_uint32_t *) &str_wea, RT_WAITING_FOREVER) == RT_EOK)
    {
        rt_mb_detach(&mb_oled_weather);
    }
    oled_sign = 0;
    while(1)
    {
        if (rt_mb_recv(&mb_oled_mpu, (rt_uint32_t *) &mes, RT_WAITING_NO) == RT_EOK){
            rt_kprintf("mes = %d\n",mes);
            oled_sign = (int)mes;
        }
        if(oled_sign == 0){
            if (rt_mb_recv(&mb_oled_time, (rt_uint32_t *) &str_time, RT_WAITING_FOREVER) == RT_EOK)
            {
                u8g2_ClearBuffer(&u8g2);
                u8g2_ClearBuffer(&u8g2);
                u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
                u8g2_DrawStr(&u8g2, 0, 10, str_time->year);
                u8g2_DrawStr(&u8g2, 52, 10, str_time->mouth);
                u8g2_DrawStr(&u8g2, 100, 10, str_time->week);
                u8g2_SetFont(&u8g2, u8g2_font_ncenB12_tr);
                u8g2_DrawStr(&u8g2, 30, 40,str_time->hour);
                u8g2_SendBuffer(&u8g2);
                rt_thread_mdelay(1);
            }
        }
        else if(oled_sign == 1){
            if (rt_mb_recv(&mb_oled_dht, (rt_uint32_t *) &str_dht, RT_WAITING_FOREVER) == RT_EOK){
                u8g2_ClearBuffer(&u8g2);
                u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
                u8g2_DrawStr(&u8g2, 0, 10, str_dht->temp);
                u8g2_DrawStr(&u8g2, 0, 20, str_dht->humi);
                u8g2_SendBuffer(&u8g2);
                rt_thread_mdelay(1);
            }

        }
        else if(oled_sign == 2){
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
            u8g2_DrawStr(&u8g2, 0, 10, str_wea->name);
            u8g2_DrawStr(&u8g2, 0, 20, str_wea->temp);
            u8g2_DrawStr(&u8g2, 0, 30, str_wea->weather);
            u8g2_SendBuffer(&u8g2);
            rt_thread_mdelay(1);
        }
    }
}
