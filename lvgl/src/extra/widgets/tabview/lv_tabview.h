/**
 * @file lv_templ.h
 *
 */

#ifndef LV_TABVIEW_H
#define LV_TABVIEW_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"

#if LV_USE_TABVIEW

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;
    char ** map;
    uint16_t tab_cnt;
    uint16_t tab_cur;
    lv_dir_t tab_pos;
} lv_tabview_t;

extern const lv_obj_class_t lv_tabview_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * 创建一个标签视图（TabView）。标签视图允许你在多个页面之间切换，每个页面都有一个对应的标签。你可以把它想象成计算机应用程序中常见的选项卡界面，或者是移动应用中底部的导航栏。
 * @param parent     指向一个对象的指针，它将是新标签视图的父类
 * @param tab_pos    标签的位置。lv_dir_t是一个枚举，其值可以是如下之一：LV_DIR_TOP：标签在上方,LV_DIR_RIGHT：标签在右侧,LV_DIR_BOTTOM：标签在下方,LV_DIR_LEFT：标签在左侧
 * @param tab_size   标签的大小，通常以像素点为单位
 * @return           指向创建的标签视图的指针
 */
lv_obj_t * lv_tabview_create(lv_obj_t * parent, lv_dir_t tab_pos, lv_coord_t tab_size);

/**
 * 为已经创建的标签视图添加新标签，并为其指定一个标题
 * @param tv         已创建的标签视图对象
 * @param name       新标签的标题，即标签上显示的文本
 * @return           函数返回一个对象指针，该指针代表新创建的标签页。你可以将其他控件（例如标签、按钮等）添加到此标签页中
 */
lv_obj_t * lv_tabview_add_tab(lv_obj_t * tv, const char * name);

/**
 * 获取标签视图中的内容部分。需要知道标签视图主要由两个部分组成：标签按钮部分（也就是用户点击以切换标签页的部分）和内容部分（显示每个标签内容的部分）。
 * @param tv         要查询的标签视图对象
 * @return           返回标签视图中的内容部分的对象指针
 */
lv_obj_t * lv_tabview_get_content(lv_obj_t * tv);

/**
 * 获取标签视图中的标签按钮部分。需要知道标签视图主要由两个部分组成：标签按钮部分（也就是用户点击以切换标签页的部分）和内容部分（显示每个标签内容的部分）。
 * @param tv         要查询的标签视图对象
 * @return           返回标签视图中的标签按钮部分的对象指针
 */
lv_obj_t * lv_tabview_get_tab_btns(lv_obj_t * tv);

void lv_tabview_set_act(lv_obj_t * obj, uint32_t id, lv_anim_enable_t anim_en);

uint16_t lv_tabview_get_tab_act(lv_obj_t * tv);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_TABVIEW*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_TABVIEW_H*/
