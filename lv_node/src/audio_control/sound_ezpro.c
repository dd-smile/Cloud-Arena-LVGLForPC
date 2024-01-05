/*
 * @Description: 易科音响控制
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-01-05 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/sound_ezpro.c
 */
#include "ui_app.h"

const char *ezpro_turn_up = "CC 2 1 9360 \r";
const char *ezpro_turn_down = "CC 2 0 9360 \r";

void sound_ezpro_turn_up()
{
    
    // sendto(multitrack_fd, ezpro_turn_up, sizeof(ezpro_turn_up), 0, (struct sockaddr *)&multitrack_addr, sizeof(multitrack_addr));
}