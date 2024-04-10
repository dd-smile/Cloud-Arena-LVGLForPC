/*
 * @Description:
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-03-09 11:28:54
 * @LastEditTime: 2023-08-31 14:21:19
 * @FilePath: lv_node/src/setting/settings.c
 */

#include "ui_app.h"

//结构体数组，存放设置页面中的卡片
static const card_info_t CARD_INFO[] = {
    {20, 65, CreateAddDevice, NULL},                 // 20, 65,  增加设备卡片
    {310, 65, CreateScreen, add_Screen_event_cb},    // 310, 65,　屏幕设置卡片
    {600, 65, CreateTime, NULL},                     // 600，65,  设置时间卡片　　
    {20, 290, CreateScreensave, NULL},               // 20, 290,  屏保图片选择卡片
    {310, 290, CreateLanguage, NULL},                 // 310, 290,  语言设置卡片
    // {600, 290, WifiCard, NULL}
};


/**
 * 创建设置页面
 * @param parent         指向父对象的指针
 * */
void CreateSettingPage(lv_obj_t*parent)
{
    for (int i = 0; i < sizeof(CARD_INFO) / sizeof(CARD_INFO[0]); i++)
    {
        lv_obj_t *card = Createcard(parent);
        lv_obj_set_pos(card, CARD_INFO[i].x_pos, CARD_INFO[i].y_pos);  //设置坐标
        CARD_INFO[i].card_func(card);   //启用卡片功能
    }
}