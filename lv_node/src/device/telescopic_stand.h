/*** 
 * @Description: 伸缩活动看台
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-03 13:58:54
 * @LastEditTime: 2023-07-03 13:59:19
 * @FilePath: /new_home_smart/lv_node/src/device/telescopic_stand.h
 */
#ifndef __TELESCOPIC_STAND_H__
#define __TELESCOPIC_STAND_H__

void telescoopic_Controls_event_cb(lv_event_t *e);
void Controls_event_cb(lv_event_t *e);

void CreateTelescopicStandPage(uint8_t device_num);


extern bool stand_flag;
extern uint8_t ReadSerialDiNum;
extern int Switch_status;

#endif //__TELESCOPIC_STAND_H__