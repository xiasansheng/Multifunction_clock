/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-08     dell       the first version
 */

#include "main.h"
#include <rtdbg.h>


void thread_entry_my_time(void *param)
{
    time_t now ;
    char *str = RT_NULL;
    struct mesg_time * mes;

    mes = (struct mesg_time*)rt_malloc(sizeof(struct mesg_time));


    time_t cur_time = ntp_get_time(RT_NULL);
    rt_device_control(rt_device_find("rtc"), RT_DEVICE_CTRL_RTC_SET_TIME, &cur_time);
    rt_kprintf("Get local time from NTP server: %s", ctime((const time_t *) &cur_time));

    rt_kprintf("date start\n");
    while(1)
    {
        now = time(RT_NULL);
//        time(&now);
//        rt_kprintf("local time: %.*s", 25, ctime(&now));
        str = ctime(&now);
//        struct timeval tv;
//        gettimeofday(&tv, NULL);            //微秒
//        struct timespec ts;
//        clock_gettime(CLOCK_REALTIME, &ts); //纳秒
//        p = localtime(&timep);
        memcpy(mes->hour, str+11, 8);
        mes->hour[8] = '\0';
        memcpy(mes->year, str+20,4);
        mes->year[4] = '\0';
        memcpy(mes->week, str, 3);
        mes->week[3] = '\0';
        memcpy(mes->mouth, str+4, 6);
        mes->mouth[6] = '\0';
//        rt_kprintf("%s\n",str);
//        rt_kprintf("time = %s,year = %s,week = %s,mouth = %s\n",mes->hour, mes->year,mes->week,mes->mouth);
        rt_mb_send(&mb_oled_time, (rt_uint32_t)mes);

//        rt_kprintf("send ok\n");rty

        rt_thread_mdelay(500);

    }

}
