#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "ringbuffer.h"



#define PROTO_PACKET_MAX_LEN    256

#define PROTO_ITEM_MAGIC        "magic"
#define PROTO_ITEM_CODE         "code"
#define PROTO_ITEM_TYPE         "type"
#define PROTO_ITEM_DATA         "data"

#define PROTO_MAGIC_CODE        "abcd"

#define PROTO_TYPE_CMD          "cmd"
#define PROTO_TYPE_MSG          "msg"
#define PROTO_TYPE_STATUS       "status"

#define PROTO_CMD_LED           "led"
#define PROTO_CMD_DOOR          "door"

#define PROTO_STA_TEMP          "temperate"
#define PROTO_STA_HUMI          "humidity"


typedef struct 
{
    int head_num;   // '{' 个数
    int detect_flag;    // 检测一包协议完成标志，1-完成
    unsigned char *data;    // 协议数据
    int data_len;   //数据长度
}proto_detect_t;


int proto_send_temperate(double temp);
int proto_send_humidity(int humi);

int proto_server_handel(struct ringbuffer *ringbuf);

int proto_init(void);
void proto_deinit(void);


#endif	// _PROTOCOL_H_
