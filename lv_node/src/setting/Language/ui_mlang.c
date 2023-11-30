#include "ui_app.h"


#define DEFAULT_LANGUAGE                    enG

language_type_e g_curr_lang_type = DEFAULT_LANGUAGE; //这个地方这个是语言的类型 我们可以set他就是改变他的语言

const char const* g_mlang_str_zhc[] = {
    "数据看板",
    "设备",
    "音频控制"
};

const char const * g_mlang_str_eng[] = {
    "Data Board",
    "Facility",
    "Audio Control"
};

const char * const *sg_language_str_list[LANGUAGE_MAX] = {//这个指针数组能就是用来获取字符串的
	[zhT] = g_mlang_str_zhc,
    [enG] = g_mlang_str_eng,
};

void set_cur_language_type(language_type_e type)//设置当前语言
{
    g_curr_lang_type = type;
}
language_type_e get_cur_language_type(void)//获取当前语言
{
    return g_curr_lang_type;
}

