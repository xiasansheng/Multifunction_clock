/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-23     flybreak     the first version
 */

#include <rtthread.h>
#include "mpu6xxx.h"

/* Default configuration, please change according to the actual situation, support i2c and spi device name */
#define MPU6XXX_DEVICE_NAME  "i2c1"
struct mpu6xxx_device *dev;


/* Test function */
static int mpu6xxx_test()
{
    struct mpu6xxx_3axes accel, gyro;
    int i;

    /* Initialize mpu6xxx, The parameter is RT_NULL, means auto probing for i2c*/
    dev = mpu6xxx_init(MPU6XXX_DEVICE_NAME, RT_NULL);

    mpu6xxx_set_param(i2c_bus, MPU6XXX_ACCEL_RANGE, MPU6XXX_GYRO_RANGE_2000DPS);  //闄�铻轰华鑼冨洿閰嶇疆
    mpu6xxx_set_param(i2c_bus, MPU6XXX_ACCEL_RANGE, MPU6XXX_ACCEL_RANGE_2G);     //鍔犻�熷害璁★紝涓�鑸缃负卤2G
    mpu6xxx_set_param(i2c_bus, MPU6XXX_SAMPLE_RATE, 50);                       //閲囨牱棰戠巼
    mpu6xxx_set_param(i2c_bus, MPU6XXX_DLPF_CONFIG, 25);                       //鏁板瓧浣庨�氭护娉㈠櫒璁剧疆锛屼竴鑸负1/2閲囨牱鐜�


    if (dev == RT_NULL)
    {
        rt_kprintf("mpu6xxx init failed\n");
        return -1;
    }
    rt_kprintf("mpu6xxx init succeed\n");

    for (i = 0; i < 5; i++)
    {
        mpu6xxx_get_accel(dev, &accel);
        mpu6xxx_get_gyro(dev, &gyro);

        rt_kprintf("accel.x = %3d, accel.y = %3d, accel.z = %3d ", accel.x, accel.y, accel.z);
        rt_kprintf("gyro.x = %3d gyro.y = %3d, gyro.z = %3d\n", gyro.x, gyro.y, gyro.z);

        rt_thread_mdelay(100);
    }

    mpu6xxx_deinit(dev);

    return 0;
}
MSH_CMD_EXPORT(mpu6xxx_test, mpu6xxx sensor test function);


rt_uint8_t MPU_Write_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *databuf)
{
    rt_int8_t res = 0;
#ifdef RT_USING_I2C
    struct rt_i2c_msg msgs;
    rt_uint8_t buf[50] = {0};
#endif
    buf[0] = reg;

    for(int i = 0;i<len;i++)
    {
        buf[i+1]=databuf[i];
    }

    if (i2c_bus->bus->type == RT_Device_Class_I2CBUS)
    {
        msgs.addr  = i2c_bus->i2c_addr;    /* slave address */
        msgs.flags = RT_I2C_WR;        /* write flag */
        msgs.buf   = buf;              /* Send data pointer */
        msgs.len   = len+1;

        if (rt_i2c_transfer((struct rt_i2c_bus_device *)i2c_bus->bus, &msgs, 1) == 1)
        {
            res = RT_EOK;
        }
        else
        {
            res = -RT_ERROR;
        }
    }
    return res;
}


rt_uint8_t MPU_Read_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf)
{
    rt_int8_t res = 0;
#ifdef RT_USING_I2C
    struct rt_i2c_msg msgs[2];
#endif
#ifdef RT_USING_SPI
    rt_uint8_t tmp;
#endif
    if (i2c_bus->bus->type == RT_Device_Class_I2CBUS)
    {
        msgs[0].addr  = i2c_bus->i2c_addr;    /* Slave address */
        msgs[0].flags = RT_I2C_WR;        /* Write flag */
        msgs[0].buf   = &reg;             /* Slave register address */
        msgs[0].len   = 1;                /* Number of bytes sent */

        msgs[1].addr  = i2c_bus->i2c_addr;    /* Slave address */
        msgs[1].flags = RT_I2C_RD;        /* Read flag */
        msgs[1].buf   = buf;              /* Read data pointer */
        msgs[1].len   = len;              /* Number of bytes read */

        if (rt_i2c_transfer((struct rt_i2c_bus_device *)i2c_bus->bus, msgs, 2) == 2)
        {
            res = RT_EOK;
        }
        else
        {
            res = -RT_ERROR;
        }
    }

    return res;
}


/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */

