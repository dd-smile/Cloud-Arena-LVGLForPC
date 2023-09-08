/**
 * @file lv_tileview.h
 *
 */

#ifndef LV_TILEVIEW_H
#define LV_TILEVIEW_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../core/lv_obj.h"

#if LV_USE_TILEVIEW

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_obj_t obj;
    lv_obj_t * tile_act;
} lv_tileview_t;

typedef struct {
    lv_obj_t obj;
    lv_dir_t dir;
} lv_tileview_tile_t;

extern const lv_obj_class_t lv_tileview_class;
extern const lv_obj_class_t lv_tileview_tile_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a Tileview object 创建一个平铺视图对象
 * @param parent pointer to an object, it will be the parent of the new tileview 指向一个对象的指针，它将是新平铺视图的父类
 * @return pointer to the created tileview 指向创建的平铺视图的指针
 */
lv_obj_t * lv_tileview_create(lv_obj_t * parent);                 /*创建 object*/

/**
 * 向 tileview 添加一个新的 tile，并指定其位置和滚动方向
 * @param tv              这是 tileview 对象的指针
 * @param col_id          这是新 tile 的列索引
 * @param row_id          这是新 tile 的行索引
 * @param dir             这定义了从这个 tile 能够滚动到的方向。lv_dir_t 是一个枚举类型，其中的值可以是 LV_DIR_LEFT、LV_DIR_RIGHT、LV_DIR_TOP 和 LV_DIR_BOTTOM，这些值可以组合使用，例如 LV_DIR_LEFT | LV_DIR_RIGHT 会允许从 tile 向左和向右滚动。
 * @return                返回一个指向新 tile 的指针
 */
lv_obj_t * lv_tileview_add_tile(lv_obj_t * tv, uint8_t col_id, uint8_t row_id, lv_dir_t dir);       

void lv_obj_set_tile(lv_obj_t * tv, lv_obj_t * tile_obj, lv_anim_enable_t anim_en);
void lv_obj_set_tile_id(lv_obj_t * tv, uint32_t col_id, uint32_t row_id, lv_anim_enable_t anim_en);

/**
 * 查询当前活跃的 tile 在 tileview 中的对象。这是获取当前显示的 tile 引用的简便方法。
 * @param obj             这是 tileview 对象的指针
 * @return                返回值是当前活跃（或说是正在显示）的 tile 对象的指针
 */
lv_obj_t * lv_tileview_get_tile_act(lv_obj_t * obj);

/*=====================
 * Other functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_TILEVIEW*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_TILEVIEW_H*/
