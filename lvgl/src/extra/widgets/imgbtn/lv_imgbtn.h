/**
 * @file lv_imgbtn.h
 *
 */

#ifndef LV_IMGBTN_H
#define LV_IMGBTN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"

#if LV_USE_IMGBTN != 0

/*********************
 *      DEFINES
 *********************/
typedef enum {
    LV_IMGBTN_STATE_RELEASED,     /**<释放状态*/
    LV_IMGBTN_STATE_PRESSED,        /**<按下状态*/
    LV_IMGBTN_STATE_DISABLED,           /**<禁用状态*/
    LV_IMGBTN_STATE_CHECKED_RELEASED,       /**<选中释放状态*/
    LV_IMGBTN_STATE_CHECKED_PRESSED,        /**<选中按下状态*/
    LV_IMGBTN_STATE_CHECKED_DISABLED,       /**<选中禁用状态*/
    _LV_IMGBTN_STATE_NUM,                   /**<状态数量*/
} lv_imgbtn_state_t;

/**********************
 *      TYPEDEFS
 **********************/
/*Data of image button*/
typedef struct {
    lv_obj_t obj;
    const void * img_src_mid[_LV_IMGBTN_STATE_NUM];   /*Store center images to each state*/
    const void * img_src_left[_LV_IMGBTN_STATE_NUM];  /*Store left side images to each state*/
    const void * img_src_right[_LV_IMGBTN_STATE_NUM]; /*Store right side images to each state*/
    lv_img_cf_t act_cf; /*Color format of the currently active image*/
} lv_imgbtn_t;

extern const lv_obj_class_t lv_imgbtn_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create an image button object
 * 创建一个图像按钮对象
 * @param parent pointer to an object, it will be the parent of the new image button
 * @return pointer to the created image button
 */
lv_obj_t * lv_imgbtn_create(lv_obj_t * parent);

/*======================
 * Add/remove functions
 *=====================*/

/*=====================
 * Setter functions
 *====================*/

/**
 * Set images for a state of the image button
 * 为图像按钮的状态设置图像
 * @param imgbtn pointer to an image button object 指向图像按钮对象的指针
 * @param state for which state set the new image 为哪个状态设置新映像
 * @param src_left pointer to an image source for the left side of the button (a C array or path to
 * a file) 指向按钮左侧图像源的指针(指向的C数组或路径)  
 * @param src_mid pointer to an image source for the middle of the button (ideally 1px wide) (a C
 * array or path to a file) 指针指向按钮中间的图像源(理想情况下1px宽)
 * @param src_right pointer to an image source for the right side of the button (a C array or path
 * to a file) 指向按钮右侧图像源的指针(指向的C数组或路径) 
 */
void lv_imgbtn_set_src(lv_obj_t * imgbtn, lv_imgbtn_state_t state, const void * src_left, const void * src_mid,
                       const void * src_right);


/**
 * Use this function instead of `lv_obj_add/clear_state` to set a state manually 使用这个函数代替' lv_obj_add/clear_state '手动设置状态
 * @param imgbtn pointer to an image button object 指向图像按钮对象的指针
 * @param state  the new state 新状态
 */
void lv_imgbtn_set_state(lv_obj_t * imgbtn, lv_imgbtn_state_t state);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the left image in a given state
 * @param imgbtn pointer to an image button object
 * @param state the state where to get the image (from `lv_btn_state_t`) `
 * @return pointer to the left image source (a C array or path to a file)
 */
const void * lv_imgbtn_get_src_left(lv_obj_t * imgbtn, lv_imgbtn_state_t state);

/**
 * Get the middle image in a given state
 * @param imgbtn pointer to an image button object
 * @param state the state where to get the image (from `lv_btn_state_t`) `
 * @return pointer to the middle image source (a C array or path to a file)
 */
const void * lv_imgbtn_get_src_middle(lv_obj_t * imgbtn, lv_imgbtn_state_t state);

/**
 * Get the right image in a given state
 * @param imgbtn pointer to an image button object
 * @param state the state where to get the image (from `lv_btn_state_t`) `
 * @return pointer to the left image source (a C array or path to a file)
 */
const void * lv_imgbtn_get_src_right(lv_obj_t * imgbtn, lv_imgbtn_state_t state);


/*=====================
 * Other functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_IMGBTN*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_IMGBTN_H*/
