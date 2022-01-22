#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "usart3.h"
#include "ringbuffer.h"
#include "wifi_esp8266.h"

// wifi receive data buffer
unsigned char ring_buf[512];
struct ringbuffer wifi_ringbuf;

// wifi manager struct
wifi_mngr_info_t wifi_mngr;


int wifi_config_tcp_server(const char *ip, const int port)
{
    char at_buf[64] = {0};
    int ret;

    if(ip==NULL || port==NULL)
        return -1;

    memset(at_buf, 0, sizeof(at_buf));
    sprintf(at_buf, "%s\"%s\"\r\n", AT_SET_AP_IP, ip);

	ret = wifi_set_AT(AT_SET_MUT_CONNECT, strlen(AT_SET_MUT_CONNECT), AT_ACK_OK, 1000);

    memset(at_buf, 0, sizeof(at_buf));
    sprintf(at_buf, "%s,%d\r\n", AT_SET_TCP_SERVER, port);
    ret = wifi_set_AT(at_buf, strlen(at_buf), AT_ACK_OK, 1000);

    wifi_mngr.tcp_role = TCP_ROLE_SERVER;

    return ret;
}

int wifi_config_AP_mode(const char *ssid, const char *pwd)
{
    char at_buf[64] = {0};
    int ret;

    if(ssid == NULL)
        return -1;
    
	ret = wifi_config_mode(WIFI_MODE_AP);

    memset(at_buf, 0, sizeof(at_buf));
    sprintf(at_buf, "%s\"%s\",\"%s\",%d,%d\r\n", AT_SET_CONFIG_AP, ssid, pwd, 5, 3);

    ret = wifi_set_AT(at_buf, strlen(at_buf), AT_ACK_OK, 1000);

    return ret;
}

int wifi_config_mode(wifi_mode_e mode)
{
    char at_buf[64] = {0};
    int ret;
    
    memset(at_buf, 0, sizeof(at_buf));
    sprintf(at_buf, "%s%d\r\n", AT_SET_MODE, mode);

    ret = wifi_set_AT(at_buf, strlen(at_buf), AT_ACK_OK, 1000);

    return ret;
}

int wifi_send_data(char *buf, int len)
{
    char at_buf[64] = {0};

    memset(at_buf, 0, sizeof(at_buf));
    if(wifi_mngr.tcp_role == TCP_ROLE_SERVER)
    {
        sprintf(at_buf, "%s%d,%d\r\n", AT_SET_TCP_SEND, 0, len);
    }

    // send AT cmd
    wifi_set_AT(at_buf, strlen(at_buf), NULL, 0);
    delay_ms(10);
    // send data
    uart3_send(buf, len);
	printf("%s[%d]: %s\r\n", __FUNCTION__, len, buf);

    return len;
}

int wifi_set_AT(char *AT_buf, int len, const char *ack, int timeout)
{
    char ack_data[32] = {0};
    int data_len = 0;
    int ack_len;
    int result = 0;
    int ret;
    int i = 0;

    if(AT_buf==NULL || len<=0)
        return -1;

    uart3_send(AT_buf, len);

    if(ack != NULL)
    {
        ack_len = strlen(ack);
        result = -1;
        for(i=0; i<timeout; )
        {
            if(ringbuf_datalen(&wifi_ringbuf) > 0)
            {
                ret = ringbuf_read(&wifi_ringbuf, (unsigned char *)ack_data+data_len, sizeof(ack_data)-data_len);
                if(ret > 0)
                {
                    if(strstr(ack_data, ack) != NULL)
                    {
                        printf("esp8266 set %s success.\r\n", AT_buf);
                        result = 0;
                        break;
                    }
                    // no match
                    data_len += ret;
                    if(data_len >= ack_len)
                    {
                        memcpy(ack_data, ack_data+data_len-(ack_len-1), ack_len-1);
                        data_len = ack_len-1;
                    }
                }
            }
            else
            {
                delay_ms(1);
                i++;
            }
        }
        if(i >= timeout)
        {
            printf("error: set %s failed!\r\n", AT_buf);
        }
    }

    return result;
}

void wifi_reset(void)
{
    printf("wifi reset ...\r\n");
    wifi_set_AT(AT_RESET, strlen(AT_RESET), AT_ACK_RST, 2*1000);
    //delay_ms(3*1000);
}

int wifi_init(void)
{
    memset(&wifi_mngr, 0, sizeof(wifi_mngr_info_t));

    ringbuf_init(&wifi_ringbuf, ring_buf, sizeof(ring_buf));
	usart3_init(115200);

    wifi_reset();
	//wifi_set_AT(AT_RESET, strlen(AT_RESET), NULL, 0);
	wifi_set_AT(AT_ECHO_OFF, strlen(AT_ECHO_OFF), AT_ACK_OK, 1000);
	wifi_set_AT(AT_TEST, strlen(AT_TEST), AT_ACK_OK, 1000);

    printf("wifi esp8266 init OK.\r\n");

    return 0;
}

void wifi_deinit(void)
{
    
}
