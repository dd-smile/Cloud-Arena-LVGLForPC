/*
 * @Description: 北斗星灯光控制
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/light_vsu.h
 */
#ifndef __LIGHT_VSU_H__
#define __LIGHT_VSU_H__

void SetLightingscene(uint8_t addr);
void SetLightSwitch(uint8_t addr, uint8_t port_number, uint8_t status);
void Setdimming(uint8_t addr, uint8_t port_number, uint8_t status);


#endif
