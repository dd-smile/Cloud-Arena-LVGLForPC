/*
 * @Description: 中立智能电箱
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-04-17 13:58:49
 * @LastEditTime: 2024-04-24 19:33:15
 * @FilePath: lv_node/src/device/neutral_box.c
 */
#include "ui_app.h"

int g_neutral_fd = -1;

/**
 * 异或
*/
uint16_t calculate_xor16(uint8_t *data, size_t length)
{
    uint16_t crc = 0x00;

    for(size_t i = 0; i < length; i++)
    {
        crc ^= data[i];
    }

    return crc;
}

/**
 * 设置套餐，负载在官龙没有用到/负载用于灯光         小端发送      11正转  10反转  00或01不工作
 * @param motor_four                 电机4-1
 * @param motor_eight                电机8-5                 neutral
 * @param motor_twelve               电机12-9
 * @param motor_fifteen              电机15-13
*/
void Setcombo_zhongli(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen)
{
    unsigned char packet[10];

    packet[0] = 0xAA;
    packet[1] = 0x00;
    packet[2] = motor_four;
    packet[3] = motor_eight;
    packet[4] = motor_twelve;
    packet[5] = motor_fifteen;
    packet[6] = 0x00;
    packet[7] = 0x00;
    packet[8] = calculate_xor16(packet, sizeof(packet) - 2);
    packet[9] = 0x55;

    if(socketconnected(g_neutral_fd) != 0)
        write(g_neutral_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}

/**
 * 完整版设置套餐，有负载/负载用于灯光          小端发送      11正转  10反转  00或01不工作  xx保留            1开启  0关闭 x不用
 * @param motor_four                      电机4-1     一个电机占用两个byte
 * @param motor_eight                     电机8-5                 
 * @param motor_twelve                    电机12-9
 * @param motor_fifteen                   电机15-13
 * @param load_eight                      负载8-1     一个负载占用一个byte
 * @param load_ten                        负载10-9
*/
void Setcombo_neutral(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen, uint8_t load_eight, uint8_t load_ten)
{
    unsigned char packet[10];

    packet[0] = 0xAA;
    packet[1] = 0x00;
    packet[2] = motor_four;
    packet[3] = motor_eight;
    packet[4] = motor_twelve;
    packet[5] = motor_fifteen;
    packet[6] = load_eight;
    packet[7] = load_ten;
    packet[8] = calculate_xor16(packet, sizeof(packet) - 2);
    packet[9] = 0x55;

    if(socketconnected(g_neutral_fd) != 0)
        write(g_neutral_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}


/**
 * 中立智能电箱套餐开场
*/
void Setopen_zhongli(void)
{
    unsigned char packet[8];

    packet[0] = 0xAA;
    packet[1] = 0x01;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x00;
    packet[6] = 0xAB;
    packet[7] = 0x55;

    if(socketconnected(g_neutral_fd) != 0)
        write(g_neutral_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}

/**
 * 中立智能电箱套餐离场
*/
void Setdeparture_zhongli(void)
{
    unsigned char packet[8];

    packet[0] = 0xAA;
    packet[1] = 0x02;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x00;
    packet[6] = 0xA8;
    packet[7] = 0x55;

    if(socketconnected(g_neutral_fd) != 0)
        write(g_neutral_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}

/**
 * 中立智能电箱急停
*/
void Jerk_zhongli(void)
{
    unsigned char packet[8];

    packet[0] = 0xAA;
    packet[1] = 0x03;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0x00;
    packet[5] = 0x00;
    packet[6] = 0xA9;
    packet[7] = 0x55;

    if(socketconnected(g_neutral_fd) != 0)
        write(g_neutral_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}

/**
 * 输出指令，负载在官龙没有用到/负载用于灯光         小端发送      11正转  10反转  00或01不工作   
 * @param motor_four                          电机4-1
 * @param motor_eight                         电机8-5
 * @param motor_twelve                        电机12-9
 * @param motor_fifteen                       电机15-13
*/
void Setoutput_zhongli(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen)
{
    unsigned char packet[10];

    packet[0] = 0xAA;
    packet[1] = 0x05;
    packet[2] = motor_four;
    packet[3] = motor_eight;
    packet[4] = motor_twelve;
    packet[5] = motor_fifteen;
    packet[6] = 0x00;
    packet[7] = 0x00;
    packet[8] = calculate_xor16(packet, sizeof(packet) - 2);
    packet[9] = 0x55;

    // for(int i = 0; i < 10; i++)
    // {
    //     printf("XOR:%02X\n",packet[i]);
    // }

    if(socketconnected(g_neutral_fd) != 0)
        write(g_neutral_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}

/**
 * 完整版输出指令，有负载/负载用于灯光          小端发送      11正转  10反转  00或01不工作 　xx保留           　　　1开启  0关闭 x不用
 * @param motor_four                 电机4-1     一个电机占用两个byte
 * @param motor_eight                电机8-5
 * @param motor_twelve               电机12-9
 * @param motor_fifteen              电机15-13
 * @param load_eight                 负载8-1     一个负载占用一个byte
 * @param load_ten                   负载10-9
*/
void Setoutput_neutral(uint8_t motor_four, uint8_t motor_eight, uint8_t motor_twelve, uint8_t motor_fifteen, uint8_t load_eight, uint8_t load_ten)
{
    unsigned char packet[10];

    packet[0] = 0xAA;
    packet[1] = 0x05;
    packet[2] = motor_four;
    packet[3] = motor_eight;
    packet[4] = motor_twelve;
    packet[5] = motor_fifteen;
    packet[6] = load_eight;
    packet[7] = load_ten;
    packet[8] = calculate_xor16(packet, sizeof(packet) - 2);
    packet[9] = 0x55;

    if(socketconnected(g_neutral_fd) != 0)
        write(g_neutral_fd, packet, sizeof(packet));

    usleep(50 * 1000);
}
