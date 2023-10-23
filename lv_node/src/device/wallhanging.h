/*
 * @Description: 璧挂球架
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-17 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/wallhanging.h
 */
#ifndef __WALLHANGING_H__
#define __WALLHANGING_H__

void CreateWallhangingPage(uint8_t device_num);
void wallhanging_Controls_event_cb(lv_event_t *e);
void set_wallhanging_stands_state(uint8_t num, uint8_t state);
void set_wallhanging_stop_state(uint8_t num);


#endif
