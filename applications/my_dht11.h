/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-06     dell       the first version
 */
#ifndef APPLICATIONS_MY_DHT11_H_
#define APPLICATIONS_MY_DHT11_H_

static char thread_stack_dht[2048];
static struct rt_thread thread_dht;

struct mesg_dht
{
    char temp[12];
    char humi[12];
};

void read_entry_dht(void *parameter);

#endif /* APPLICATIONS_MY_DHT11_H_ */
