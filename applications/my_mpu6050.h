/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-27     dell       the first version
 */
#ifndef APPLICATIONS_MY_MPU6050_H_
#define APPLICATIONS_MY_MPU6050_H_
#include <rtthread.h>

static char thread_stack_mpu[2048];
static struct rt_thread thread_mpu;

void thread_entry_mpu();

rt_uint8_t MPU_Write_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *databuf);
rt_uint8_t MPU_Read_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf);

#endif /* APPLICATIONS_MY_MPU6050_H_ */
