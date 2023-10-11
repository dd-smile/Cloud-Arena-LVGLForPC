/*** 
 * @Description: 
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:12
 * @LastEditTime: 2023-07-03 15:58:46
 * @FilePath: /new_home_smart/lv_node/src/home.h
 */
#ifndef _LV_HOME_H
#define _LV_HOME_H


typedef struct {
    lv_obj_t *home_button[4];   // 设备页导航栏
    lv_obj_t *all_pages;    // 全部页面
    lv_obj_t *display_page;  // 数据看板页面
    lv_obj_t *audio_page; // 音频控制页面
    lv_obj_t *finance_page; // 厨房页面
} HomeData;
extern HomeData device_data;    // 设备页数据
lv_obj_t *device_page_box(lv_obj_t *parent, int num_pages, ...);
void CreateHomePage(lv_obj_t*parent);
#endif