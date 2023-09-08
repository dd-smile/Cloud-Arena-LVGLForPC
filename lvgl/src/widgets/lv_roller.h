/**
 * @file lv_roller.h
 *
 */

#ifndef LV_ROLLER_H
#define LV_ROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_conf_internal.h"

#if LV_USE_ROLLER != 0

#include "../core/lv_obj.h"
#include "lv_label.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/** Roller mode.*/
enum {
    LV_ROLLER_MODE_NORMAL, /**< Normal mode (roller ends at the end of the options). 正常模式(辊端在选项的末端)*/
    LV_ROLLER_MODE_INFINITE, /**< Infinite mode (roller can be scrolled forever). 无限模式(滚轮可以永远滚动)*/
};

typedef uint8_t lv_roller_mode_t;

typedef struct {
    lv_obj_t obj;
    uint16_t option_cnt;          /**< Number of options*/
    uint16_t sel_opt_id;          /**< Index of the current option*/
    uint16_t sel_opt_id_ori;      /**< Store the original index on focus*/
    lv_roller_mode_t mode : 1;
    uint32_t moved : 1;
} lv_roller_t;

extern const lv_obj_class_t lv_roller_class;


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a roller object 创建滚轮对象
 * @param parent    pointer to an object, it will be the parent of the new roller. 指向一个对象的指针，它将是新滚轮的父对象。
 * @return          pointer to the created roller 指向创建的滚轮的指针
 */
lv_obj_t * lv_roller_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the options on a roller 设置滚轮上的选项
 * @param obj       pointer to roller object 指向滚轮对象的指针
 * @param options   a string with '\n' separated options. E.g. "One\nTwo\nThree"  一个带有'\n'分隔选项的字符串
 * @param mode      `LV_ROLLER_MODE_NORMAL` or `LV_ROLLER_MODE_INFINITE`
 */
void lv_roller_set_options(lv_obj_t * obj, const char * options, lv_roller_mode_t mode);

/**
 * Set the selected option 允许用户从一个滚动列表中选择一个选项
 * @param obj       pointer to a roller object 指向滚轮对象的指针
 * @param sel_opt   index of the selected option (0 ... number of option - 1); 要选择的选项的索引。例如，0 代表第一个选项，1 代表第二个选项，以此类推
 * @param anim_en   LV_ANIM_ON: set with animation; LV_ANOM_OFF set immediately LV_ANIM_ON:使用动画;LV_ANOM_OFF:不使用设置
 */
void lv_roller_set_selected(lv_obj_t * obj, uint16_t sel_opt, lv_anim_enable_t anim);

/**
 * Set the height to show the given number of rows (options) 设置高度以显示给定的行数(选项)
 * @param obj       pointer to a roller object 指向滚轮对象的指针
 * @param row_cnt   number of desired visible rows 所需的可见行数
 */
void lv_roller_set_visible_row_count(lv_obj_t * obj, uint8_t row_cnt);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the index of the selected option
 * @param obj       pointer to a roller object
 * @return          index of the selected option (0 ... number of option - 1);
 */
uint16_t lv_roller_get_selected(const lv_obj_t * obj);

/**
 * Get the current selected option as a string. 将当前选定的选项作为字符串获取
 * @param obj       pointer to ddlist object  你想查询的滚轮对象
 * @param buf       pointer to an array to store the string 一个字符数组，用于存储选中选项的字符串表示
 * @param buf_size  size of `buf` in bytes. 0: to ignore it. buf 的大小（即字符数组可以容纳的最大字符数，包括结尾的 NULL 字符）
 */
void lv_roller_get_selected_str(const lv_obj_t * obj, char * buf, uint32_t buf_size);


/**
 * Get the options of a roller
 * @param obj       pointer to roller object
 * @return          the options separated by '\n'-s (E.g. "Option1\nOption2\nOption3")
 */
const char * lv_roller_get_options(const lv_obj_t * obj);

/**
 * Get the total number of options
 * @param obj   pointer to a roller object
 * @return      the total number of options
 */
uint16_t lv_roller_get_option_cnt(const lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_ROLLER*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_ROLLER_H*/
