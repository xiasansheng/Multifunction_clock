/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-10     dell       the first version
 */
#ifndef APPLICATIONS_OLED_H_
#define APPLICATIONS_OLED_H_

static char thread_stack_oled[2048];
static struct rt_thread thread_oled;

void thread_entry_oled(void *param);


#endif /* APPLICATIONS_OLED_H_ */
