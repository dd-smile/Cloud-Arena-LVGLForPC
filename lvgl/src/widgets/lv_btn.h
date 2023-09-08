/**
 * @file lv_btn.h
 *
 */

#ifndef LV_BTN_H
#define LV_BTN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_conf_internal.h"

#if LV_USE_BTN != 0
#include "../core/lv_obj.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;
} lv_btn_t;

extern const lv_obj_class_t lv_btn_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a button object 创建一个按钮对象
 * @param parent    pointer to an object, it will be the parent of the new button 指向一个对象的指针，它将是新按钮的父节点 
 * @return          pointer to the created button 指向已创建按钮的指针
 */
lv_obj_t * lv_btn_create(lv_obj_t * parent);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_BTN*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_BTN_H*/
