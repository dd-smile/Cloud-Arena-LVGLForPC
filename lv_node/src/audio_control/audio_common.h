/*
 * @Description: 音频通用控制
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/audio_common.h
 */
#ifndef __AUDIO_COMMON_H__
#define __AUDIO_COMMON_H__

uint16_t calculate_crc16(uint8_t *data, size_t length);
void CreateAudioPage(lv_obj_t *parent);

#endif
