/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:54
 * @LastEditTime: 2023-07-03 14:21:19
 * @FilePath: /new_home_smart/lv_node/src/settings.c
 */

#include "ui_app.h"

static const card_info_t CARD_INFO[] = {
    {20, 65, CreateAddDevice,NULL},                 // 20, 65,
    {310, 65, CreateScreen,add_Screen_event_cb},    // 310, 65,
    {600, 65, CreateTime,NULL},                     // 600，65
    {20, 290, CreateScreensave,NULL},               // 20, 290,
    {310, 290, CreateLanguage,NULL}                 // 310, 290,

};

/*------------------------------设置页面----------------------------------*/

void CreateSettingPage(lv_obj_t*parent)
{
    for (int i = 0; i < sizeof(CARD_INFO) / sizeof(CARD_INFO[0]); i++)
    {
        lv_obj_t *card = Createcard(parent);
        lv_obj_set_pos(card, CARD_INFO[i].x_pos, CARD_INFO[i].y_pos);
        CARD_INFO[i].card_func(card);
    }
}