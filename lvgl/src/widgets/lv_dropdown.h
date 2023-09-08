/**
 * @file lv_dropdown.h
 *
 */

#ifndef LV_DROPDOWN_H
#define LV_DROPDOWN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_conf_internal.h"

#if LV_USE_DROPDOWN != 0

/*Testing of dependencies*/

#if LV_USE_LABEL == 0
#error "lv_dropdown: lv_label is required. Enable it in lv_conf.h (LV_USE_LABEL 1)"
#endif

#include "../widgets/lv_label.h"

/*********************
 *      DEFINES
 *********************/
#define LV_DROPDOWN_POS_LAST 0xFFFF
LV_EXPORT_CONST_INT(LV_DROPDOWN_POS_LAST);

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;
    lv_obj_t * list;                /**< The dropped down list*/
    const char * text;              /**< Text to display on the dropdown's button*/
    const void * symbol;            /**< Arrow or other icon when the drop-down list is closed*/
    char * options;                 /**< Options in a '\n' separated list*/
    uint16_t option_cnt;            /**< Number of options*/
    uint16_t sel_opt_id;            /**< Index of the currently selected option*/
    uint16_t sel_opt_id_orig;       /**< Store the original index on focus*/
    uint16_t pr_opt_id;             /**< Index of the currently pressed option*/
    lv_dir_t dir              : 4;  /**< Direction in which the list should open*/
    uint8_t static_txt        : 1;  /**< 1: Only a pointer is saved in `options`*/
    uint8_t selected_highlight: 1;  /**< 1: Make the selected option highlighted in the list*/
} lv_dropdown_t;

typedef struct {
    lv_obj_t obj;
    lv_obj_t * dropdown;
} lv_dropdown_list_t;

extern const  lv_obj_class_t lv_dropdown_class;
extern const  lv_obj_class_t lv_dropdownlist_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a drop-down list object 创建一个下拉列表对象
 * @param parent pointer to an object, it will be the parent of the new drop-down list 指向对象的指针，它将是新下拉列表的父对象 
 * @return pointer to the created drop-down list 指向已创建的下拉列表的指针
 */
lv_obj_t * lv_dropdown_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set text of the drop-down list's button.
 * If set to `NULL` the selected option's text will be displayed on the button.
 * If set to a specific text then that text will be shown regardless of the selected option.
 * @param obj       pointer to a drop-down list object
 * @param txt       the text as a string (Only its pointer is saved)
 */
void lv_dropdown_set_text(lv_obj_t * obj, const char * txt);

/**
 * Set the options in a drop-down list from a string. 从字符串中设置下拉列表中的选项 
 * The options will be copied and saved in the object so the `options` can be destroyed after calling this function 选项将被复制并保存在对象中，因此' options '可以在调用此函数后销毁
 * @param obj       pointer to drop-down list object 指向下拉列表对象的指针
 * @param options   a string with '\n' separated options. E.g. "One\nTwo\nThree" 一个带有'\n'分隔选项的字符串
 */
void lv_dropdown_set_options(lv_obj_t * obj, const char * options);

/**
 * Set the options in a drop-down list from a static string (global, static or dynamically allocated).
 * Only the pointer of the option string will be saved.
 * @param obj       pointer to drop-down list object
 * @param options   a static string with '\n' separated options. E.g. "One\nTwo\nThree"
 */
void lv_dropdown_set_options_static(lv_obj_t * obj, const char * options);

/**
 * Add an options to a drop-down list from a string.  Only works for non-static options.
 * @param obj       pointer to drop-down list object
 * @param option    a string without '\n'. E.g. "Four"
 * @param pos       the insert position, indexed from 0, LV_DROPDOWN_POS_LAST = end of string
 */
void lv_dropdown_add_option(lv_obj_t * obj, const char * option, uint32_t pos);

/**
 * Clear all options in a drop-down list.  Works with both static and dynamic options.
 * @param obj       pointer to drop-down list object
 */
void lv_dropdown_clear_options(lv_obj_t * obj);

/**
 * Set the selected option
 * @param obj       pointer to drop-down list object
 * @param sel_opt   id of the selected option (0 ... number of option - 1);
 */
void lv_dropdown_set_selected(lv_obj_t * obj, uint16_t sel_opt);

/**
 * Set the direction of the a drop-down list
 * @param obj       pointer to a drop-down list object
 * @param dir       LV_DIR_LEFT/RIGHT/TOP/BOTTOM
 */
void lv_dropdown_set_dir(lv_obj_t * obj, lv_dir_t dir);

/**
 * Set an arrow or other symbol to display when on drop-down list's button. Typically a down caret or arrow. 设置下拉列表按钮时显示的箭头或其他符号。 通常是下插入符号或箭头
 * @param obj       pointer to drop-down list object 指向下拉列表对象的指针
 * @param symbol    a text like `LV_SYMBOL_DOWN`, an image (pointer or path) or NULL to not draw symbol icon 你想设置的符号。在LVGL中，符号通常是特殊的字符，定义在符号字体中。LVGL预定义了许多这样的符号，你可以从 lv_symbol_def.h 头文件中找到它们。这些符号以LV_SYMBOL_前缀开始。
 * @note angle and zoom transformation can be applied if the symbol is an image. 如果符号是图像，则可以应用角度和缩放变换 
 * E.g. when drop down is checked (opened) rotate the symbol by 180 degree 例如，当选中(打开)下拉框时，将符号旋转180度 
 */
void lv_dropdown_set_symbol(lv_obj_t * obj, const void * symbol);

/**
 * Set whether the selected option in the list should be highlighted or not
 * @param obj       pointer to drop-down list object
 * @param en        true: highlight enabled; false: disabled
 */
void lv_dropdown_set_selected_highlight(lv_obj_t * obj, bool en);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the list of a drop-down to allow styling or other modifications 获取下拉列表以允许进行样式化或其他修改 
 * @param obj       pointer to a drop-down list object 指向下拉列表对象的指针
 * @return          pointer to the list of the drop-down 返回指向下拉列表的指针
 */
lv_obj_t * lv_dropdown_get_list(lv_obj_t * obj);

/**
 * Get text of the drop-down list's button. 获取下拉列表按钮的文本
 * @param obj   pointer to a drop-down list object 指向下拉列表对象的指针
 * @return      the text as string, `NULL` if no text 
 */
const char * lv_dropdown_get_text(lv_obj_t * obj);

/**
 * Get the options of a drop-down list
 * @param obj       pointer to drop-down list object
 * @return          the options separated by '\n'-s (E.g. "Option1\nOption2\nOption3")
 */
const char * lv_dropdown_get_options(const lv_obj_t * obj);

/**
 * Get the index of the selected option
 * @param obj       pointer to drop-down list object
 * @return          index of the selected option (0 ... number of option - 1);
 */
uint16_t lv_dropdown_get_selected(const lv_obj_t * obj);

/**
 * Get the total number of options
 * @param obj       pointer to drop-down list object
 * @return          the total number of options in the list
 */
uint16_t lv_dropdown_get_option_cnt(const lv_obj_t * obj);

/**
 * Get the current selected option as a string
 * @param obj       pointer to drop-down object
 * @param buf       pointer to an array to store the string
 * @param buf_size  size of `buf` in bytes. 0: to ignore it.
 */
void lv_dropdown_get_selected_str(const lv_obj_t * obj, char * buf, uint32_t buf_size);

/**
 * Get the symbol on the drop-down list. Typically a down caret or arrow.
 * @param obj       pointer to drop-down list object
 * @return          the symbol or NULL if not enabled
 */
const char * lv_dropdown_get_symbol(lv_obj_t * obj);

/**
 * Get whether the selected option in the list should be highlighted or not
 * @param obj       pointer to drop-down list object
 * @return          true: highlight enabled; false: disabled
 */
bool lv_dropdown_get_selected_highlight(lv_obj_t * obj);

/**
 * Get the direction of the drop-down list
 * @param obj       pointer to a drop-down list object
 * @return          LV_DIR_LEF/RIGHT/TOP/BOTTOM
 */
lv_dir_t lv_dropdown_get_dir(const lv_obj_t * obj);

/*=====================
 * Other functions
 *====================*/

/**
 * Open the drop.down list
 * @param obj       pointer to drop-down list object
 */
void lv_dropdown_open(lv_obj_t * dropdown_obj);

/**
 * Close (Collapse) the drop-down list
 * @param obj       pointer to drop-down list object
 */
void lv_dropdown_close(lv_obj_t * obj);

/**
 * Tells whether the list is opened or not
 * @param obj       pointer to a drop-down list object
 * @return          true if the list os opened
 */
bool lv_dropdown_is_open(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_DROPDOWN*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_DROPDOWN_H*/
