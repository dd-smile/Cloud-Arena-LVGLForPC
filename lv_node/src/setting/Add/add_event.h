/***
 * @Description:添加设备功能
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-05-16 18:13:23
 * @LastEditTime: 2023-05-16 18:21:02
 * @FilePath: lv_node/src/setting/Add/add_event.h
 */
#ifndef _LV_ADD_EVENT_H
#define _LV_ADD_EVENT_H

lv_obj_t *CreateDropDownList(lv_obj_t *parent);
lv_obj_t *CreateDownListLabel(lv_obj_t *parent, const char *text, lv_coord_t y);
lv_obj_t *lv_device_textarea(lv_obj_t *parent, uint32_t c, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb);
void CreateDeviceType(lv_obj_t *parent, const char *options, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb);

#endif