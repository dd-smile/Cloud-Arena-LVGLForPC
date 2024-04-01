/*
 * @Description:wifi底层通用代码
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-01-30 11:28:06
 * @LastEditTime: 2024-03-20 14:51:43
 * @FilePath: lv_node/src/Hardware/wifi_recv.h
 */
#ifndef __WIFI_RECV_H__
#define __WIFI_RECV_H__

#define MAX_SCAN_RESULTS 64 // 定义扫描Wi-Fi网络的最大数量 

// Structure to hold Wi-Fi information
typedef struct WifiInfo
{
    char SSID[128];     //wifi的ssid名称(未经转换的)
    char BSSID[64];
    char channel[64];
    char signal_strength[64];
    int count;      //扫描到的wifi数量
}WifiInfo;

void *ScanningWifiThread(void *args);
char* get_connected_wifi_name();
char *convert_unicode_str(const char *str);

int wifi_scan(WifiInfo *wifi_results, int max_results);
extern  WifiInfo wifi_results[MAX_SCAN_RESULTS]; // Array to hold the results of the Wi-Fi scan
#endif