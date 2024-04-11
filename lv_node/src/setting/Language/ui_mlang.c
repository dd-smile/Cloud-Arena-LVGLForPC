/*
 * @Description:中英文转换功能
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-03-09 11:28:42
 * @LastEditTime: 2024-04-01 10:34:31
 * @FilePath: lv_node/src/setting/Language/ui_mlang.c
 */
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
    "展开", //41
    "收合", //42
    "急停", //43
    "放下", //44
    "打开", //45
    "关闭", //46
    "表演", //47
    "合唱", //48
    "上升", //49
    "下降", //50
    "总急停", //51
    "总复位", //52
    "伸缩活动看台", //53
    "设备简介: 解决高规格场馆对于高排数、\n静音稳定的看台需求。\n", //54
    "解决方案", //55
    "状况", //56
    "维护正常报告", //57
    "检验人员已进行例行检查\n"
     "       无任何异常情况",  //58
    "设备名称: ",  //59
    "制造商简介: ", //60
    "采购时间: ", //61
    "检修周期: ", //62
    "供应商服务电话: ", //63
    "故障申报", //64
    "重命名", //65
    "检修正常", //66
    "返回", //67
    "制造商售后电话: ", //68
    "屏保密码设置", //69
    "密码设置", //70
    "亮度设置", //71
    "退场模式", //72
    "舞台大幕", //73


};

const char const * g_mlang_str_eng[] = {
    "Data Board",  //0
    "Facility",   //1
    "Audio Control",  //2
    "facility",  //3
    "mode",  //4
    "setting",  //5
    "training pattern",  //6
    "game mode",  //7
    "performance mode",  //8
    "3vs3 mode",  //9
    "movable stand",  //10
    "Hanging ball rack 1",  //11
    "Hanging ball rack 2",  //12
    "Hanging ball rack 3",  //13
    "Hanging ball rack 4",  //14
    "Automatic divider 1",  //15
    "Automatic divider 2",  //16
    "Wall hanging ball rack 1",  //17
    "Wall hanging ball rack 2",  //18
    "Buried flip seat 1",  //19
    "Buried flip seat 2",  //20
    "Telescopic stage",    //21
    "Elevating rotary stage",  //22
    "Venue lighting",  //23
    "manual mode",   //24
    "Add Device",   //25
    "Please add a new device",   //26
    "Screen setup",   //27
    "Screen brightness Settings",   //28
    "Automatically enter the screen saver",   //29
    "Confirm",   //30
    "Cancel",   //31
    "Whether to save Settings",   //32
    "Time settings",   //33
    "Save",   //34
    "Screensaver Settings",  //35
    "New upgraded screensaver",  //36
    "Language\n  settings",  //37
    "Select language",   //38
    "Chinese(CN)",   //39 
    "English(US)",   //40
    "Spread",   //41
    "Closure",  //42
    "Scram",    //43
    "Vail",   //44
    "Open",   //45
    "Close",    //46
    "Perform",  //47
    "Tutti",   //48
    "Rise",    //49
    "Drop",    //50
    "E-STOP",   //51
    "RST",    //52
    "Retractable Seating System",   //53
    "Brief Introduction: Comply requirement\n"
                       "of high-standard venues for high-row, \n"
                       "quiet and stableseating system\n",   //54
    "Solution",  //55
    "Condition",  //56
    "Maintenance normal report",   //57
    "Routine checks have\n" 
        "been carried out",   //58
    "Devicename: ", //59
    "Manufacturer profile: ", //60
    "Purchasing period: ", //61
    "Repair cycle: ", //62
    "Supplier service TEL: ", //63
    "report",   //64
    "rename",   //65
    "normal",   //66
    "Back",   //67
    "After-sales hotline: ", //68
    "Screen password Settings",//69
    "password setting",  //70
    "Brightness setting",  //71
    "exit mode",   //72
    "Front curtain",  //73


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

