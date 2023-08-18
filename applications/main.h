/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-08     dell       the first version
 */
#ifndef APPLICATIONS_MAIN_H_
#define APPLICATIONS_MAIN_H_




#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "rtdef.h"
#include "math.h"
#include "stdio.h"
#include <rthw.h>
#include "sensor.h"
#include "sensor_dallas_dht11.h"
#include "drv_common.h"

#include "Weather.h"
#include "my_time.h"
#include "my_dht11.h"
#include "oled.h"
#include "cJSON.h"

struct rt_mailbox mb_oled_time;
char mb_pool_oled_time[128];

struct rt_mailbox mb_oled_mpu;
char mb_pool_oled_mpu[128];

struct rt_mailbox mb_oled_dht;
char mb_pool_oled_dht[128];

struct rt_mailbox mb_oled_weather;
char mb_pool_oled_weather[128];


struct mesg_weather
{
    rt_uint8_t name[16];
    rt_uint8_t weather[12];
    rt_uint8_t temp[6];

};




#endif /* APPLICATIONS_MAIN_H_ */
