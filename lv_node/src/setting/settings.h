/*** 
 * @Description: 
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:48
 * @LastEditTime: 2023-07-03 14:06:52
 * @FilePath: lv_node/src/setting/settings.h
 */
#ifndef _LV_SETTING_H
#define _LV_SETTING_H


typedef struct {
    int x_pos;    //ｘ坐标
    int y_pos;    //ｙ坐标
    void (*card_func)(lv_obj_t *card);   //功能函数
    lv_event_cb_t event_cb;    //注册的事件

} card_info_t; //设置页面卡片数组



void CreateSettingPage(lv_obj_t*parent);


#endif