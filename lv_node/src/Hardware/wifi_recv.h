#ifndef __WIFI_RECV_H__
#define __WIFI_RECV_H__

#define MAX_SCAN_RESULTS 64 // Define the maximum number of Wi-Fi networks to scan

// Structure to hold Wi-Fi information
typedef struct WifiInfo
{
    char SSID[128];
    char BSSID[64];
    char channel[64];
    char signal_strength[64];
    int count;
}WifiInfo;

void *ScanningWifiThread(void *args);
char* get_connected_wifi_name();
char *convert_unicode_str(const char *str);

int wifi_scan(WifiInfo *wifi_results, int max_results);
extern  WifiInfo wifi_results[MAX_SCAN_RESULTS]; // Array to hold the results of the Wi-Fi scan
#endif