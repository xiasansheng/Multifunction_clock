/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-06     dell       the first version
 */
#include "main.h"

/* Modify this pin according to the actual wiring situation */
#define DHT11_DATA_PIN    GET_PIN(E, 15)

void read_entry_dht(void *parameter)
{
    struct mesg_dht * msg;
    msg = (struct mesg_dht*)rt_malloc(sizeof(struct mesg_dht));
    rt_hw_dht11_port();
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;
    rt_uint8_t get_data_freq = 1; /* 1Hz */

    dev = rt_device_find("temp_dht11");
    if (dev == RT_NULL)
    {
        return;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!\n");
        return;
    }

    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)(&get_data_freq));

    while (1)
    {
        res = rt_device_read(dev, 0, &sensor_data, 1);

        if (res != 1)
        {
            rt_kprintf("read data failed! result is %d\n", res);
            rt_device_close(dev);
            return;
        }
        else
        {
            if (sensor_data.data.temp >= 0)
            {
                int strTemp;   //定义数组
                int strHumi;
                strTemp = (sensor_data.data.temp & 0xffff) >> 0;      // get temp
                strHumi = (sensor_data.data.temp & 0xffff0000) >> 16; // get humi
                sprintf(msg->temp, "Temp: %d C", strTemp);
                sprintf(msg->humi, "Humi: %d %%", strHumi);
                rt_mb_send(&mb_oled_dht, (rt_uint32_t)msg);
//                rt_kprintf("temp:%d, humi:%d\n" ,strTemp, strHumi);
            }
        }

        rt_thread_mdelay(100);
    }
}


int rt_hw_dht11_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.user_data = (void *)DHT11_DATA_PIN;
    rt_hw_dht11_init("dht11", &cfg);

    return RT_EOK;
}

