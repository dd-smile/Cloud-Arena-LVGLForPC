/*
 * @Description: MQTT通用控制,调用MqttKit库
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/mqtt/onenet.h
 */
#ifndef _ONENET_H_
#define _ONENET_H_

//用于存储声光电的MQTT控制指令
typedef struct {
    char news_type[10];   //消息类型
    char dev_type[50];   //设备类型
    char eid[50];   //设备号
    char vid[50];   //数值
} Soundlight_Data;

void connect_mqtt();

_Bool OneNet_DevLink(void);

void OneNet_HeartBeat(void);

void OneNet_Subscribe(const char *topics[], unsigned char topic_cnt);

void OneNet_Publish(const char *topic, const char *msg);

void OneNet_RevPro(unsigned char *cmd);

void OneNet_ParseJsonNow(char *msg, Soundlight_Data *info);

void *recvMqttNews(void *parg);

extern char *sub_topics[];
extern unsigned char dataPtr[1024];

#endif