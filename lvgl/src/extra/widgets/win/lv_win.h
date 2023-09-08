/**
 * @file lv_win.h
 *
 */

#ifndef LV_WIN_H
#define LV_WIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_obj_t obj;
} lv_win_t;

extern const lv_obj_class_t lv_win_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * 创建窗口，提供了一个带有标题和可选操作按钮的容器
 * @param parent           这是窗口对象的父对象。通常，如果你想在主屏幕上创建窗口，你可以使用lv_scr_act()作为父对象。
 * @param header_height    窗口标题栏的高度。如果你传递LV_SIZE_CONTENT，则标题的高度将根据其内容（标题文本和按钮）自动设置。
 * @return                 返回一个指向新创建的窗口对象的指针       
 * */
lv_obj_t * lv_win_create(lv_obj_t * parent, lv_coord_t header_height);


lv_obj_t * lv_win_add_title(lv_obj_t * win, const char * txt);
lv_obj_t * lv_win_add_btn(lv_obj_t * win, const void * icon, lv_coord_t btn_w);

lv_obj_t * lv_win_get_header(lv_obj_t * win);

/**
 * 窗口对象由三个主要部分组成：标题栏、内容区域和一个可选的底部工具栏。
 * 当你想在窗口内部添加其他对象（如标签、按钮等）时，你应该将它们添加到内容区域而不是直接添加到窗口对象本身。
 * 为了做到这一点，你需要获取窗口的内容区域的引用，这正是lv_win_get_content函数的作用。
 * 用于获取窗口对象的内容区域。
 * @param win             窗口对象的指针
 * @return                返回一个指向窗口的内容区域的指针              
 * */
lv_obj_t * lv_win_get_content(lv_obj_t * win);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_WIN_H*/
