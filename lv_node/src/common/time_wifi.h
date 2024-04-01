/*** 
 * @Description: 主页面时间与logo标志
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:58:32
 * @LastEditTime: 2023-03-09 12:01:49
 * @FilePath: lv_node/src/common/time_wifi.h
 */
#ifndef _LV_TIME_WIFI_H
#define _LV_TIME_WIFI_H

extern lv_obj_t *g_mqtt_detection;

 lv_obj_t *create_wifi(lv_obj_t *parent);
 void create_wifi_and_time(lv_obj_t *parent);

#endif