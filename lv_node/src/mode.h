/*** 
 * @Description: 模式切换页面
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-03-09 11:28:37
 * @LastEditTime: 2024-01-03 14:25:10
 * @FilePath: lv_node/src/mode.h
 */
#ifndef _LV_MODE_H
#define _LV_MODE_H

extern int g_mode_num;

void CreateModePage(lv_obj_t *obj);
void manual_Controls_event_cb(lv_event_t *e);

void mode_train_Controls(void);
void mode_performance_Controls(void);
void mode_competition_Controls(void);
void mode_halfcourt_Controls(void);

void Judgmentmode(void);


#endif