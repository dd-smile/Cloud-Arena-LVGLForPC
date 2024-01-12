/*
 * @Description:屏幕设置
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-03-09 11:28:42
 * @LastEditTime: 2023-12-25 10:34:31
 * @FilePath: lv_node/src/setting/Screen/Screen.h
 */
#ifndef _LV_SCREEN_SETTING_H
#define _LV_SCREEN_SETTING_H

#define  SCREEN_SETTING_JSON  "/home/csd/demo/lx_screen_setting.json"  


void CreateScreen(lv_obj_t *parent);
void add_Screen_event_cb(lv_event_t *e);
lv_obj_t *create_page_back(lv_obj_t *obj);
void *create_allset_stlye(lv_obj_t *parent);
lv_obj_t *create_screen_btn(lv_obj_t *parent);


#endif
