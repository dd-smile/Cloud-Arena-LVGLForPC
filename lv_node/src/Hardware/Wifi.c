#include "ui_app.h"
#include "lv_node/src/Hardware/Wifi.h"

// WLAN_HANDLE g_wlanHdl = -1;                                                // wifi句柄
// static MI_WLAN_InitParams_t g_stParam = {"/config/wifi/wlan.json"};        // wifi配置文件路径
// static MI_WLAN_OpenParams_t g_stOpenParam = {E_MI_WLAN_NETWORKTYPE_INFRA}; // wifi模式
// MI_WLAN_ScanParam_t *pstParam;
// MI_WLAN_Status_t status;
bool g_bConnected = false; // 是否连接上wifi
pthread_t g_ptConn;        // 连接线程句柄
bool g_bThreadRun = false; // 连接线程是否运行

static void *ConnectWorkThread(void *args)
{
    // MI_WLAN_ConnectParam_t g_stConnectParam;

    // // 获取wifi配置信息
    // WifiNode *node = (WifiNode *)args;

    // WifiNode *temp = wifiListHead;

    // strncpy((char *)g_stConnectParam.au8SSId, node->ssid, sizeof(node->ssid));
    // g_stConnectParam.au8SSId[sizeof(g_stConnectParam.au8SSId) - 1] = '\0';

    // strncpy((char *)g_stConnectParam.au8Password, node->password, sizeof(node->password));
    // g_stConnectParam.au8Password[sizeof(g_stConnectParam.au8Password) - 1] = '\0';

    // printf("g_stConnectParam.au8SSId: %s\n", (char *)g_stConnectParam.au8SSId);
    // printf("g_stConnectParam.au8Password: %s\n", (char *)g_stConnectParam.au8Password);

    // g_stConnectParam.OverTimeMs = 5000;
    // g_stConnectParam.eSecurity = E_MI_WLAN_SECURITY_WPA;

    // MI_WLAN_Connect(&g_wlanHdl, &g_stConnectParam); // 连接wifi

    // bool connected = false;
    // int retry_count = 30; // Retry for a maximum of 30 seconds (30 * 100ms)
    // while (g_bThreadRun && retry_count > 0)
    // {
    //     usleep(100 * 1000);                    // 休眠100毫秒
    //     MI_WLAN_GetStatus(g_wlanHdl, &status); // 重新获取wifi状态

    //     switch (status.stStaStatus.state)
    //     {
    //     case WPA_DISCONNECTED:
    //         printf("Wi-Fi State: 断开连接\r\n");
    //         break;
    //     case WPA_INTERFACE_DISABLED:
    //         printf("Wi-Fi State: 网络接口禁用\r\n");
    //         break;
    //     case WPA_INACTIVE:
    //         printf("Wi-Fi State: 不活跃（wpa_supplicant禁用）\r\n");
    //         break;
    //     case WPA_SCANNING:
    //         printf("Wi-Fi State: 扫描网络\r\n");
    //         lv_label_set_text(node->JoinTxet, "正在连接..."); // Clear connection status
    //         break;
    //     case WPA_AUTHENTICATING:
    //         printf("Wi-Fi State: 正在验证\r\n");
    //         lv_label_set_text(node->JoinTxet, "正在验证..."); // Clear connection status
    //         break;
    //     case WPA_ASSOCIATING:
    //         printf("Wi-Fi State: 正在关联\r\n");
    //         lv_label_set_text(node->JoinTxet, "正在连接..."); // Clear connection status
    //         break;
    //     case WPA_ASSOCIATED:
    //         printf("Wi-Fi State: 关联完成\r\n");
    //         lv_label_set_text(node->JoinTxet, "获取IP地址..."); // Clear connection status
    //         break;
    //     case WPA_4WAY_HANDSHAKE:
    //         printf("Wi-Fi State: WPA 4路密钥握手正在进行中\r\n");
    //         break;
    //     case WPA_GROUP_HANDSHAKE:
    //         printf("Wi-Fi State: WPA组密钥握手正在进行中\r\n");
    //         break;
    //     case WPA_COMPLETED:
    //         printf("Wi-Fi State: 连接成功\r\n");
    //         connected = true;
    //         break;
    //     default:
    //         break;
    //     }

    //     if (connected)
    //     {
    //         break; // Exit the loop if connected successfully
    //     }

    //     retry_count--;
    // }

    // if (connected)
    // { // 连接成功
    //     g_bConnected = true;
    //     printf("Wi-Fi Connected Successfully\r\n");
    //     lv_label_set_text(node->JoinIcon, LV_SYMBOL_OK);
    //     lv_label_set_text(node->JoinTxet, "已连接"); // Clear connection status

    //     while (temp != NULL)
    //     {
    //         if (temp != node)
    //         {
    //             lv_label_set_text(temp->JoinIcon, ""); // Clear connection status
    //             lv_label_set_text(temp->JoinTxet, ""); // Clear connection status
    //         }
    //         temp = temp->next;
    //     }
    //     // 创建已连接的wifi列表按钮
    //     CreatrWifiConnectButton(WifiList_connect, node->ssid);
    // }
    // else
    // {
    //     lv_label_set_text(node->JoinTxet, "连接失败");
    //     sleep(2);
    //     lv_label_set_text(node->JoinTxet, "");
    //     printf("Wi-Fi Connect Failed\r\n");
    // }

    // while (g_bThreadRun && connected)
    // {
    //     usleep(100 * 1000);                    // 休眠100毫秒
    //     MI_WLAN_GetStatus(g_wlanHdl, &status); // 重新获取wifi状态

    //     if (status.stStaStatus.state == WPA_DISCONNECTED)
    //     {
    //         printf("Wi-Fi State: 断开连接\r\n");
    //         lv_label_set_text(node->JoinTxet, ""); // Clear connection status
    //         lv_label_set_text(node->JoinIcon, ""); // Clear connection status
    //         g_bConnected = false;
    //         break;
    //     }
    // }

    // return NULL;
}
void init_wifi(void) // 初始化wifi
{
    // MI_WLAN_Init(&g_stParam); // 初始化wifi
    // MI_WLAN_Open(&g_stOpenParam);
}

void connect_wifi(WifiNode *node) // 连接线程
{
    // system("ifconfig wlan0 up");
    // g_bConnected = false;
    // g_bThreadRun = true;
    // pthread_create(&g_ptConn, NULL, ConnectWorkThread, node);
}

void Disconnect_wifi(void) // 关闭连接
{
    // g_bThreadRun = false;
    // if (g_bConnected)
    // {
    //     MI_WLAN_Disconnect(g_wlanHdl); // 断开连接
    //     g_bConnected = false;
    // }
}
