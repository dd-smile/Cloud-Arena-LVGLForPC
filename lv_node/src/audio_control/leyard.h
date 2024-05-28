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

extern int multitrack_fd;
extern int multitrack_red;
extern char buf_audio[1024];
extern lv_obj_t *connect_label;

void *create_client_mu(void * parg);
void multitrack_Controls_event_cb(lv_event_t *e);
void synchronous_mutework();
void synchronous_unmutework();
void audio_working();




#endif // __DEVICE_H__

