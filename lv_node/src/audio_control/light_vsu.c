/*
 * @Description: 北斗星灯光控制
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/light_vsu.c
 */
#include "ui_app.h"

//场景调用灯光控制
/*
                                    06功能码
                                    00   02    00    00    00 06         f0            05 00          02         FF 00
                                                                场景ID号：240（十进制）               场景号
*/
//unsigned char packet_open[12] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0xf0, 0x05, 0x00, 0x01, 0xFF, 0x00};
//unsigned char packet_close[12] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0xf0, 0x05, 0x00, 0x02, 0xFF, 0x00};

//设置灯光场景
void SetLightingscene_vsu(uint8_t addr)
{
    unsigned char packet[12];

    packet[0] = 0x00;
    packet[1] = 0x02;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x06;
    packet[6] = 0xf0;
    packet[7] = 0x05;
    packet[8] = 0x00;
    packet[9] = addr;
    packet[10] = 0xFF;
    packet[11] = 0x00;

    if (socketconnected(light_fd) != 0) 
        write(light_fd, packet, sizeof(packet));
    
    usleep(50 * 1000);
}

void SetLightSwitch_vsu(uint8_t addr, uint8_t port_number, uint8_t status)
{
    unsigned char packet[12];

    packet[0] = 0x00;
    packet[1] = 0x02;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x06;
    packet[6] = addr;
    packet[7] = 0x05;
    packet[8] = 0x00;
    packet[9] = port_number;
    if(status == 1)
        packet[10] = 0xFF;
    else
        packet[10] = 0x00;
    packet[11] = 0x00;


    if (socketconnected(light_fd) != 0) 
        write(light_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}

//status : 64为100亮度，00为0亮度
void Setdimming_vsu(uint8_t addr, uint8_t port_number, uint8_t status)
{
    unsigned char packet[12];

    packet[0] = 0x00;
    packet[1] = 0x02;
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

    if (socketconnected(light_fd) != 0) 
        write(light_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}
