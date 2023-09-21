/*
 * @Description: 伸缩活动看台
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-07-03 13:58:49
 * @LastEditTime: 2023-09-13 11:39:31
 * @FilePath: /new_home_smart/lv_node/src/device/telescopic_stand.c
 */
#include "ui_app.h"
DevicePageData lvPageData = {0};
DevicePageData *pPageData = &lvPageData;
uint8_t ReadSerialDiNum;  //用于读取ＤＩ
int Switch_status = -1;   //按钮开关状态(用于判断是哪个按钮)

bool stop_flag = false;   //急停标志位
bool stand_flag = false;  //看台标志位，ｔｒｕｅ为收拢

/**
 * 设备卡片看台控制事件
 * 从机１的ＤＯ１控制看台展开电机，Ｄ０２控制看台收拢电机
 * ＤＯ３座椅展开电机，Ｄ０４座椅收合电机
 * @param e          指向要设置对象的指针
 * */
void telescoopic_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);  //获取事件最初针对的对象    （活动看台按钮）

    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; //判断按钮号

    if(code == LV_EVENT_CLICKED)
    {
        switch (index)
        {
        case 0:  //一键打开
            if(stop_flag == false) 
            {
                for (size_t i = 0; i < 4; i++)  //关闭ＤＯ
                {
                    SetSerialDo(1,i,0);
                }
                SetSerialDo(1,0,1);   //打开DO1   第一个ＩＯ控制器
                ReadSerialDiNum = 1;
            }
            break;
        case 1:   //急停
            if(stop_flag == false)
            {
                stop_flag = true;   //表示急停产生

                lv_obj_add_state(obj, LV_STATE_PRESSED);  //添加长按属性，使得按钮保持被点击着的样子
                //关闭所有继电器的端口
                for (size_t i = 0; i < 6; i++)
                {
                    SetSerialDo(1,i,0);
                }
                for (size_t i = 0; i < 6; i++)
                {
                    SetSerialDo(2,i,0);
                }
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj,LV_STATE_PRESSED);
            }
            break;
        case 2:   //一键关闭
            if (stop_flag == false)
            {
                for (size_t i = 0; i < 4; i++) //关闭ＤＯ
                {
                    SetSerialDo(1,i,0);
                }
                SetSerialDo(1,3,1);    //打开DO4    第一个ＩＯ控制器   先收拢座椅  
                ReadSerialDiNum = 4;
            }
            break;
        }
        Switch_status = index;   //保存区分按钮一键或单独控制
    }
}

/**
 * 详情页面看台控制事件
 * 从机１的ＤＯ１控制看台展开电机，Ｄ０２控制看台收拢电机
 * ＤＯ３座椅展开电机，Ｄ０４座椅收合电机
 * @param e          指向要设置对象的指针
 * */
void Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; //判断按钮号

    if(code == LV_EVENT_CLICKED)
    {
        if(index >= 3)   //若是下方４个按钮
        {
            for (size_t i = 0; i < 4; i++)
            {
                if(i != index - 3)
                {
                    SetSerialDo(1,i,0);  //其余电机关闭
                }
            }
        }
        switch(index)
        {
        case 0: //一键打开
            if(stop_flag == false)
            {
                ReadSerialDiNum = 1;
                //SetAllDoClose(0XFE);  关闭ＤＯ   写在开发板的代码中
                usleep(100 * 1000);
                SetSerialDo(1,0,1);   //展开看台，DO1打开
            }
            break;
        case 1: //急停
            if(stop_flag == false)
            {
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                //SetAllDoClose(0XFE);  关闭ＤＯ   
            }
            else
            {
                stop_flag = false;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
            }
            break;
        case 2: //一键关闭
            if(stop_flag == false)
            {
                ReadSerialDiNum = 4;
                //SetAllDoClose(0XFE);  关闭ＤＯ   
                SetSerialDo(1,3,1);    //打开DO4    第一个ＩＯ控制器   先收拢座椅  
            }
        case 3:  //打开看台
            if(stop_flag == false)
            {
                ReadSerialDiNum = 1;
                SetSerialDo(1,0,1);    //打开DO1   　　
            }
            break;
        case 4:  //收拢看台
            if(stop_flag == false)
            {
                ReadSerialDiNum = 2;
                SetSerialDo(1,1,1);   //打开DO2  
            }
            break;
        case 5:  //打开座椅,看台打开才能打开
            if (stop_flag == false && !stand_flag)
            {
                ReadSerialDiNum = 3;
                SetSerialDo(1,2,1);    //打开DO3
            }
            break;
        case 6:  //收拢座椅
            if(stop_flag == false)
            {
                ReadSerialDiNum = 4;
                SetSerialDo(1,3,1);    //打开DO4
            }
            break;
        }
        Switch_status = index;  //保存区分按钮一键或单独控制
    }
}


/**
 * 创建设备详情页面文字说明
 * @param device_num        
 * */
void CreateTelescopicStandPage(uint8_t device_num)
{

    //基本说明初始化
    pPageData->deviceName = "伸缩活动看台";
    pPageData->intro = "设备简介: 解决高规格场馆对于高排数、\n"
                       "静音稳定的看台需求。\n";
    pPageData->date = "2023-7-4";
    pPageData->period = "6 months";
    pPageData->phoneNumber = "+86 755 26490688";
    pPageData->image = &telescopic_stand_big;  
    pPageData->expandBtnText = "一键打开";
    pPageData->emergencyStopBtnText = "急停";
    pPageData->collapseBtnText = "一键收合";

    pPageData->StandOpenBtnText = "看台展开";
    pPageData->StandClosureBtnText = "看台收缩";
    pPageData->SeatRiseBtnText = "座椅上升";
    pPageData->SeatTipBtnText = "座椅下降";

    pPageData->ManufacturingTel = "12345678";
    pPageData->SalehAfterotline = "12345679";
    pPageData->SolveProblem = "解决方案" ;
    pPageData->Description = "Condition";
    pPageData->PopupTitlie = "检验人员必须及时进行检验\n"
                             "            根据检查时间和内容";
    pPageData->PopupContent = "维护\n"
                              "正常报告";

    CreateDevicePageBg(pPageData, device_num);
}
