/*
 * @Description:  璧挂球架
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-07-03 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/wallhanging.c
 */
#include "ui_app.h"

DevicePageData wallhanging_data = {0};
DevicePageData *wa_data = &wallhanging_data;

extern bool stop_flag;

/* 通过state的值选择打开的电机 */
void set_wallhanging_stands_state(uint8_t num, uint8_t state)
{
    
    switch (num) // 选择设备号
    {
    case 1:// 壁挂球架1 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_OPEN_ONE_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_SHUT_ONE_FUNCTION_ENABLED]);
        }
        break;

    case 2:// 壁挂球架2
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_OPEN_TWO_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_SHUT_TWO_FUNCTION_ENABLED]);
        }  
        break;

    default:
        break;
    }
    
}

void set_wallhanging_stop_state(uint8_t num, uint8_t state)
{
    
    switch (num) 
    {
    case 1:// 壁挂球架1 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_STOP_ONE_FUNCTION_ENABLED]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_STOP_ONE_FUNCTION_UNENABLED]);
        }
        break;

    case 2:// 壁挂球架2
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_STOP_TWO_FUNCTION_ENABLED]);  
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[WALL_HANGING_BALL_RACK_STOP_TWO_FUNCTION_UNENABLED]);  
        }
        break;

    default:
        break;
    }
    
}

// 壁挂球架
void wallhanging_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; // 判断按钮号
    uint8_t device_num = user_data->num;
    //printf("index %d\n",index);

    if (code == LV_EVENT_CLICKED)   
    {
        switch (index)
        {
        case 0:     //壁挂球架打开
            if (stop_flag == false)
            {
                set_wallhanging_stands_state(device_num, 1);
            }
            break;

        case 1:
            if (stop_flag == false)
            {
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                set_wallhanging_stop_state(device_num, 1);
 
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                set_wallhanging_stop_state(device_num, 0);
 
            }           
            break;   

        case 2:    //壁挂球架收起
            if (stop_flag == false)
            {
                set_wallhanging_stands_state(device_num, 0);
            }          
            break;

        default:
            break;

        }
    }
}



void CreateWallhangingPage(uint8_t device_num)
{

  
  wa_data->deviceName = "壁挂球架";
  wa_data->intro = "设备简介: 方便收合省时省力，让运动无处不在\n"
                   "高度可调节满足不同年龄段\n";
  wa_data->date = "2023-7-4";
  wa_data->period = "6 months";
  wa_data->phoneNumber = "0755-26490688";
  wa_data->image = &wall_hanging_ball_big;
  wa_data->expandBtnText = "一键打开";
  wa_data->emergencyStopBtnText = "急停";
  wa_data->collapseBtnText = "一键收合";

  wa_data->StandOpenBtnText = NULL;
  wa_data->StandClosureBtnText = NULL;
  wa_data->SeatRiseBtnText = NULL;
  wa_data->SeatTipBtnText = NULL;

  wa_data->ManufacturingTel = "400-618-1848";
  wa_data->SalehAfterotline = "0755-26490688";
  wa_data->SolveProblem = "Solution";
  wa_data->Description = "Condition";
  wa_data->PopupTitlie = "Inspection personnel must perform the inspection in a timely manner\n"
                            "            according to the inspection time and contents";
  wa_data->PopupContent = " Maintenance\n"
                            "normal report";

  CreateWallhangingPageBg(wa_data, device_num);

}