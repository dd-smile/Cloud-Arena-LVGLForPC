/*
 * @Description: 地面翻折座椅
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-17 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/folding_seat.c
 */
#include "ui_app.h"

DevicePageData folding_seat_data = {0};
DevicePageData *fs_data = &folding_seat_data;

extern bool stop_flag;


/* 通过state的值选择打开的电机 */
void set_folding_stands_state(uint8_t num, uint8_t state)
{
    
    switch (num) // 选择设备号
    {
    case 1:// 地面翻折座椅1 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[36]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[37]);
        }
        break;

    case 2:// 地面翻折座椅2
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[38]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[39]);
        }  
        break;

    default:
        break;
    }
    
}

void set_folding_stop_state(uint8_t num, uint8_t state)
{
    
    switch (num) // 选择设备号
    {
    case 1:// 地面翻折座椅1 
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[40]);
        }
        else if(state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[41]);
        }
        break;

    case 2:// 地面翻折座椅2
        if (state == 1)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[42]);
        }
        else if (state == 0)
        {
            OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, usrnet_mqtt_msh[43]);
        }
        break;

    default:
        break;
    }
    
}

// 地面翻折座椅
void folding_Controls_event_cb(lv_event_t *e)
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
        case 0:     //地面翻折座椅打开
            if (stop_flag == false)
            {
                set_folding_stands_state(device_num, 1);
            }
            break;  

        case 1:
            if (stop_flag == false)
            {
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                set_folding_stop_state(device_num, 1);
 
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                set_folding_stop_state(device_num, 0);
            }           
            break;

        case 2:    //地面翻折座椅收起
            if (stop_flag == false)
            {
                set_folding_stands_state(device_num, 0);
            }          
            break;
            
        }
    }
}


void CreateFoldingPage(uint8_t device_num)
{

  
  fs_data->deviceName = "地埋翻转座椅";
  fs_data->intro = "设备简介: 采用地埋方式，不占用场地，释放\n"
                   "场馆空间\n";
  fs_data->date = "2023-7-4";
  fs_data->period = "6 months";
  fs_data->phoneNumber = "+86 755 26490688";
  fs_data->image = &folding_seat_big;
  fs_data->expandBtnText = "一键打开";
  fs_data->emergencyStopBtnText = "急停";
  fs_data->collapseBtnText = "一键收合";

  fs_data->StandOpenBtnText = NULL;
  fs_data->StandClosureBtnText = NULL;
  fs_data->SeatRiseBtnText = NULL;
  fs_data->SeatTipBtnText = NULL;

  fs_data->ManufacturingTel = "12345678";
  fs_data->SalehAfterotline = "12345679";
  fs_data->SolveProblem = "Solution";
  fs_data->Description = "Condition";
  fs_data->PopupTitlie = "Inspection personnel must perform the inspection in a timely manner\n"
                            "            according to the inspection time and contents";
  fs_data->PopupContent = " Maintenance\n"
                            "normal report";

  CreateFoldingPageBg(fs_data, device_num);

}