
#ifndef __WIFI_H__
#define __WIFI_H__
#include "lv_node/src/setting/WIFI/WifiCard.h"
#include "lv_node/src/setting/WIFI/WifiKeyboard.h"

 void init_wifi(void); // 初始化wifi
 void Disconnect_wifi(void); // 断开wifi
 void connect_wifi(WifiNode *node); // 连接线程
 

#endif
