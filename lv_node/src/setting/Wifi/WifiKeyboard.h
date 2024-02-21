/*
 * @Description:输入WIFI密码界面
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-01-30 11:28:06
 * @LastEditTime: 2024-03-20 14:51:43
 * @FilePath: lv_node/src/setting/Wifi/WifiKeyboard.h
 */
#ifndef __WIFIKEYBOARD_H__
#define __WIFIKEYBOARD_H__
#include "lvgl/lvgl.h"
// Define the maximum length for SSID and password
#define SSID_MAX_LENGTH     64
#define PASSWORD_MAX_LENGTH 64
#define GET_LABEL_TEXT(parent) lv_label_get_text(lv_obj_get_child(parent, 1))



typedef struct {
    char ssid[SSID_MAX_LENGTH];  // Wi-Fi name (SSID) string
    char password[PASSWORD_MAX_LENGTH];  // Wi-Fi password string
} WifiCredentials;


void CreateWifiKeyboard(lv_obj_t *parent);


#endif // LV_WIFI_KEYBOARD_H