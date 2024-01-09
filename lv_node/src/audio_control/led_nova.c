/*
 * @Description: LED-诺瓦
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/led_nova.c
 */
#include "ui_app.h"

// 声明SUM_H和SUM_L
unsigned char sum_h, sum_l;
/*
常用示例
窗口 1 切源 命令数据   
HDMI1 55 aa 00 00 fe 00 00 00 00 00 01 00 12 00 02 13 03 00 00 00 00 7e 56
HDMI2 55 aa 00 00 fe 00 00 00 00 00 01 00 12 00 02 13 03 00 01 00 00 7f 56
DVI1 55 aa 00 00 fe 00 00 00 00 00 01 00 12 00 02 13 03 00 02 00 00 80 56
SDI 55 aa 00 00 fe 00 00 00 00 00 01 00 12 00 02 13 03 00 03 00 00 81 56
窗口 2 切源 命令数据
HDMI1 55 aa 00 00 fe 00 00 00 00 00 01 00 42 00 02 13 03 00 00 00 00 ae 56
HDMI2 55 aa 00 00 fe 00 00 00 00 00 01 00 42 00 02 13 03 00 01 00 00 af 56
DVI1 55 aa 00 00 fe 00 00 00 00 00 01 00 42 00 02 13 03 00 02 00 00 b0 56
SDI 55 aa 00 00 fe 00 00 00 00 00 01 00 42 00 02 13 03 00 03 00 00 b1 56
窗口 3 切源 命令数据
HDMI1 55 aa 00 00 fe 00 00 00 00 00 01 00 72 00 02 13 03 00 00 00 00 de 56
HDMI2 55 aa 00 00 fe 00 00 00 00 00 01 00 72 00 02 13 03 00 01 00 00 df 56
DVI1 55 aa 00 00 fe 00 00 00 00 00 01 00 72 00 02 13 03 00 02 00 00 e0 56
SDI 55 aa 00 00 fe 00 00 00 00 00 01 00 72 00 02 13 03 00 03 00 00 e1 56

*/

// 计算数组中所有元素的和
static unsigned int calculateArraySum(const unsigned int *array, size_t length) 
{
    unsigned int array_sum = 0;
    for (size_t i = 2; i < length; ++i) 
    {
        array_sum += array[i];
    }
    // 要相加的十六进制数
    unsigned int hex_to_add = 0x5555;
    array_sum += hex_to_add;
    return array_sum;
}

// 计算SUM_H和SUM_L
static void calculateSumHL(unsigned int sum, unsigned char *sum_h, unsigned char *sum_l) 
{
    // 将SUM分解为高字节和低字节
    *sum_h = (sum >> 8) & 0xFF; // 高字节
    *sum_l = sum & 0xFF;        // 低字节
}


/**
 * 设置led大屏输入源-诺瓦
 * 型号：v1260
 * 窗口１：addr0=0x12, addr1=0x00, addr2=0x02, addr3=0x13
 * 窗口２：addr0=0x42, addr1=0x00, addr2=0x02, addr3=0x13
 * 窗口３：addr0=0x72, addr1=0x00, addr2=0x02, addr3=0x13
 * 窗口４：addr0=0xA2, addr1=0x00, addr2=0x02, addr3=0x13
 * 窗口５：addr0=0xD2, addr1=0x00, addr2=0x02, addr3=0x13
 * 卡槽序号编码：   HDMI     0x00
 * 　　　　　　　　 DVI1     0X01
 *               DVI2     0X02
 *               DVI3     0X03
 *               DVI4     0X04
 *               SDI      0X05
 * @param addr                  表示窗口参数地址 addr=addr3<<24 +addr2<<16 + addr1<<8 +addr0
 * @param cardno                卡槽
*/
void SetLedinputsource_nova(uint8_t addr0, uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t cardno)
{
    unsigned int packet[23];

    packet[0] = 0x55;
    packet[1] = 0xaa;
    packet[2] = 0x00;
    packet[3] = 0x00;
    packet[4] = 0xfe;
    packet[5] = 0x00;
    packet[6] = 0x00;
    packet[7] = 0x00;
    packet[8] = 0x00;
    packet[9] = 0x00;
    packet[10] = 0x01;
    packet[11] = 0x00;
    packet[12] = addr0;
    packet[13] = addr1;
    packet[14] = addr2;
    packet[15] = addr3;
    packet[16] = 0x03;
    packet[17] = 0x00;
    packet[18] = cardno;
    packet[19] = 0x00;
    packet[20] = 0x00;

    unsigned int array_sum = calculateArraySum(packet, sizeof(packet)-2);
    calculateSumHL(array_sum, &sum_h, &sum_l);

    packet[21] = sum_l;
    packet[22] = sum_h;

    //ＴＣＰ连接方式，　需要修改客户端连接
    if (socketconnected(led_fd) != 0) 
    {
        write(led_fd, packet, sizeof(packet));
    }

    usleep(50 * 1000);

}




