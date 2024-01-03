/*
 * @Description: LED-奥拓
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/led_aoto.c
 */
#include "ui_app.h"


int led_fd = 0;
struct sockaddr_in seraddr;

//从第三个开始
static unsigned char checksum8(const unsigned char *data, size_t size)
{
    unsigned char sum = 0;
    
    for (size_t i = 3; i < size; ++i)
    {
        sum += data[i];
    }
    
    return sum & 0xFF;
}

/**
 * 设置led大屏输入源-奥拓
 * @param coverage              图层
 * @param neck                  卡槽
 * @param addr                  输入源
*/
void SetLedinputsource_aoto(uint8_t coverage, uint8_t neck, uint8_t addr)
{
    unsigned char packet[39];

    packet[0] = 0xED;
    packet[1] = 0xCB;
    packet[2] = 0x28;
    packet[3] = 0x00;
    packet[4] = 0x30;
    packet[5] = 0x02;
    packet[6] = coverage;  
    packet[7] = 0x00;
    packet[8] = neck;
    packet[9] = addr;
    packet[10] = 0x00;
    packet[11] = 0x55;
    packet[12] = 0x55;
    packet[13] = 0x55;
    packet[14] = 0x55;;
    packet[15] = 0x55;
    packet[16] = 0x55;
    packet[17] = 0x55;
    packet[18] = 0x55;
    packet[19] = 0x55;
    packet[20] = 0x55;
    packet[21] = 0x55;
    packet[22] = 0x55;
    packet[23] = 0x55;
    packet[24] = 0x55;
    packet[25] = 0x55;
    packet[26] = 0x55;
    packet[27] = 0x55;
    packet[28] = 0x55;
    packet[29] = 0x55;
    packet[30] = 0x55;
    packet[31] = 0x55;
    packet[32] = 0x55;
    packet[33] = 0x55;
    packet[34] = 0x55;
    packet[35] = 0x55;
    packet[36] = 0x55;
    packet[37] = 0x55;
    packet[38] = checksum8(packet, sizeof(packet)-1);

    //默认连接方式　　ＵＤＰ
    sendto(led_fd, packet, sizeof(packet)/sizeof(packet[0]), 0, (struct sockaddr*)&seraddr, sizeof(seraddr));

}

/**
 * 创建LED客户端  UDP
 * */
void *create_client_led()
{
    if(led_fd == 0)
    {
        led_fd= socket(AF_INET, SOCK_DGRAM, 0);
        if(led_fd == -1)
        {
            perror("socket");
            exit(0);
        }
        
        seraddr.sin_family = AF_INET;
        seraddr.sin_port = htons(61214);    
        inet_pton(AF_INET, "192.168.1.223", &seraddr.sin_addr.s_addr);
    }
}
