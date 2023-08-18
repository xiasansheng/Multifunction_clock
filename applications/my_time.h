/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-08     dell       the first version
 */
#ifndef APPLICATIONS_MY_TIME_H_
#define APPLICATIONS_MY_TIME_H_

#include "ntp.h"

static char thread_stack_time[2048];
static struct rt_thread thread_time;


struct mesg_time{
    char hour[10];
    char week[6];
    char mouth[8];
    char year[8];
};


void thread_entry_my_time(void *param);

#endif /* APPLICATIONS_MY_TIME_H_ */
