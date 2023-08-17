/*** 
 * @Description: 
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:48
 * @LastEditTime: 2023-07-03 14:06:52
 * @FilePath: /new_home_smart/lv_node/src/settings.h
 */
#ifndef _LV_SETTING_H
#define _LV_SETTING_H


typedef struct {
    int x_pos;
    int y_pos;
    void (*card_func)(lv_obj_t *card);
    lv_event_cb_t event_cb;

} card_info_t;



void CreateSettingPage(lv_obj_t*parent);


#endif