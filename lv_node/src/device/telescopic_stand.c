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
int Switch_status = -1;   //按钮开关状态(用于判断是哪个按钮)

bool stop_flag = false;   //急停标志位
bool stand_flag = false;  //看台标志位，ｔｒｕｅ为收拢

/**
 * 设备卡片看台控制事件
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
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[0]);
            }
            break;
        case 1:   //急停
            if(stop_flag == false)
            {
                stop_flag = true;   //表示急停产生

                lv_obj_add_state(obj, LV_STATE_PRESSED);  //添加长按属性，使得按钮保持被点击着的样子
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[1]);
               //       /mytest/ycg        /usr/plcnet/Cloud_Arena_sports/edge/d
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
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[2]);
            }
            break;
        }
        Switch_status = index;   //保存区分按钮一键或单独控制
    }
}


/**
 * 创建设备详情页面文字说明
 * @param device_num        
 * */
void CreateTelescopicStandPage(uint8_t device_num)
{

    //基本说明初始化
    // pPageData->deviceName = "伸缩活动看台";
    // pPageData->intro = "设备简介: 解决高规格场馆对于高排数、\n"
    //                    "静音稳定的看台需求。\n";
    // pPageData->date = "2023-7-4";
    // pPageData->period = "6 months";
    // pPageData->phoneNumber = "+86 755 26490688";
    // pPageData->image = &telescopic_stand_big;  
    // pPageData->expandBtnText = "一键打开";
    // pPageData->emergencyStopBtnText = "急停";
    // pPageData->collapseBtnText = "一键收合";

    // pPageData->StandOpenBtnText = "看台展开";
    // pPageData->StandClosureBtnText = "看台收缩";
    // pPageData->SeatRiseBtnText = "座椅上升";
    // pPageData->SeatTipBtnText = "座椅下降";

    // pPageData->ManufacturingTel = "12345678";
    // pPageData->SalehAfterotline = "12345679";
    // pPageData->SolveProblem = "解决方案" ;
    // pPageData->Description = "Condition";
    // pPageData->PopupTitlie = "检验人员必须及时进行检验\n"
    //                          "            根据检查时间和内容";
    // pPageData->PopupContent = "维护\n"
    //                           "正常报告";
    pPageData->deviceName = "Retractable Seating System";
    pPageData->intro = "Brief Introduction: Comply requirement\n"
                       "of high-standard venues for high-row, \n"
                       "quiet and stableseating system\n";
    pPageData->date = "2023-7-4";
    pPageData->period = "6 months";
    pPageData->phoneNumber = "+86 755 26490688";
    pPageData->image = &telescopic_stand_big;
    pPageData->expandBtnText = "Open";
    pPageData->emergencyStopBtnText = "Stop";
    pPageData->collapseBtnText = "Close";

    pPageData->StandOpenBtnText = "StandOpen";
    pPageData->StandClosureBtnText = "StandClose";
    pPageData->SeatRiseBtnText = "SeatRise";
    pPageData->SeatTipBtnText = "SeatTip";

    pPageData->ManufacturingTel = "12345678";
    pPageData->SalehAfterotline = "12345679";
    pPageData->SolveProblem = "Solution" ;
    pPageData->Description = "Condition";
    pPageData->PopupTitlie = "Inspection personnel must perform the inspection in a timely manner\n"
                             "            according to the inspection time and contents";
    pPageData->PopupContent = " Maintenance\n"
                              "normal report";

    CreateDevicePageBg(pPageData, device_num);
}
