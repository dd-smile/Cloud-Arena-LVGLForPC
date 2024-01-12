#include "ui_app.h"


#define DEFAULT_LANGUAGE                    zhT

language_type_e g_curr_lang_type = DEFAULT_LANGUAGE; //这个地方这个是语言的类型 我们可以set他就是改变他的语言

const char const* g_mlang_str_zhc[] = {
    "数据看板", //0
    "设备",  //1
    "音频控制",  //2
    "设备",  //3
    "模式",  //4
    "设置",  //5
    "训练模式",  //6
    "比赛模式",  //7
    "演出模式",  //8
    "3vs3模式",  //9
    "活动看台",  //10
    "1号悬挂球架",  //11
    "2号悬挂球架",  //12
    "3号悬挂球架",  //13
    "4号悬挂球架",  //14
    "1号自动分隔幕",  //15
    "2号自动分隔幕", //16
    "1号璧挂球架",  //17
    "2号璧挂球架",  //18
    "1号地面翻折座椅",  //19
    "2号地面翻折座椅",  //20
    "伸缩升降舞台",   //21
    "旋转升降舞台",  //22
    "场馆灯光",   //23
    "手动模式",   //24
    "增加设备",   //25
    "请添加一个新设备",   //26
    "屏幕设置", //27
    "屏幕亮度设置", //28
    "自动进入屏幕保护", //29
    "确定",  //30
    "取消", //31
    "是否保存设置", //32
    "时间设置", //33
    "保存", //34
    "屏保设置", //35
    "全新升级屏保", //36
    "语言设置", //37
    "选择语言", //38
    "中文", //39
    "英文", //40
};

const char const * g_mlang_str_eng[] = {
    "Data Board",
    "Facility",
    "Audio Control",
    "facility",
    "mode",
    "setting",
    "training pattern",
    "game mode",
    "performance mode",
    "3vs3 mode",
    "movable stand",
    "Hanging ball rack 1",
    "Hanging ball rack 2",
    "Hanging ball rack 3",
    "Hanging ball rack 4",
    "Automatic divider 1",
    "Automatic divider 2",
    "Wall hanging ball rack 1",
    "Wall hanging ball rack 2",
    "Buried flip seat 1",
    "Buried flip seat 2",
    "Telescopic stage",
    "Elevating rotary stage",
    "Venue lighting",
    "manual mode",
    "Add Device",
    "Please add a new device",
    "Screen setup",
    "Screen brightness Settings",
    "Automatically enter the screen saver",
    "Confirm",
    "Cancel",
    "Whether to save Settings",
    "Time settings",
    "Save",
    "Screensaver Settings",
    "New upgraded screensaver",
    "Language\n  settings",
    "Select language",
    "Chinese(CN)",
    "English(US)"
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

