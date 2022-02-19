#ifndef _WIFI_ESP8266_H_
#define _WIFI_ESP8266_H_


#define AT_TEST         "AT\r\n"        // 测试AT指令
#define AT_RESET        "AT+RST\r\n"    // 重启模块
#define AT_ECHO_OFF     "ATE0\r\n"      // 关闭回显

#define AT_SET_MODE   "AT+CWMODE"      // 设置wifi模式
#define AT_SET_MUT_CONNECT  "AT+CIPMUX"   // 设置多连接模式
#define AT_SET_TCP_SERVER   "AT+CIPSERVER=1"    // 建立TCP服务器
#define AT_SET_CONFIG_AP    "AT+CWSAP"     // 配置AP模式参数
#define AT_SET_AP_IP        "AT+CIPAP"     // 配置AP模式的IP地址
#define AT_SET_TCP_MODE        "AT+CIPMODE"    // 设置传输模式：普通/透传
#define AT_SET_TCP_SEND        "AT+CIPSEND"    // 发送数据
#define AT_SET_TCP_TIMEOUT        "AT+CIPSTO"  // 设置服务器超时时间

#define AT_CONNCET_AP       "AT+CWJAP"          // 连接AP
#define AT_TCP_CONNECT_SVR  "AT+CIPSTART=\"TCP\""   // 建立TCP连接

#define AT_GET_LOCAL_IP     "AT+CIFSR\r\n"      // 查询本地IP地址


#define AT_ACK_OK           "OK"
#define AT_ACK_RST          "ready"

#define WIFI_CONNECT_STR    "CONNECT"
#define WIFI_CLOSE_STR    "CLOSED"
#define WIFI_RECV_HEAD  "+IPD"

typedef enum{
    WIFI_MODE_NONE=0,     // no-wifi(close)
    WIFI_MODE_STA=1,      // station
    WIFI_MODE_AP=2,       // AP
    WIFI_MODE_STA_AP=3,   // station + AP
}wifi_mode_e;

typedef enum{
    TCP_ROLE_SERVER,
    TCP_ROLE_CLIENT,
}tcp_role_e;

typedef struct{
    wifi_mode_e mode;
    tcp_role_e tcp_role;   // server/client
}wifi_mngr_info_t;


int wifi_set_tcp_timeout(int timeout);
int wifi_config_tcp_server(const char *ip, const int port);
int wifi_config_AP_mode(const char *ssid, const char *pwd);
int wifi_config_mode(wifi_mode_e mode);
// 多连接模式：0-单连接，1-多连接
int wifi_config_mut_connect(int mut_flag);

int wifi_connect_AP(const char *ssid, const char *pwd);
int wifi_connect_server(const char *ip, int port);

int wifi_send_data(char *buf, int len);
int wifi_set_AT(char *at, int len, const char *ack, int timeout);
void wifi_reset(void);
int wifi_init(void);
void wifi_deinit(void);


#endif	// _WIFI_ESP8266_H_
