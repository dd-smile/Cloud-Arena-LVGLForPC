/*
 * @Description: LED电源控制-ＱＮ２－ＰＬＣ
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/device/qn2_plc.h
 */
#ifndef __QN2_PLC_H__
#define __QN2_PLC_H__

extern int g_plc_fd;

void SetLedpower_abesn(uint8_t status);

#endif