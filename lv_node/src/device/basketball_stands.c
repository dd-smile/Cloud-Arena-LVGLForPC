/*
 * @Description: 悬空球架控制
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-11 11:28:17
 * @LastEditTime: 2023-09-12 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/device/basketball_stands.c
 */

#include "ui_app.h"

DevicePageData basketball_stands_data = {0};
DevicePageData *bs_data = &basketball_stands_data;

bool basketball_flag[4] = {false, false, false, false}; // true为收拢

/* 通过state的值选择打开的电机 */
void set_basketball_stands_state(uint8_t num, uint8_t state)
{
    
    switch (num) 
    {
    case 1:// 球架1, 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_DOWN_ONE_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_UP_ONE_FUNCTION_ENABLED]);
        }
        break;

    case 2:// 球架2, 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_DOWN_TWO_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_UP_TWO_FUNCTION_ENABLED]);
        }  
        break;

    case 3:// 球架3, 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_DOWN_THREE_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_UP_THREE_FUNCTION_ENABLED]);
        } 
        break;

    case 4:// 球架4, 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_DOWN_FOUR_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_UP_FOUR_FUNCTION_ENABLED]);
        } 
        break;

    default:
        break;
    }
    
}

void set_basketball_stop_state(uint8_t num, uint8_t state)
{
    
    switch (num) 
    {
    case 1:// 球架1,
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_ONE_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_ONE_FUNCTION_UNENABLED]);
        }
        break;

    case 2:// 球架2, 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_TWO_FUNCTION_ENABLED]);  
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_TWO_FUNCTION_UNENABLED]);  
        }
        break;

    case 3:// 球架3, 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_THREE_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_THREE_FUNCTION_UNENABLED]);
        }
        break;

    case 4:// 球架4, 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_FOUR_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[HANGING_BALL_RACK_STOP_FOUR_FUNCTION_UNENABLED]);
        }
        break;

    default:
        break;
    }
    
}

/**
 * 设备卡片悬空球架控制事件   与设备详情页的控制事件一样
 * @param e          指向要设置对象的指针
 * */
void basketball_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind;  //判断按钮号
    uint8_t device_num = user_data->num;

    if (code == LV_EVENT_CLICKED)
    {
        switch (index)
        {
        case 0: //放下球架
            if (stop_flag == false) 
            {
                set_basketball_stands_state(device_num, 1);
            }
            break;

        case 1:  //急停
            if (stop_flag == false )
            {
                stop_flag = true;   //表示急停产生
                lv_obj_add_state(obj, LV_STATE_PRESSED);  //添加长按属性，使得按钮保持被点击着的样子
                set_basketball_stop_state(device_num, 1);
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);  //移除长按属性
                set_basketball_stop_state(device_num, 0);
            }
            break;

        case 2:  //收拢球架
            if (stop_flag == false)
            {   
                set_basketball_stands_state(device_num, 0);
            }
            break;
            
        default:
            break;
            
        }
    }
}

void CreateBasketballStandsPage(uint8_t device_num)
{
    //基本说明初始化
    bs_data->deviceName = "悬空球架";
    bs_data->intro = "设备简介: 篮球架悬挂空中，远程联网\n"
                       "控制，释放场地空间。\n";
    bs_data->date = "2023-9-21";
    bs_data->period = "6 months";
    bs_data->phoneNumber = "0755-26490688";
    bs_data->image = &basketball_stands_big;  
    bs_data->expandBtnText = "一键打开";
    bs_data->emergencyStopBtnText = "急停";
    bs_data->collapseBtnText = "一键收合";

    bs_data->StandOpenBtnText = NULL;
    bs_data->StandClosureBtnText = NULL;
    bs_data->SeatRiseBtnText = NULL;
    bs_data->SeatTipBtnText = NULL;

    bs_data->ManufacturingTel = "400-618-1848";
    bs_data->SalehAfterotline = "0755-26490688";
    bs_data->SolveProblem = "解决方案" ;
    bs_data->Description = "Condition";
    bs_data->PopupTitlie = "检验人员必须及时进行检验\n"
                             "            根据检查时间和内容";
    bs_data->PopupContent = "维护\n"
                              "正常报告";

    CreatebasketballPageBg(bs_data, device_num);
}