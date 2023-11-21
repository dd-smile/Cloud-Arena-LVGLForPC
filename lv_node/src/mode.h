/*** 
 * @Description: 
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:37
 * @LastEditTime: 2023-07-03 14:25:10
 * @FilePath: /new_home_smart/lv_node/src/mode.h
 */
#ifndef _LV_MODE_H
#define _LV_MODE_H

extern int mode_num;

void CreateModePage(lv_obj_t *obj);
void manual_Controls_event_cb(lv_event_t *e);


#endif