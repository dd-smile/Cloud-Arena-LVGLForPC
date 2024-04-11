/*
 * @Description: 
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-17 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/curtain.c
 */
#include "ui_app.h"

DevicePageData curtain_data = {0};
DevicePageData *cur_data = &curtain_data;   //舞台大幕

bool curtain_flag = false; 

void curtain_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; 
    uint8_t device_num = user_data->num;

    if (code == LV_EVENT_CLICKED)
    {
        switch (index)
        {
        case 0: 
            if(stop_flag == false && curtain_flag == false)
            {          
                curtain_flag = true;    
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CURTAIN_OPEN_FUNCTION_ENABLED]);
            }else
            {   
                curtain_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CURTAIN_OPEN_FUNCTION_UNENABLED]);
            }    
            break;            
        case 1:
            if(stop_flag == false) // 急停
            {
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CURTAIN_STOP_FUNCTION_ENABLED]);
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CURTAIN_STOP_FUNCTION_UNENABLED]);
            }
            break;          
        case 2: // 收拢
            if(stop_flag == false && curtain_flag == false)
            {   
                curtain_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CURTAIN_SHUT_FUNCTION_ENABLED]);
            }else
            {
                curtain_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CURTAIN_SHUT_FUNCTION_UNENABLED]);
            }
            break;
        }
    }
}


void CreateCurtainPage(uint8_t device_num)
{

  cur_data->deviceName = "舞台大幕";
  cur_data->intro = "设备简介: 舞台大幕远程联网智能\n"
                   "控制，体现场馆智能化\n";
  cur_data->date = "2023-7-4";
  cur_data->period = "6 months";
  cur_data->phoneNumber = "+86 755 26490688";
  cur_data->image = &partition_curtain_big;
  cur_data->expandBtnText = "一键展开";
  cur_data->emergencyStopBtnText = "急停";
  cur_data->collapseBtnText = "一键合拢";

  cur_data->StandOpenBtnText = NULL;
  cur_data->StandClosureBtnText = NULL;
  cur_data->SeatRiseBtnText = NULL;
  cur_data->SeatTipBtnText = NULL;

  cur_data->ManufacturingTel = "+86 755 26490688";
  cur_data->SalehAfterotline = "+86 755 26490688";
  cur_data->SolveProblem = "Solution";
  cur_data->Description = "Condition";
  cur_data->PopupTitlie = "Inspection personnel must perform the inspection in a timely manner\n"
                            "            according to the inspection time and contents";
  cur_data->PopupContent = " Maintenance\n"
                            "normal report";

  CreatecurtainPageBg(cur_data, device_num);
}