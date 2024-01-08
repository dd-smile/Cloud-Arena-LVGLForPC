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

void connect_mqtt();

_Bool OneNet_DevLink(void);

void OneNet_HeartBeat(void);

void OneNet_Subscribe(const char *topics[], unsigned char topic_cnt);

void OneNet_Publish(const char *topic, const char *msg);

// void OneNet_RevPro(unsigned char *cmd);

#endif