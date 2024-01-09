/*
 * @Description: 优莱德智能照明灯光控制
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-01-05 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/light_ulaide.c
 */
#include "ui_app.h"

//modbus TCP
/*
0号调光模块     最高FA                最低00           0号开关模块      01开        00关
    00 00      00 00     00 06    01      06       00 00       00 01
   事物处理     协议标识    长度   网关地址   功能码   寄存器起始位   写入值
*/
/*
0号模块回路1的寄存器地址为    00 00
      回路2的寄存器地址为    00 01
...
1号模块回路1的寄存器地址为    00 0c(12)
1号模块回路2的寄存器地址为    00 0d(13)
寄存器地址＝模块地址*12+回路序号，回路序号从0开始
*/

/**
 * 设置灯光开关或灯光的亮度
 * @param addr             网关地址
 * @param register_h       寄存器地址高位
 * @param register_l       寄存器地址低位
 * @param status_h         状态值高位
 * @param status_l         状态值低位
*/
void SetLightSwitch_ulaide(uint8_t addr, uint8_t register_h, uint8_t register_l, uint8_t status_h, uint8_t status_l)
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
    packet[8] = register_h;
    packet[9] = register_l;
    packet[10] = status_h;
    packet[11] = status_l;

    if (socketconnected(light_fd) != 0) 
    {
        write(light_fd, packet, sizeof(packet));
    }
    
    usleep(50 * 1000);
}   