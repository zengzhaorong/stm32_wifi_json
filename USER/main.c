#include <stdio.h>
#include <string.h>
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "ringbuffer.h"
#include "cJSON.h"
#include "wifi_esp8266.h"
#include "protocol.h"

extern unsigned char ring_buf[512];
extern struct ringbuffer wifi_ringbuf;

/************************************************
 ALIENTEK精英STM32开发板实验4
 串口 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 int main(void)
 {		
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200

	wifi_init();
	proto_init();

	 wifi_config_AP_mode("hello-esp8266", "88888888");
	 wifi_config_tcp_server("192.168.1.101", 9100);

	 wifi_set_AT(AT_GET_LOCAL_IP, strlen(AT_GET_LOCAL_IP), NULL, 0);

 	while(1)
	{
		
		proto_server_handel(&wifi_ringbuf);

	}	 
 }

