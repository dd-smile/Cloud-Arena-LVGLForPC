#ifndef _LV_NODE_HOME_H
#define _LV_NODE_HOME_H

#define LV_CARD_WIDTH 221      //卡片宽度
#define LV_CARD_HEIGHT 250     //卡片高度

typedef struct
{
    lv_obj_t *home_button[3]; // 主页按钮
    lv_obj_t *home_pages[3];  // 主页页面
    lv_obj_t *content;        // 主页内容

    lv_obj_t *home_page;    // 首页页面
    lv_obj_t *mode_page;    // 模式页面
    lv_obj_t *setting_page; // 设置页面


    lv_obj_t *mode_page_box; // 首页页面盒子

} All_Data;

extern All_Data home_data;

void create_lv_layout(lv_obj_t *scr);

#endif