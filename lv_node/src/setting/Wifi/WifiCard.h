/*
 * @Description:wifi页面
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-01-30 11:28:06
 * @LastEditTime: 2024-03-20 14:51:43
 * @FilePath: lv_node/src/setting/Wifi/WifiCard.h
 */
#ifndef __WIFI_CARD_H__
#define __WIFI_CARD_H__

#include "lv_node/src/setting/WIFI/WifiKeyboard.h"

void WifiCard(lv_obj_t *parent);

typedef struct wifi_node
{
    lv_obj_t *WifiName;     //wifi名称
    lv_obj_t *JoinTxet;     //wifi连接文本（是否已连接)
    lv_obj_t *JoinIcon;     //wifi连接图标（连接成功显示)
    lv_obj_t *password_textarea;
    char ssid[SSID_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
    struct wifi_node *next;
} WifiNode;

WifiNode *getWifiNodeFromListButton(lv_obj_t *listButton);
void CreatrWifiConnectButton(lv_obj_t *parent, char *text);

extern WifiNode *wifiListHead;
extern lv_obj_t *WifiList_connect;
#endif