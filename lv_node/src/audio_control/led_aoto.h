/*
 * @Description: LED-奥拓
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/led_aoto.h
 */
#ifndef __LED_AOTO_H__
#define __LED_AOTO_H__

extern int led_fd;
extern struct sockaddr_in seraddr;

void SetLedinputsource_aoto(uint8_t coverage, uint8_t neck, uint8_t addr);
void *create_client_led();

#endif