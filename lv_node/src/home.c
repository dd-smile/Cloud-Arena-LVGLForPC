/*
 * @Description: 创建主页
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-03-09 11:28:17
 * @LastEditTime: 2023-08-23 15:59:05
 * @FilePath: /new_home_smart/lv_node/src/home.c
 */

#include "ui_app.h"
HomeData device_data;

/**
 * 滚动容器事件
 * @param e           指向事件描述符的指针     
 * */
static void scroll_begin_event(lv_event_t *e)
{

    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        lv_anim_t *a = lv_event_get_param(e);
        if (a)
            a->time = 0;
    }
}

/**
 * 创建滚动容器
 * @param obj              已创建的标签视图对象 
 * @param name             新标签的标题，即标签上显示的文本
 * @return                 返回指向标签页面的指针
 * */
static lv_obj_t *page_card(lv_obj_t *obj, char *name)
{
    lv_obj_t *card = lv_tabview_add_tab(obj, name);       //为已经创建的标签视图添加新标签，并为其指定一个标题
    lv_obj_t *Group_navigation = lv_tileview_create(card);     //以标签页面为类，创建平铺试图
    lv_obj_set_style_bg_opa(Group_navigation, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_width(Group_navigation, 0, LV_PART_SCROLLBAR);
    return Group_navigation;
}

/**
 * 创建滑动页面
 * @param obj              指向一个对象的指针，它将是新标签视图的父类
 * @param num_pages        标签页面数量 
 * @param tab_names        一个以NULL结束的字符串数组，表示要为标签视图创建的标签的文本
 * @return                 返回标签视图中的内容部分的对象指针
 * */
static lv_obj_t *TabviewDevicePage(lv_obj_t *obj, int num_pages, const char **tab_names)
{
    lv_obj_t *tabview = lv_tabview_create(obj, LV_DIR_TOP, 30);      //创建一个标签视图
    lv_obj_clear_flag(tabview, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(tabview, lv_pct(100), lv_pct(100));
    lv_obj_align(tabview, LV_ALIGN_CENTER, -10, 0);
    lv_obj_set_style_bg_opa(tabview, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(lv_tabview_get_content(tabview), scroll_begin_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabview);      //获取标签视图中的标签按钮部分
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

    lv_obj_t *home_tab = lv_tabview_get_content(tabview);    //获取标签视图中的标签内容部分

    return home_tab;
}

/**
 * 创建滚动页面跟导航栏
 * @param parent          指向父对象的指针
 * @param num_pages       可变参数的数量
 * @return                返回指向滑动页面的指针          
 * */
lv_obj_t *device_page_box(lv_obj_t *parent, int num_pages, ...)
{
    const char *tab_names[num_pages];
    va_list args;
    va_start(args, num_pages);  //这个宏初始化一个 va_list 类型的变量，用于后续的 va_arg 调用。
    for (int i = 0; i < num_pages; i++)
    {
          /*该宏用于访问可变参数列表中的参数。它需要两个参数：一个 va_list 和想要访问的参数的类型。
        每次调用 va_arg 时，它都会返回当前参数并将内部的指针移到下一个参数。*/
        tab_names[i] = va_arg(args, const char *);     //读取可变参数，把他存储在tab_names里面                                     
    }
    va_end(args);  //在完成所有的 va_arg 调用后，必须使用 va_end 来清理为 va_list 分配的内存。

    lv_obj_t *home_tab = TabviewDevicePage(parent, num_pages, tab_names); // 创建滑动页面

    return home_tab;
}

/**
 * 创建主页
 * @param parent          指向父对象的指针   
 * */
void CreateHomePage(lv_obj_t *parent)
{
    lv_obj_t *home_tab = device_page_box(parent, 1, "Facilities"); // 设备列框架

    device_data.all_pages = lv_tileview_add_tile(lv_obj_get_child(home_tab, 0), 0, 0, LV_DIR_RIGHT); // 添加全部页面

    CreateDevicePage(device_data.all_pages);
}