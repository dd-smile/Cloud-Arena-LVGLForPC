/*
 * @Description: 
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-04-11 08:58:49
 * @LastEditTime: 2024-04-11 17:33:15
 * @FilePath: lv_node/src/device/curtain.h
 */
#ifndef __CURTAIN_H__
#define __CURTAIN_H__

void CreateCurtainPage(uint8_t device_num);
void curtain_Controls_event_cb(lv_event_t *e);

#endif   