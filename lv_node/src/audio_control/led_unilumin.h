/*
 * @Description: LED-洲明
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/led_unilumin.h
 */
#ifndef __LED_UNILUMIN_H__
#define __LED_UNILUMIN_H__

void unilumin_Heartbeat_packet(void);
void SetLedinputsource_unilumin(uint8_t transmitter_l, uint8_t transmitter_h, uint8_t coverage, uint8_t addr);

#endif
