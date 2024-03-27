/*
 * @Description:wifi页面
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-01-30 11:28:06
 * @LastEditTime: 2024-03-20 14:51:43
 * @FilePath: lv_node/src/setting/Wifi/WifiCard.c
 */

#include "ui_app.h"

WifiNode *wifiListHead = NULL;           // WIFI元素链表头
WifiInfo wifi_results[MAX_SCAN_RESULTS]; // 扫描结果  

static lv_obj_t *wifi_page = NULL;       // wifi界面
static lv_obj_t *WifiList = NULL;        // 其他WIFI列表
lv_obj_t *WifiList_connect = NULL;       // 已连接WIFI列表

WifiNode *wifi_has_been_connnected = NULL; // 已经连上的wifi
static bool wifi_switch = true;
static lv_style_t style_bg_mask;


static void CreatrWifiListButton(lv_obj_t *parent, char *text);

/**
 * 释放内存
*/
static void freeWifiList()
{
    //创建一个临时结构体，把当前wifi列表头部赋值给它
    WifiNode *current = wifiListHead;
    //如果赋值成功开始循环释放内存操作
    while (current != NULL)
    {
        WifiNode *next = current->next;
   
        free(current);
        current = next;
    }
    wifiListHead = NULL;//初始化wifi列表头部，等待下个列表重新初始化赋值
}

/**
 * 输入密码
 * @param e            指向事件描述符的指针
*/
static void TextWIFIClick(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        CreateWifiKeyboard(obj);
    }
}

/**
 * 创建WIFI列表UI界面
 * @param parent       指向父对象的指针
 * @param h            列表高度
 * @param y            y坐标对齐后的偏移量
 * @return             返回列表对象
*/
lv_obj_t *CreatrWifiList(lv_obj_t *parent, int h, int y)
{

    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, 980, h);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_set_style_bg_opa(list, 0, 0);
    lv_obj_set_style_border_opa(list, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(list, 0, LV_STATE_DEFAULT);
    return list;
}


/**
 * 创建wifi连接图标标签对象
 * @param parent           指向一个对象的指针，它将是新图像的父对象
 * @return                 创建的wifi连接图标标签对象
*/
static lv_obj_t *CreatrWifiJoinIcon(lv_obj_t *parent)
{
    lv_obj_t *JoinIcon = lv_label_create(parent);
    lv_label_set_text(JoinIcon, "");
    lv_obj_set_style_text_font(JoinIcon, &lv_font_montserrat_16, 0);
    lv_obj_align(JoinIcon, LV_ALIGN_RIGHT_MID, -10, 5);
    lv_obj_set_style_text_color(JoinIcon, lv_color_hex(0xffffff), 0);
    return JoinIcon;
}

/**
 * 创建wifi连接成功文本
 * @param parent           指向一个对象的指针，它将是新图像的父对象
 * @return                 创建的wifi连接成功标签对象        
*/
static lv_obj_t *CreatrWifiJoinTxet(lv_obj_t *parent)
{
    lv_obj_t *JoinTxet = lv_label_create(parent);
    lv_label_set_text(JoinTxet, "");
    lv_obj_set_style_text_font(JoinTxet, &PuHuiTi_Regular_16, 0);
    lv_obj_align(JoinTxet, LV_ALIGN_RIGHT_MID, -30, 5);
    lv_obj_set_style_text_color(JoinTxet, lv_color_hex(0xffffff), 0);
    return JoinTxet;
}

/**
 * 创建wifi名称的标签对象
 * @param parent           指向一个对象的指针，它将是新图像的父对象
 * @param text             wifi名称
 * @return                 创建的wifi名称标签对象
*/
static lv_obj_t *CreatrWifiWifiName(lv_obj_t *parent, char *text)
{
    lv_obj_t *WifiName = lv_label_create(parent);
    lv_label_set_text(WifiName, text);
    lv_obj_set_style_bg_opa(WifiName, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(WifiName, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(WifiName, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(WifiName, &PuHuiTi_Regular_16, LV_STATE_DEFAULT);
    lv_obj_align(WifiName, LV_ALIGN_LEFT_MID, 40, 5);
    lv_obj_set_style_text_color(WifiName, lv_color_hex(0xffffff), 0);
    return WifiName;
}


/**
 * 添加列表(WIFI列表模板)，组建好WIFI信息结构体
 * @param parent            指向一个对象的指针，它将是新图像的父对象
 * @param text              wifi的ssid名称
*/
static void addWifiInfoToList(lv_obj_t *parent, char *text)
{

    WifiNode *newNode = (WifiNode *)malloc(sizeof(WifiNode));
    //如果为空，表示创建失败
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    //进行WIFI信息结构体的组建，依次赋值
    strcpy(newNode->ssid, text);     //复制wifi的ssid名称到结构体中
    newNode->WifiName = CreatrWifiWifiName(parent, text);
    newNode->JoinTxet = CreatrWifiJoinTxet(parent);
    newNode->JoinIcon = CreatrWifiJoinIcon(parent);
    newNode->password_textarea = NULL;
    newNode->next = NULL;

    /*由wifi信息结构体组成的wifi链表，来表示wifi列表*/
    //如果链表第一个元素为null，那么把当前wifi信息结构体赋值给他，表示初始化链表
    if (wifiListHead == NULL)
    {
        wifiListHead = newNode;
    }
    else    //如果链表第一个元素不为null，那么把当前wifi信息结构体赋值到下一个元素
    {
        WifiNode *temp = wifiListHead;
        //如果链表第二个元素不为null，表示现在要赋值的元素是第三个.....以此类推
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    lv_obj_set_user_data(parent, newNode);   //设置用户数据(wifi信息)
}


/**
 * 扫描wifi列表，
*/
static void ScanningWifi()
{
    wifi_results->count = wifi_scan(wifi_results, MAX_SCAN_RESULTS); // 扫描WIFI

    //进行初始化操作
    lv_obj_del(WifiList);                 // 删除WIFI列表
    freeWifiList();                       // 释放WIFI列表
    WifiList = CreatrWifiList(wifi_page, 340, 140); // 创建WIFI列表

    //如果扫描到的wifi数量大于0，就进行创建对应的wifi列表按钮
    if (wifi_results->count > 0)
    {
        for (int i = 0; i < wifi_results->count; i++)
        {
            //如果wifi的ssid名称为空
            if (strlen(wifi_results[i].SSID) == 0)
            {
                continue; // 跳过空的SSID
            }
            char *converted_str = convert_unicode_str(wifi_results[i].SSID); // 转换SSID
            CreatrWifiListButton(WifiList, converted_str);                   // 创建WIFI列表按钮
            //printf("ssid:%s\n",converted_str);
            free(converted_str);
        }
    }
    else
    {
        printf("No Wi-Fi networks found.\n");
    }
    
}

/**
 * 创建WIFI列表对象按钮，同时绘制WIFI列表中每个对象按钮的WIFI图标
 * @param parent          指向一个对象的指针，它将是新图像的父对象     
 * @return                返回指向新创建WIFI列表按钮对象的指针                    
*/
lv_obj_t *CreatrWifiListStaticButton(lv_obj_t *parent)
{
    //列表对象按钮样式
    static lv_style_t style_wifi_list;
    lv_style_init(&style_wifi_list);
    lv_style_set_bg_opa(&style_wifi_list, LV_OPA_TRANSP);
    lv_style_set_bg_color(&style_wifi_list, lv_color_hex(0x4c4965));
    lv_style_set_border_width(&style_wifi_list, 0);
    lv_style_set_radius(&style_wifi_list, 0);
    lv_style_set_text_color(&style_wifi_list, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_wifi_list, &lv_font_montserrat_16);
    lv_style_set_text_align(&style_wifi_list, LV_TEXT_ALIGN_LEFT);
    lv_style_set_pad_right(&style_wifi_list, 10);
    lv_style_set_pad_all(&style_wifi_list, 20);
    lv_style_set_img_recolor(&style_wifi_list, lv_color_hex(0xffffff));

    lv_obj_t *ListButton = lv_list_add_btn(parent, NULL, NULL);
    lv_obj_remove_style_all(ListButton);
    lv_obj_set_size(ListButton, 920, 50);
    lv_obj_align(ListButton, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_style(ListButton, &style_wifi_list, LV_PART_MAIN);
    lv_obj_set_style_pad_all(ListButton, 15, LV_STATE_DEFAULT);
  
    //直线对象样式
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 1);
    lv_style_set_line_color(&style_line, lv_color_hex(0xCCCCCC));
    lv_style_set_bg_color(&style_line, lv_color_hex(0xCCCCCC));
    lv_style_set_line_opa(&style_line, LV_OPA_50);
    lv_obj_t *line = lv_line_create(parent);
    static const lv_point_t line_points1[] = {{5, 0}, {920, 0}};
    lv_line_set_points(line, line_points1, 2);
    lv_obj_align(line, LV_ALIGN_BOTTOM_MID, 40, 0);
    lv_obj_add_style(line, &style_line, LV_PART_MAIN);

    //WIFI图标
    lv_obj_t *WifiIcon = lv_label_create(ListButton);
    lv_label_set_text(WifiIcon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(WifiIcon, &lv_font_montserrat_16, 0);
    lv_obj_align(WifiIcon, LV_ALIGN_LEFT_MID, 0, 5);
    lv_obj_set_style_text_color(WifiIcon, lv_color_hex(0xffffff), 0);
    return ListButton;
}

/**
 * 创建其他wifi列表按钮
 * @param parent                指向一个对象的指针，它将是新图像的父对象
 * @param text                  wifi的ssid名称
*/
static void CreatrWifiListButton(lv_obj_t *parent, char *text)
{
    lv_obj_t *ListButton = CreatrWifiListStaticButton(parent);
    lv_obj_add_event_cb(ListButton, TextWIFIClick, LV_EVENT_CLICKED, NULL);
    addWifiInfoToList(ListButton, text);
}


/**
 * 获取已连上的wifi列表结构体
 * @param listButton            WIFI列表对象按钮
 * @return                      返回已连上的wifi列表结构体
*/
WifiNode *getWifiNodeFromListButton(lv_obj_t *listButton)
{
    return (WifiNode *)lv_obj_get_user_data(listButton);
}

/**
 * 搜索wifi的定时器
 * @param timer                 创建的定时器
*/
void search_wifi(lv_timer_t *timer)
{
    //如果其他WIFI列表对象不为空，进行扫描附近WIFI信息
    if(WifiList != NULL)
    {
        ScanningWifi();      
    }
    lv_timer_del(timer);
}


/**
 * 断开当前wifi确认按键
 * @param e                      指向事件描述符的指针
*/
static void disconnect_confirm(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        Disconnect_wifi();
        //删除已连接WIFI列表对象，重新初始化WIFI列表对象，重新绘制已连接WIFI的UI界面
        lv_obj_del(WifiList_connect);
        WifiList_connect = CreatrWifiList(wifi_page, 55, 35);
        // 重新搜索wifi
        lv_timer_create(search_wifi, 500, NULL);
        
        //释放同时初始化已连接WIFI信息结构体
        free(wifi_has_been_connnected);
        wifi_has_been_connnected = NULL;

        lv_obj_del(obj->parent->parent);
    }
}

/**
 * 初始化遮蔽罩风格
 * @param obj              要初始化的对象
*/
static void lv_common_style_set_style_mask(lv_obj_t *obj)
{
    lv_style_init(&style_bg_mask); // 初始化遮蔽罩风格
    lv_style_set_bg_color(&style_bg_mask, lv_color_hex(0x11141A));
    lv_style_set_bg_opa(&style_bg_mask, 200);
    lv_obj_add_style(obj, &style_bg_mask, LV_PART_MAIN);
}

/**
 * 创建遮罩层
 * @param parent           指向一个对象的指针，它将是新图像的父对象
 * @param back             返回标志位,如果为true,为遮罩层添加返回事件
 * @return                 返回指向遮罩层对象
*/
lv_obj_t *lv_common_create_mask_box(lv_obj_t *parent, bool back)
{
    lv_obj_t *gray_layer = lv_obj_create(parent); // 主页面灰色遮罩层
    lv_obj_remove_style_all(gray_layer);
    lv_obj_set_pos(gray_layer, 0, 0);
    lv_obj_set_size(gray_layer, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_common_style_set_style_mask(gray_layer);
    if (back)
    {
        lv_obj_add_event_cb(gray_layer, lv_back_event, LV_EVENT_CLICKED, NULL); // 添加返回事件
    }

    return gray_layer;
}

/**
 * 创建一个wifi断开连接弹出框
 * @param parent                     指向一个对象的指针，它将是新图像的父对象
 * @param card_back                  返回标志位,如果为true,为遮罩层添加返回事件
 * @param title                      弹出框标题
 * @param confirm_text               确认按钮文本
 * @param back_text                  返回按钮文本
 * @param text                       弹出框的提示信息
 * @param event_cb                   确认按钮的执行事件
 * @param user_data                  要传入的用户数据
 * @return                           返回指向弹出框的对象
*/
static lv_obj_t *create_wifi_popup(lv_obj_t *parent, bool card_back, const char *title, const char *confirm_text, const char *back_text, const char *text, lv_event_cb_t event_cb, void *user_data)
{
    lv_obj_t *mask = lv_common_create_mask_box(parent, card_back); // 创建遮罩
    lv_obj_t *popup = lv_obj_create(mask);
    lv_obj_set_size(popup,581,358);
    lv_obj_align(popup,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_bg_color(popup, lv_color_hex(0x252c39), 0);
    lv_obj_set_style_border_width(popup, 0, LV_PART_MAIN);


    lv_obj_t *label = lv_label_create(popup);                 // 创建标题
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_30, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *confirm_btn = btn_create_text(popup, false, confirm_text, 155, 265); // 创建按钮
    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);       // 添加确认按钮的执行事件
    btn_create_text(popup, true, back_text, 305, 265);                             // 创建按钮
    card_create_24_text(popup, text, 0, 0);

    return popup;
}

/**
 * 断开wifi连接事件
 * @param e           指向事件描述符的指针
*/
static void wifi_disconnect_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        create_wifi_popup(lv_scr_act(), false, "温馨提示", "确定", "取消", "是否断开连接", disconnect_confirm, NULL);
    }
}


/**
 * 创建已连接的wifi按钮
 * @param parent       指向一个对象的指针，它将是新图像的父对象
 * @param text         wifi的ssid名称    
*/
void CreatrWifiConnectButton(lv_obj_t *parent, char *text)
{
    lv_obj_t *ListButton = CreatrWifiListStaticButton(parent);
    lv_obj_add_event_cb(ListButton, wifi_disconnect_event, LV_EVENT_CLICKED, NULL);
    addWifiInfoToList(ListButton, text);

    WifiNode *node = getWifiNodeFromListButton(ListButton);//获取已连上的wifi
    lv_label_set_text(node->JoinIcon, LV_SYMBOL_OK);
    lv_label_set_text(node->JoinTxet, "已连接"); 

    wifi_has_been_connnected = (WifiNode *)malloc(sizeof(WifiNode));  //创建一块内存用于存储已连接wifi的信息
    memcpy(wifi_has_been_connnected, node, sizeof(WifiNode));
}

/**
 * 创建WiFi列表界面背景
 * @param parent        指向一个对象的指针，它将是新图像的父对象
 * @return              返回指向wifi列表对象
*/
lv_obj_t *create_wifi_page(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_popup_page(scr, "WIFI设置", 0, 0);

    lv_obj_t *page = lv_obj_create(popup_page); 
    lv_obj_set_size(page, 1024, 530);
    lv_obj_align(page, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(page, 0, 0);
    lv_obj_set_style_border_opa(page, 0, 0);

    lv_obj_t *text1 = card_create_20_text(page, "当前已连接网络", 0, 0);
    lv_obj_align(text1, LV_ALIGN_TOP_LEFT, 10, 0);
    lv_obj_t *text2 = card_create_20_text(page, "其他网络", 0, 0);
    lv_obj_align(text2, LV_ALIGN_TOP_LEFT, 10, 100);

    return page;
}



/**
 * 创建WIFI页面(包括已连接wifi，其他wifi)
 * 创建完页面后开启定时器扫描wifi
*/
void CreateWifiPage(void)
{

    wifi_page = create_wifi_page(lv_scr_act());

    // 已连接wifi
    WifiList_connect = CreatrWifiList(wifi_page, 55, 35);
    if(wifi_has_been_connnected != NULL)    //如果已连接wifi不为空
    {
        CreatrWifiConnectButton(WifiList_connect, wifi_has_been_connnected->ssid);         //创建可点击的已连接wifi信息
    }

    // 其他wifi
    WifiList = CreatrWifiList(wifi_page, 340, 140);

    // 搜索wifi
    lv_timer_create(search_wifi, 500, NULL);
}

/**
 * 查询当前连接状态与可供连接wifi的信息
 * @param e             指向事件描述符的指针
*/
void add_Wifi_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        CreateWifiPage();
    }
}


/**
 * 创建WIFI界面的卡片
 * @param parent         指向父对象的指针
 * */
void WifiCard(lv_obj_t *parent)
{

    lv_obj_t *ImageObj = CreateOBJclick(parent);
    card_create_24_text(parent, "WIFI", 2, -30);               // 创建标题
    lv_obj_add_event_cb(ImageObj, add_Wifi_event_cb, LV_EVENT_ALL, NULL);

}
