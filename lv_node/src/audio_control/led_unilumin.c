/*
 * @Description: LED-洲明
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/led_unilumin.c
 */
#include "ui_app.h"

//型号x8e,只支持TCP,需要发送心跳包（只需发送，无需响应）端口号９９９９

/**
 * 洲明科技－发送心跳包，每秒发送一次
*/
void unilumin_Heartbeat_packet(void)
{
    //发送心跳包
    //...
    unsigned char packet[4];

    packet[0] = 0x99;
    packet[1] = 0x99;
    packet[2] = 0x04;
    packet[3] = 0x00;

    // TCP连接方式
    if (socketconnected(led_fd) != 0) 
    {
        write(led_fd, packet, sizeof(packet));
    }

}

/**
 * 设置led大屏输入源-洲明
 * @param transmitter_l        操作哪个发送器低字节     eg:0x0001  ->     0x00
 * @param transmitter_h        操作哪个发送器高字节     0x01
 * @param coverage             操作哪个图层
 * @param addr                 输入源
*/
void SetLedinputsource_unilumin(uint8_t transmitter_l, uint8_t transmitter_h, uint8_t coverage, uint8_t addr)
{
    unsigned char packet[18];

    packet[0] = 0x33;
    packet[1] = 0x00;
    packet[2] = 0x12;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x00;
    packet[6] = transmitter_l;
    packet[7] = transmitter_h;
    packet[8] = 0xFF;
    packet[9] = 0x00;
    packet[10] = 0x00;
    packet[11] = 0x00;
    packet[12] = 0x00;
    packet[13] = 0x00;
    packet[14] = 0x00;
    packet[15] = 0x00;
    packet[16] = coverage;
    packet[17] = addr;

    // TCP连接方式
    if (socketconnected(led_fd) != 0)
    {
        write(led_fd, packet, sizeof(packet));
    } 

    usleep(50 * 1000);

}





