/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-27     dell       the first version
 */

#include "mpu6xxx.h"
#include "my_mpu6050.h"
#include "main.h"

#define DBG_TAG "Mpu6050"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define Page_Max 2

#include "sensor_inven_mpu6xxx.h"
/* Default configuration, please change according to the actual situation, support i2c and spi device name */
#define MPU6XXX_DEVICE_NAME  "i2c1"
struct mpu6xxx_device *dev;
float Pitch,Roll,Yaw;


/* Test function */
void thread_entry_mpu()
{
    struct mpu6xxx_3axes accel, gyro;
    float pitch,roll,yaw;
    char str[50];
    int i;
    int page = 0;
    int page_buff = 1;
    /* Initialize mpu6xxx, The parameter is RT_NULL, means auto probing for i2c*/
    dev = mpu6xxx_init(MPU6XXX_DEVICE_NAME, RT_NULL);

    if (dev == RT_NULL)
    {
        rt_kprintf("mpu6xxx init failed\n");
        return -1;
    }

    rt_kprintf("mpu6xxx init succeed\n");


    while(mpu_dmp_init())
   {
        rt_thread_mdelay(500);
        rt_kprintf("\r\nMPU6050 Error\r\n");
   }
   rt_kprintf("\r\nMPU6050 OK\r\n");

    while(1){
        mpu_dmp_get_data(&Pitch,&Roll,&Yaw);
        pitch = Pitch;
        roll = Roll;
        yaw = Yaw;

        if(pitch < -40 && page_buff == 1){
            page++;
            if(page > Page_Max){
                page = 0;
            }
            page_buff = 0;
            rt_mb_send(&mb_oled_mpu, (rt_uint32_t)page);
        }
        else if(pitch > 40 && page_buff == 1){
            page--;
            if(page < 0){
                page = Page_Max;
            }
            page_buff = 0;
            rt_mb_send(&mb_oled_mpu, (rt_uint32_t)page);
        }
        else if (pitch > -30 && pitch < 30){
            page_buff = 1;
        }
        rt_thread_mdelay(10);
    }
    mpu6xxx_deinit(dev);

    return 0;
}



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

    if (dev->bus->type == RT_Device_Class_I2CBUS)
    {
        msgs.addr  = dev->i2c_addr;    /* slave address */
        msgs.flags = RT_I2C_WR;        /* write flag */
        msgs.buf   = buf;              /* Send data pointer */
        msgs.len   = len+1;

        if (rt_i2c_transfer((struct rt_i2c_bus_device *)dev->bus, &msgs, 1) == 1)
        {
            res = RT_EOK;
        }
        else
        {
            res = -RT_ERROR;
        }
    }
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
    if (dev->bus->type == RT_Device_Class_I2CBUS)
    {
        msgs[0].addr  = dev->i2c_addr;    /* Slave address */
        msgs[0].flags = RT_I2C_WR;        /* Write flag */
        msgs[0].buf   = &reg;             /* Slave register address */
        msgs[0].len   = 1;                /* Number of bytes sent */

        msgs[1].addr  = dev->i2c_addr;    /* Slave address */
        msgs[1].flags = RT_I2C_RD;        /* Read flag */
        msgs[1].buf   = buf;              /* Read data pointer */
        msgs[1].len   = len;              /* Number of bytes read */

        if (rt_i2c_transfer((struct rt_i2c_bus_device *)dev->bus, msgs, 2) == 2)
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




