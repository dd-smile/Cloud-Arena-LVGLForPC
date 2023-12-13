/*
 * @Description: 分隔幕
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-07-03 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/partition_curtian.c
 */
#include "ui_app.h"

DevicePageData partition_curtain_data = {0};
DevicePageData *pc_data = &partition_curtain_data; // 分隔幕的详情页信息

extern bool stop_flag;

bool patition_flag = false; // true为收拢


/* 通过state的值选择打开的电机 */
void set_partition_stands_state(uint8_t num, uint8_t state)
{
    
    switch (num) // 选择设备号
    {
    case 1:// 分隔幕1 
        if(state == 1)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_down_1\",\"v\":\"%d\"}]}",1);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }else if (state == 0)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_up_1\",\"v\":\"%d\"}]}",1);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }
        break;

    case 2:// 分隔幕2
        if(state == 1)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_down_2\",\"v\":\"%d\"}]}",1);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }else if(state == 0)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_up_2\",\"v\":\"%d\"}]}",1);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }  
        break;

    default:
        break;
    }
    
}
//急停
void set_partition_stop_state(uint8_t num, uint8_t state)
{
    
    switch (num) // 选择设备号
    {
    case 1:
        if(state == 1)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_stop_1\",\"v\":\"%d\"}]}",1);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }else if(state == 0)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_stop_1\",\"v\":\"%d\"}]}",0);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }
        break;

    case 2:
        if(state == 1)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_stop_2\",\"v\":\"%d\"}]}",1);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }else if(state == 0)
        {
            sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"divider_stop_2\",\"v\":\"%d\"}]}",0);
            OneNet_Publish("/mytest/ycg", PUB_BUF);
        }
        break;

    default:
        break;
    }
    
}

// 分隔幕
void partition_Controls_event_cb(lv_event_t *e)
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
        case 0:     //分隔幕放下
            if (stop_flag == false)
            {
                set_partition_stands_state(device_num, 1);
            }
            break;            
        case 1:
            if(stop_flag == false)
            {
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                set_partition_stop_state(device_num, 1);
 
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                set_partition_stop_state(device_num, 0);
            }           
            break;          
        case 2:    //分隔幕收起
            if(stop_flag == false)
            {
                set_partition_stands_state(device_num, 0);
            }          
            break;
        }
    }
}

void CreatePartitionCurtianPage(uint8_t device_num)
{

  
  pc_data->deviceName = "自动分隔幕";
  pc_data->intro = "设备简介: 可变场地分隔，结构轻巧灵\n"
                    "活、安全高效、阻隔噪音\n";
  pc_data->date = "2023-7-4";
  pc_data->period = "6 months";
  pc_data->phoneNumber = "+86 755 26490688";
  pc_data->image = &partition_curtain_big;
  pc_data->expandBtnText = "一键打开";
  pc_data->emergencyStopBtnText = "急停";
  pc_data->collapseBtnText = "一键收合";

  pc_data->StandOpenBtnText = NULL;
  pc_data->StandClosureBtnText = NULL;
  pc_data->SeatRiseBtnText = NULL;
  pc_data->SeatTipBtnText = NULL;

  pc_data->ManufacturingTel = "12345678";
  pc_data->SalehAfterotline = "12345679";
  pc_data->SolveProblem = "Solution";
  pc_data->Description = "Condition";
  pc_data->PopupTitlie = "Inspection personnel must perform the inspection in a timely manner\n"
                            "            according to the inspection time and contents";
  pc_data->PopupContent = " Maintenance\n"
                            "normal report";

  CreatePartitionPageBg(pc_data, device_num);


}
