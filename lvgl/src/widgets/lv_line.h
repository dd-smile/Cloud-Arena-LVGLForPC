/**
 * @file lv_line.h
 *
 */

#ifndef LV_LINE_H
#define LV_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_conf_internal.h"

#if LV_USE_LINE != 0

#include "../core/lv_obj.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/*Data of line*/
typedef struct {
    lv_obj_t obj;
    const lv_point_t * point_array;     /**< Pointer to an array with the points of the line*/
    uint16_t point_num;                 /**< Number of points in 'point_array'*/
    uint8_t y_inv : 1;                  /**< 1: y == 0 will be on the bottom*/
} lv_line_t;

extern const lv_obj_class_t lv_line_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/** 
 * Create a line object 创建一个直线对象
 * @param parent pointer to an object, it will be the parent of the new line 指针指向一个对象，它将是新线的父行
 * @return pointer to the created line 指向已创建线的指针
 */
lv_obj_t * lv_line_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set an array of points. The line object will connect these points. 定义线对象的坐标点。这允许你创建不仅仅是直线，而是任何由连续线段组成的形状。
 * @param obj           pointer to a line object 一个已经创建的线对象的指针
 * @param points        an array of points. Only the address is saved, so the array needs to be alive while the line exists  一个包含线段坐标点的数组。lv_point_t是一个结构体，有两个字段：x和y，分别表示点的x和y坐标。
 * @param point_num     number of points in 'point_a' 表示points数组的大小
 */
void lv_line_set_points(lv_obj_t * obj, const lv_point_t points[], uint16_t point_num);

/**
 * Enable (or disable) the y coordinate inversion.
 * If enabled then y will be subtracted from the height of the object,
 * therefore the y = 0 coordinate will be on the bottom.
 * @param obj       pointer to a line object
 * @param en        true: enable the y inversion, false:disable the y inversion
 */
void lv_line_set_y_invert(lv_obj_t * obj, bool en);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the y inversion attribute
 * @param obj       pointer to a line object
 * @return          true: y inversion is enabled, false: disabled
 */
bool lv_line_get_y_invert(const lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_LINE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_LINE_H*/
