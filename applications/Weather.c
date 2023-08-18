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
#include "webclient.h"



#define GET_HEADER_BUFSZ               1024
#define GET_RESP_BUFSZ                 1024

#define GET_LOCAL_URI                  "http://api.seniverse.com/v3/weather/now.json?key=S21jPVZPvEVcB69Es&location=nanyang&language=en&unit=c"

//#define GET_LOCAL_URI2                  "http://quan.suning.com/getSysTime.do"

static int cJSON_NowWeatherParse(char *JSON)
{
    cJSON *json,*arrayItem,*object,*subobject,*item;
    json = cJSON_Parse(JSON);//解析JSON数据包
    struct mesg_weather *mes = NULL;
    mes = (struct mesg_weather*)rt_malloc(sizeof(struct mesg_weather));
    if(json == NULL)          //检测JSON数据包是否存在语法上的错误，返回NULL表示数据包无效
    {
        rt_kprintf("Error before: [%s]\r\n",cJSON_GetErrorPtr()); //打印数据包语法错误的位置
        return 1;
    }
    else
    {
        if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL); //匹配字符串"results",获取数组内容
        {
            int size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数
            rt_kprintf("cJSON_GetArraySize: size=%d\n",size);

            if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//获取父对象内容
            {
                rt_kprintf("--------------------------------Now Weather Data Begin------------------------------\r\n");
                /* 匹配子对象1 */
                if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
                {
//                    rt_kprintf("---------------------------------subobject1-------------------------------\r\n");
                    if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)
                    {
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                    }
                    if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //地名--需要用到的数据
                    {
//                        strcpy(mes->name, item->valuestring);
                        sprintf(mes->name,"%s",item->valuestring);
                        rt_kprintf("name: %s\r\n",mes->name);
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                    }
                    if((item = cJSON_GetObjectItem(subobject,"country")) != NULL)
                    {
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                    }
                    if((item = cJSON_GetObjectItem(subobject,"path")) != NULL)
                    {
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                    }
                    if((item = cJSON_GetObjectItem(subobject,"timezone")) != NULL)
                    {
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                    }
                    if((item = cJSON_GetObjectItem(subobject,"timezone_offset")) != NULL)
                    {
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                    }
                }
                /* 匹配子对象2 */
                if((subobject = cJSON_GetObjectItem(object,"now")) != NULL)
                {
//                    rt_kprintf("---------------------------------subobject2-------------------------------\r\n");
                    if((item = cJSON_GetObjectItem(subobject,"text")) != NULL)//天气预报文字--需要用到的数据
                    {
//                        strcpy(mes->weather, item->valuestring);
                        sprintf(mes->weather,"%s",item->valuestring);
                        rt_kprintf("weather: %s\r\n",mes->weather);
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                    }
                    if((item = cJSON_GetObjectItem(subobject,"code")) != NULL)//天气预报代码--需要用到的数据
                    {
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string, item->valuestring);

                    }
                    if((item = cJSON_GetObjectItem(subobject,"temperature")) != NULL) //温度--需要用到的数据
                    {
//                        strcpy(mes->temp, item->valuestring);
                        sprintf(mes->temp,"%s",item->valuestring);
                        rt_kprintf("temperature: %s\r\n",mes->temp);
//                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string, item->valuestring);
                    }
                }
                /* 匹配子对象3 */
                if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
                {
//                    rt_kprintf("---------------------------------subobject3-------------------------------\r\n");
//                    rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
                }
                rt_kprintf("--------------------------------Now Weather Data End--------------------------------\r\n");
            }
        }
    }
    rt_mb_send(&mb_oled_weather, (rt_uint32_t)mes);
    rt_free(mes);
    mes = NULL;
    cJSON_Delete(json); //释放cJSON_Parse()分配出来的内存空间
    return 0;
}


/* send HTTP GET request by common request interface, it used to receive longer data */
static int weather_get_comm(const char *uri)
{
    struct webclient_session* session = RT_NULL;
    unsigned char *buffer = RT_NULL;
    int index, ret = 0;
    int bytes_read, resp_status;
    int content_length = -1;

    buffer = (unsigned char *) web_malloc(GET_RESP_BUFSZ);
    if (buffer == RT_NULL)
    {
        rt_kprintf("no memory for receive buffer.\n");
        ret = -RT_ENOMEM;
        goto __exit;

    }

    /* create webclient session and set header response size */
    session = webclient_session_create(GET_HEADER_BUFSZ);
    if (session == RT_NULL)
    {
        ret = -RT_ENOMEM;
        goto __exit;
    }

    /* send GET request by default header */
    if ((resp_status = webclient_get(session, uri)) != 200)
    {
        rt_kprintf("webclient GET request failed, response(%d) error.\n", resp_status);
        ret = -RT_ERROR;
        goto __exit;
    }

    rt_kprintf("webclient get response data: \n");

    content_length = webclient_content_length_get(session);
    if (content_length < 0)
    {
        rt_kprintf("webclient GET request type is chunked.\n");
        do
        {
            bytes_read = webclient_read(session, (void *)buffer, GET_RESP_BUFSZ);
            if (bytes_read <= 0)
            {
                break;
            }

            for (index = 0; index < bytes_read; index++)
            {
                rt_kprintf("%c", buffer[index]);
            }
        } while (1);

        rt_kprintf("\n");
    }
    else
    {
        int content_pos = 0;

        do
        {
            bytes_read = webclient_read(session, (void *)buffer,
                    content_length - content_pos > GET_RESP_BUFSZ ?
                            GET_RESP_BUFSZ : content_length - content_pos);
            cJSON_NowWeatherParse(buffer);
            if (bytes_read <= 0)
            {
                break;
            }
//
//            for (index = 0; index < bytes_read; index++)
//            {
//                rt_kprintf("%c", buffer[index]);
//            }

            content_pos += bytes_read;
        } while (content_pos < content_length);

        rt_kprintf("\n");
    }

__exit:
    if (session)
    {
        webclient_close(session);
    }

    if (buffer)
    {
        web_free(buffer);
    }

    return ret;
}

void thread_entry_weather(void *param)
{
    char *uri = RT_NULL;
    uri = web_strdup(GET_LOCAL_URI);
    if(uri == RT_NULL)
    {
        rt_kprintf("no memory for create get request uri buffer.\n");
        return ;
    }
    weather_get_comm(uri);
    if (uri)
    {
        web_free(uri);
    }
}

