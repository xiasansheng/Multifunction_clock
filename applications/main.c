/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-06     RT-Thread    first version
 */



#include "main.h"
#include "my_mpu6050.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int main(void)
{
    rt_thread_init(&thread_wea,
                    "name_wea",
                    thread_entry_weather,
                    RT_NULL,
                    &thread_stack_weather[0],
                    sizeof(thread_stack_weather),
                    30,
                    5);
        rt_thread_startup(&thread_wea);
    rt_thread_init(&thread_time,
                    "name_time",
                    thread_entry_my_time,
                    RT_NULL,
                    &thread_stack_time[0],
                    sizeof(thread_stack_time),
                    22,
                    20);
        rt_thread_startup(&thread_time);
    rt_thread_init(&thread_oled,
                    "name_oled",
                    thread_entry_oled,
                    RT_NULL,
                    &thread_stack_oled[0],
                    sizeof(thread_stack_oled),
                    28,
                    20);
        rt_thread_startup(&thread_oled);
    rt_thread_init(&thread_mpu,
                    "name_mpu6050",
                    thread_entry_mpu,
                    RT_NULL,
                    &thread_stack_mpu[0],
                    sizeof(thread_stack_mpu),
                    27,
                    5);
        rt_thread_startup(&thread_mpu);
    rt_thread_init(&thread_dht,
                        "name_dht11",
                        read_entry_dht,
                        RT_NULL,
                        &thread_stack_dht[0],
                        sizeof(thread_stack_dht),
                        24,
                        5);
            rt_thread_startup(&thread_dht);

    rt_err_t result;

    /* 天气的 mailbox */
    result = rt_mb_init(&mb_oled_weather, "mb_weather",
           &mb_pool_oled_weather[0], /* 邮箱用到的内存池是 mb_pool */
           sizeof(mb_pool_oled_weather) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
           RT_IPC_FLAG_FIFO); /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
       rt_kprintf("init mailbox failed.\n");
       return -1;
    }
    /* 时间的 mailbox */
    result = rt_mb_init(&mb_oled_time, "mb_time",
                 &mb_pool_oled_time[0], /* 邮箱用到的内存池是 mb_pool */
                 sizeof(mb_pool_oled_time) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
                 RT_IPC_FLAG_FIFO); /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
     rt_kprintf("init mailbox failed.\n");
     return -1;
    }
    /* MPU6050的 mailbox */
    result = rt_mb_init(&mb_oled_mpu, "mb_time",
                 &mb_pool_oled_mpu[0], /* 邮箱用到的内存池是 mb_pool */
                 sizeof(mb_pool_oled_mpu) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
                 RT_IPC_FLAG_FIFO); /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
     rt_kprintf("init mailbox failed.\n");
     return -1;
    }

    /* 温度的的 mailbox */
        result = rt_mb_init(&mb_oled_dht, "mb_dht11",
                &mb_pool_oled_dht[0], /* 邮箱用到的内存池是 mb_pool */
                sizeof(mb_pool_oled_dht) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
                RT_IPC_FLAG_FIFO); /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
       rt_kprintf("init mailbox failed.\n");
       return -1;
    }


    while (1)
    {
        //LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
