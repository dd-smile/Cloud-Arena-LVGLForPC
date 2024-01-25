/*
 * @Description: 灯光－润声
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-01-18 11:28:17
 * @LastEditTime: 2023-01-24 15:59:05
 * @FilePath: lv_node/src/audio_control/light_runshen.c
 */
#include "ui_app.h"

//八路，八个指令 crc16-modbus     port_number从00到07      status从00到64

/**
 * 设置调光亮度
 * @param addr             地址
 * @param port_number      写入第几路
 * @param status           写入的亮度数值
*/
void Setdimming_runshen(uint8_t addr, uint8_t port_number, uint8_t status)
{
    unsigned char packet[12];

    packet[0] = 0x00;
    packet[1] = 0x00;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x06;
    packet[6] = addr;
    packet[7] = 0x06;
    packet[8] = 0x00;
    packet[9] = port_number;
    packet[10] = 0x00;
    packet[11] = status;
    // packet[12] = calculate_crc16(packet, sizeof(packet) - 2) & 0xFF;
    // packet[13] = calculate_crc16(packet, sizeof(packet) - 2) >> 8;

    if(socketconnected(light_fd) != 0)
        write(light_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}