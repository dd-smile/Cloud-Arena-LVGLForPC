/*
 * @Description: 分隔幕
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-07-03 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/partition_curtian.h
 */
#ifndef __PARTITION_CURTAION_H__
#define __PARTITION_CURTAION_H__


extern bool patition_flag;

void partition_Controls_event_cb(lv_event_t *e);
void CreatePartitionCurtianPage(uint8_t device_num);
void set_partition_stands_state(uint8_t num, uint8_t state);
void set_partition_stop_state(uint8_t num, uint8_t state);

#endif //__SMART_SEAT_BIG_H__