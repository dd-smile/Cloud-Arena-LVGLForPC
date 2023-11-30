/*
 * @Description: 数据看板展示页面
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-21 11:28:17
 * @LastEditTime: 2023-09-22 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/display/display.h
 */
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

typedef struct{
    lv_obj_t *label_temp;   //温度
    lv_obj_t *label_hum;    //湿度
    lv_obj_t *label_weather;   //天气
} Sensor_Data;

typedef struct {
    char id[16];
    char name[32];
    char country[16];
    char path[64];
    char timezone[32];
    char tz_offset[16];
    char text[16];
    char code[4];
    char temp[8];
    char last_update[32];
} weather_t;

extern char temp_data[1024];
extern char hum_data[1024];
extern lv_timer_t *timer;
extern uint16_t hour;
extern uint16_t last_hour;

void CreateDisplayPage(lv_obj_t *parent);
void timer_data_callback(lv_timer_t * timer);
void get_localtime();
void timer_weather_callback(lv_timer_t * timer); 
void timer_average_callback(lv_timer_t * timer);





#endif // __DEVICE_H__

