/*
 * @Description: 音频控制面板－利亚德
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-26 11:28:17
 * @LastEditTime: 2023-09-28 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/audio_control/leyard.h
 */
#ifndef __LEYARD_H__
#define __LEYARD_H__

extern int led_fd;
extern int multitrack_fd;
extern int multitrack_red;
extern int synchronous_fd;
extern int synchronous_red;
extern char buf_audio[1024];
extern unsigned char packet_led2[39];
extern unsigned char packet_led4[39];
extern struct sockaddr_in seraddr;

void CreateAudioPage(lv_obj_t *parent);
void *create_client_led();





#endif // __DEVICE_H__

