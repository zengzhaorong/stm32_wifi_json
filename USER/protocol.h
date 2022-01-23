#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "ringbuffer.h"



#define PROTO_PACKET_MAX_LEN    256

#define PROTO_ITEM_MAGIC        "magic"
#define PROTO_ITEM_RESULT       "result"
#define PROTO_ITEM_TYPE         "type"
#define PROTO_ITEM_DATA         "data"

#define PROTO_MAGIC_CODE        "abcd"

#define PROTO_RESULT_TRUE       "true"
#define PROTO_RESULT_FALSE      "false"

#define PROTO_TYPE_CMD          "cmd"
#define PROTO_TYPE_MSG          "msg"
#define PROTO_TYPE_STATUS       "status"
#define PROTO_TYPE_QUERY        "queryDevice"

#define PROTO_CMD_STATE_ON      "on"
#define PROTO_CMD_STATE_OFF     "off"

#define PROTO_DEV_LED           "led"
#define PROTO_DEV_DOOR          "door"
#define PROTO_DEV_WINDOW        "window"
#define PROTO_DEV_HUMIDIFIER    "humidifier"

#define PROTO_STATUS_NORMAL      "正常"
#define PROTO_STATUS_ABNORMAL    "异常"

#define PROTO_STATUS_TEMP_HUMI      "temp-humi"
#define PROTO_STATUS_SMOKE          "smoke"
#define PROTO_STATUS_TEMP          "temperate"
#define PROTO_STATUS_HUMI          "humidity"


typedef struct 
{
    int head_num;   // '{' 个数
    int detect_flag;    // 检测一包协议完成标志，1-完成
    unsigned char *data;    // 协议数据
    int data_len;   //数据长度
}proto_detect_t;


int proto_send_dev_state(void);

int proto_send_temp_humi(double temp, int humi);
int proto_send_smoke_status(char *status);

int proto_send_ack_result(char *type, char *result);

int proto_server_handel(struct ringbuffer *ringbuf);

int proto_init(void);
void proto_deinit(void);


#endif	// _PROTOCOL_H_
