/*
 * @Description: 数据看板通用代码
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-04-09 11:28:17
 * @LastEditTime: 2023-04-10 15:59:05
 * @FilePath: lv_node/src/display/display_common.h
 */
#ifndef __DISPLAY_COMMON_H__
#define __DISPLAY_COMMON_H__

lv_obj_t *equipment_data_style(lv_obj_t *parent, const void *src, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
void tempehum_label_style(lv_obj_t *destlabel, lv_obj_t *alignlabel, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

lv_obj_t *CreateTemHumCard(lv_obj_t *parent);
lv_obj_t *CreateElectricityCard(lv_obj_t *parent);

lv_obj_t *CreateDataBoardtitle(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
lv_obj_t *CreateDataBoardtxt(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

int searchTemp(char *Buf, char *Res);
int searchHum(char *Buf, char *Res);
void *listening_temphum(void * parg);

#endif // __DEVICE_H__
