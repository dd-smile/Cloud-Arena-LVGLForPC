/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:17
 * @LastEditTime: 2023-07-03 15:59:05
 * @FilePath: /new_home_smart/lv_node/src/home.c
 */

#include "ui_app.h"
HomeData device_data;

/*容器事件*/
static void scroll_begin_event(lv_event_t *e)
{

    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        lv_anim_t *a = lv_event_get_param(e);
        if (a)
            a->time = 0;
    }
}

/*创建滚动容器*/
static lv_obj_t *page_card(lv_obj_t *obj, char *name)
{
    lv_obj_t *card = lv_tabview_add_tab(obj, name);
    lv_obj_t *Group_navigation = lv_tileview_create(card);
    lv_obj_set_style_bg_opa(Group_navigation, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_width(Group_navigation, 0, LV_PART_SCROLLBAR);
    return Group_navigation;
}

/*创建滑动页面*/
static lv_obj_t *TabviewDevicePage(lv_obj_t *obj, int num_pages, const char **tab_names)
{
    lv_obj_t *tabview = lv_tabview_create(obj, LV_DIR_TOP, 30);
    lv_obj_clear_flag(tabview, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(tabview, lv_pct(100), lv_pct(100));
    lv_obj_align(tabview, LV_ALIGN_CENTER, -10, 0);
    lv_obj_set_style_bg_opa(tabview, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(lv_tabview_get_content(tabview), scroll_begin_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabview);
    lv_obj_set_style_bg_opa(tab_btns, 0, LV_STATE_DEFAULT);
    lv_obj_remove_style_all(tab_btns);
    // lv_obj_set_width(tab_btns, 480);
    lv_obj_set_width(tab_btns, 150);
    lv_obj_set_height(tab_btns, 65);

    lv_obj_set_style_text_font(tab_btns, &PuHuiTi_Regular_20, 0);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_BOTTOM, LV_PART_ITEMS);
    lv_obj_set_style_border_color(tab_btns, lv_color_hex(0x14E7F0), LV_PART_ITEMS);
    lv_obj_set_style_border_width(tab_btns, 3, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_clear_flag(tab_btns, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_scrollbar_mode(lv_tabview_get_content(tabview), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(tab_btns, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(lv_tabview_get_content(tabview), 0, LV_STATE_DEFAULT);

    for (int i = 0; i < num_pages; i++)
    {
        page_card(tabview, tab_names[i]);
    }

    lv_obj_t *home_tab = lv_tabview_get_content(tabview);

    return home_tab;
}
/*创建滚动页面跟导航栏*/
lv_obj_t *device_page_box(lv_obj_t *parent, int num_pages, ...)
{
    const char *tab_names[num_pages];
    va_list args;
    va_start(args, num_pages);
    for (int i = 0; i < num_pages; i++)
    {
        tab_names[i] = va_arg(args, const char *);
    }
    va_end(args);

    lv_obj_t *home_tab = TabviewDevicePage(parent, num_pages, tab_names); // 创建滑动页面

    return home_tab;
}

void CreateHomePage(lv_obj_t *parent)
{
    lv_obj_t *home_tab = device_page_box(parent, 1, "Facilities"); // 设备列框架

    device_data.all_pages = lv_tileview_add_tile(lv_obj_get_child(home_tab, 0), 0, 0, LV_DIR_RIGHT); // 全部

    CreateDevicePage(device_data.all_pages);
}