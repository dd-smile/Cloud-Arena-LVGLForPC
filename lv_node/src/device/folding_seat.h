/*
 * @Description: 地面翻折座椅
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-17 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/folding_seat.h
 */
#ifndef __FOLDING_SEAT_H__
#define __FOLDING_SEAT_H__


void CreateFoldingPage(uint8_t device_num);
void folding_Controls_event_cb(lv_event_t *e);
void set_folding_stands_state(uint8_t num, uint8_t state);
void set_folding_stop_state(uint8_t num, uint8_t state);

#endif
