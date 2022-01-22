#include <stdio.h>
#include <string.h>
#include "protocol.h"
#include "cJSON.h"


unsigned char proto_buf[PROTO_PACKET_MAX_LEN] = {0};
proto_detect_t proto_detect;


int proto_send_temperate(double temp)
{
    cJSON *json_root, *json_data;
    char *json_str;

    json_root = cJSON_CreateObject();
    cJSON_AddStringToObject(json_root, PROTO_ITEM_MAGIC, PROTO_MAGIC_CODE);
    cJSON_AddStringToObject(json_root, PROTO_ITEM_TYPE, PROTO_TYPE_STATUS);
    cJSON_AddItemToObject(json_root, PROTO_ITEM_DATA, json_data=cJSON_CreateObject());
    cJSON_AddNumberToObject(json_data, PROTO_STA_TEMP, temp);
    json_str = cJSON_Print(json_root);
    printf("JSON:\r\n%s\r\n", json_str);

    wifi_send_data(json_str, strlen(json_str));

    //free(json_str);
    free_pri(json_str);
    cJSON_Delete(json_root);

    return 0;
}

int proto_send_humidity(int humi)
{
    cJSON *json_root, *json_data;
    char *json_str;
    char humi_str[8] = {0};

    sprintf(humi_str, "%d%c", humi, '%');

    json_root = cJSON_CreateObject();
    cJSON_AddStringToObject(json_root, PROTO_ITEM_MAGIC, PROTO_MAGIC_CODE);
    cJSON_AddStringToObject(json_root, PROTO_ITEM_TYPE, PROTO_TYPE_STATUS);
    cJSON_AddItemToObject(json_root, PROTO_ITEM_DATA, json_data=cJSON_CreateObject());
    cJSON_AddStringToObject(json_data, PROTO_STA_HUMI, humi_str);
    json_str = cJSON_Print(json_root);
    printf("JSON:\r\n%s\r\n", json_str);

    wifi_send_data(json_str, strlen(json_str));

    //free(json_str);
    free_pri(json_str);
    cJSON_Delete(json_root);

    return 0;
}


int proto_cmd_dispatch(cJSON *json_data)
{
    cJSON *json_item;

    if(json_data == NULL)
        return -1;

    json_item = cJSON_GetObjectItem(json_data, PROTO_CMD_LED);
    if(json_item != NULL)
    {
        printf("cmd [%s]: %s\r\n", json_item->string, json_item->valuestring);
    }
    json_item = cJSON_GetObjectItem(json_data, PROTO_CMD_DOOR);
    if(json_item != NULL)
    {
        printf("cmd [%s]: %s\r\n", json_item->string, json_item->valuestring);

        // test
        proto_send_temperate(37.3);
        proto_send_humidity(80);
    }

    return 0;
}

int proto_msg_dispatch(cJSON *json_data)
{

    return 0;
}

int proto_status_dispatch(cJSON *json_data)
{

    return 0;
}

int proto_packet_analy(unsigned char *data, int len)
{
    cJSON *root, *json_magic, *json_type, *json_data;

    root = cJSON_Parse((const char *)data);
    if(!root)
    {
        printf("%s: json parse filed!\r\n", __FUNCTION__);
        return -1;
    }

    json_magic = cJSON_GetObjectItem(root, PROTO_ITEM_MAGIC);
    printf("get [%s]: %s\r\n", json_magic->string, json_magic->valuestring);
    if(strcmp(json_magic->valuestring, PROTO_MAGIC_CODE))
        return -1;

    json_type = cJSON_GetObjectItem(root, PROTO_ITEM_TYPE);
    printf("get [%s]: %s\r\n", json_type->string, json_type->valuestring);

    json_data = cJSON_GetObjectItem(root, PROTO_ITEM_DATA);

    if(!strcmp(json_type->valuestring, PROTO_TYPE_CMD))
    {
        proto_cmd_dispatch(json_data);
    }
    else if(!strcmp(json_type->valuestring, PROTO_TYPE_MSG))
    {
        proto_msg_dispatch(json_data);
    }
    else if(!strcmp(json_type->valuestring, PROTO_TYPE_STATUS))
    {
        proto_status_dispatch(json_data);
    }

	cJSON_Delete(root);

    return 0;
}

int proto_packet_detect(proto_detect_t *dectect_info, struct ringbuffer *ringbuf)
{
    unsigned char c = 0;
    int len;
    int i;

    len = ringbuf_datalen(ringbuf);
    if(len <= 0)
    {
        return 0;
    }

    for(i=0; i<len; i++)
    {
        ringbuf_read(ringbuf, &c, 1);
        if(c == '{' || dectect_info->head_num>0)
        {
            dectect_info->data[dectect_info->data_len] = c;
            dectect_info->data_len ++;
        }
        printf("%c", c);
        if(c == '{')
        {
            dectect_info->head_num ++;
        }
        else if(c == '}')
        {
            if(dectect_info->head_num > 0)
            {
                dectect_info->head_num --;
                if(dectect_info->head_num == 0)
                {
                    dectect_info->detect_flag = 1;
                    printf("\r\n### detect protocol: %s\r\n", dectect_info->data);
                    return 0;
                }
            }
        }
    }

    return 0;
}

int proto_server_handel(struct ringbuffer *ringbuf)
{

    if(ringbuf == NULL)
        return -1;

    proto_packet_detect(&proto_detect, ringbuf);
    if(proto_detect.detect_flag == 1)
    {
        proto_packet_analy(proto_detect.data, proto_detect.data_len);
        proto_detect.data_len = 0;
        proto_detect.detect_flag = 0;
        proto_detect.head_num = 0;
        memset(proto_detect.data, 0, PROTO_PACKET_MAX_LEN);
    }

    return 0;
}

int proto_init(void)
{

    memset(&proto_detect, 0, sizeof(proto_detect_t));
    proto_detect.data = proto_buf;

    return 0;
}

void proto_deinit(void)
{
    
}
