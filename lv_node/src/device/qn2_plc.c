/*
 * @Description: LED电源控制-ＱＮ２－ＰＬＣ
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/device/qn2_plc.c
 */
#include "ui_app.h"

int g_plc_fd = 0;


/**
 * 设置第几路开关值来打开电源
 * @param status    表明第几路的打开还是关闭
*/
void SetLedpower_abesn(uint8_t status)
{
    unsigned char packet[12];

    packet[0] = 0x00;
    packet[1] = 0x00;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x06;
    packet[6] = 0x01;
    packet[7] = 0x05;  
    packet[8] = 0x08;  
    packet[9] = status;
    packet[10] = 0xFF;
    packet[11] = 0x00;

    //modbus TCP自己新建客户端  网口 IP：192.168.0.15 端口号为 5000
    if (socketconnected(g_plc_fd) != 0) 
    {
        write(g_plc_fd, packet, sizeof(packet));
    }

    usleep(50 * 1000);

}