/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-08     dell       the first version
 */
#ifndef APPLICATIONS_WEATHER_H_
#define APPLICATIONS_WEATHER_H_

static char thread_stack_weather[2048];
static struct rt_thread thread_wea;


void thread_entry_weather(void *param);

#endif /* APPLICATIONS_WEATHER_H_ */
