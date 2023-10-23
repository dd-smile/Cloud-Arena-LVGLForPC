/*
 * @Description: 伸缩舞台与升降舞台
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-17 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15   CONTRACTION_STAGE_H
 * @FilePath: lv_node/src/device/contraction_stage.h
 */
#ifndef __CONTRACTION_STAGE_H__
#define __CONTRACTION_STAGE_H__


void CreateContractionPage(uint8_t device_num);
void CreateRevolvingPage(uint8_t device_num);
void contraction_Controls_event_cb(lv_event_t *e);
void telescopic_Controls_event_cb(lv_event_t *e);
void revolving_Controls_event_cb(lv_event_t *e);
void stage_Controls_event_cb(lv_event_t *e);


#endif
