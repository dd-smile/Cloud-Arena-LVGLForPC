/*
 * @Description:wifi底层功能代码
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-01-30 11:28:06
 * @LastEditTime: 2024-03-20 14:51:43
 * @FilePath: lv_node/src/Hardware/Wifi.h
 */

#ifndef __WIFI_H__
#define __WIFI_H__
#include "lv_node/src/setting/WIFI/WifiCard.h"
#include "lv_node/src/setting/WIFI/WifiKeyboard.h"

 void init_wifi(void); // 初始化wifi
 void Disconnect_wifi(void); // 断开wifi
 void connect_wifi(WifiNode *node); // 连接线程
 

#endif
