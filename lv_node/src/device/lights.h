/*
 * @Description: 灯具
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-17 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/lights.h
 */
#ifndef __LIGHTS_H__
#define __LIGHTS_H__

extern unsigned char packet_open[12];
extern unsigned char packet_close[12];

void lights_Controls_event_cb(lv_event_t *e);
void CreateLightsPage(uint8_t device_num);


#endif //__TELESCOPIC_STAND_H__