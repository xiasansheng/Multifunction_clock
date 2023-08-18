/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-03    chenyong      the first version
 */

#include <rtthread.h>
#include <webclient.h>
#include "cJSON.h"

#define GET_HEADER_BUFSZ               1024
#define GET_RESP_BUFSZ                 1024

#define GET_LOCAL_URI                  "http://api.seniverse.com/v3/weather/now.json?key=S21jPVZPvEVcB69Es&location=nanyang"


typedef struct tianqi{
    char local_name [16];
    char local_text [8];
    char local_code [8];
} tian;


static int cJSON_NowWeatherParse(char *JSON)
{
    cJSON *json,*arrayItem,*object,*subobject,*item;
    json = cJSON_Parse(JSON);//解析JSON数据包
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

                        rt_kprintf("---------------------------------subobject1-------------------------------\r\n");
                        if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                        }
                        if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //地名--需要用到的数据
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                        }
                        if((item = cJSON_GetObjectItem(subobject,"country")) != NULL)
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                        }
                        if((item = cJSON_GetObjectItem(subobject,"path")) != NULL)
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                        }
                        if((item = cJSON_GetObjectItem(subobject,"timezone")) != NULL)
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                        }
                        if((item = cJSON_GetObjectItem(subobject,"timezone_offset")) != NULL)
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
                        }
                    }
                    /* 匹配子对象2 */
                    if((subobject = cJSON_GetObjectItem(object,"now")) != NULL)
                    {
                        rt_kprintf("---------------------------------subobject2-------------------------------\r\n");
                        if((item = cJSON_GetObjectItem(subobject,"text")) != NULL)//天气预报文字--需要用到的数据
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);

                        }
                        if((item = cJSON_GetObjectItem(subobject,"code")) != NULL)//天气预报代码--需要用到的数据
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string, item->valuestring);

                        }
                        if((item = cJSON_GetObjectItem(subobject,"temperature")) != NULL) //温度--需要用到的数据
                        {
                            rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",item->string, item->valuestring);
                        }
                    }
                    /* 匹配子对象3 */
                    if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
                    {
                        rt_kprintf("---------------------------------subobject3-------------------------------\r\n");
                        rt_kprintf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
                    }
                    rt_kprintf("--------------------------------Now Weather Data End--------------------------------\r\n");
                }
            }
        }
        cJSON_Delete(json); //释放cJSON_Parse()分配出来的内存空间
        return 0;
}





/* send HTTP GET request by common request interface, it used to receive longer data */
static int webclient_get_comm(const char *uri)
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

            for (index = 0; index < bytes_read; index++)
            {
                rt_kprintf("%c", buffer[index]);
            }

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

/* send HTTP GET request by simplify request interface, it used to received shorter data */
static int webclient_get_smpl(const char *uri)
{
    char *response = RT_NULL;
    size_t resp_len = 0;
    int index;

    if (webclient_request(uri, RT_NULL, RT_NULL, 0, (void **)&response, &resp_len) < 0)
    {
        rt_kprintf("webclient send get request failed.");
        return -RT_ERROR;
    }

    rt_kprintf("webclient send get request by simplify request interface.\n");
    rt_kprintf("webclient get response data: \n");
    for (index = 0; index < rt_strlen(response); index++)
    {
        rt_kprintf("%c", response[index]);
    }
    rt_kprintf("\n");

    if (response)
    {
        web_free(response);
    }

    return 0;
}


int webclient_get_test(int argc, char **argv)
{
    char *uri = RT_NULL;

    if (argc == 1)
    {
        uri = web_strdup(GET_LOCAL_URI);
        if(uri == RT_NULL)
        {
            rt_kprintf("no memory for create get request uri buffer.\n");
            return -RT_ENOMEM;
        }
        webclient_get_comm(uri);
    }
    else if (argc == 2)
    {
        if (rt_strcmp(argv[1], "-s") == 0)
        {
            uri = web_strdup(GET_LOCAL_URI);
            if(uri == RT_NULL)
            {
                rt_kprintf("no memory for create get request uri buffer.\n");
                return -RT_ENOMEM;
            }

            webclient_get_smpl(uri);
        }
        else
        {
            uri = web_strdup(argv[1]);
            if(uri == RT_NULL)
            {
                rt_kprintf("no memory for create get request uri buffer.\n");
                return -RT_ENOMEM;
            }
            webclient_get_comm(uri);
        }
    }
    else if(argc == 3 && rt_strcmp(argv[1], "-s") == 0)
    {
        uri = web_strdup(argv[2]);
        if(uri == RT_NULL)
        {
            rt_kprintf("no memory for create get request uri buffer.\n");
            return -RT_ENOMEM;
        }

        webclient_get_smpl(uri);
    }
    else
    {
        rt_kprintf("web_get_test [URI]     - webclient GET request test.\n");
        rt_kprintf("web_get_test -s [URI]  - webclient simplify GET request test.\n");
        return -RT_ERROR;
    }

    if (uri)
    {
        web_free(uri);
    }

    return RT_EOK;
}

#ifdef FINSH_USING_MSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(webclient_get_test, web_get_test, webclient get request test);
#endif /* FINSH_USING_MSH */
