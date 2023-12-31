#ifndef _LV_UI_MLANG_H
#define _LV_UI_MLANG_H

//支持语言的枚举 自己定义
typedef enum {
    zhT,
    enG,
    LANGUAGE_MAX,
}language_type_e;

#define UI_MLANG_STR(_str_id)		sg_language_str_list[g_curr_lang_type][_str_id]

extern const char * const *sg_language_str_list[LANGUAGE_MAX];
extern language_type_e g_curr_lang_type;

language_type_e get_cur_language_type(void);
void set_cur_language_type(language_type_e type);


#endif

