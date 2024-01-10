/*
 * @Description: 伸缩舞台与升降舞台
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-17 13:58:49
 * @LastEditTime: 2023-10-17 19:33:15
 * @FilePath: lv_node/src/device/contraction_stage.c
 */
#include "ui_app.h"

DevicePageData contraction_stage_data = {0};
DevicePageData *cs_data = &contraction_stage_data;

DevicePageData revolving_stage_data = {0};
DevicePageData *rs_data = &revolving_stage_data;

extern bool stop_flag;
bool data_flag = false;   //数据刷新   
bool rotate_flag = false;     

// 伸缩舞台事件
void contraction_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; // 判断按钮号
    // uint8_t device_num = user_data->num;
    //printf("index %d\n",index);

    if (code == LV_EVENT_CLICKED)   
    {
        switch (index)
        {
        case 0:     //舞台表演打开
            if (stop_flag == false)
            {
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_MODE_FUNCTION_ENABLED]); 
            }
            break; 
                       
        case 1:   //舞台合唱打开
            if (stop_flag == false)
            {
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[CHORUS_MODE_FUNCTION_ENABLED]); 
 
            }          
            break; 

        case 2:    //舞台收起
            if (stop_flag == false)
            {
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CLOSED_OPERATION_FUNCTION_ENABLED]); 
            }          
            break;

        default:
            break;

        }
    }
}

// 伸缩舞台详情页面事件
void telescopic_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; // 判断按钮号

    if (code == LV_EVENT_CLICKED)   
    {
        switch (index)
        {
        case 0:     //舞台表演打开
            if (data_flag == false)
            {
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_MODE_FUNCTION_ENABLED]); 
            }
            break; 

        case 1:   //舞台合唱打开
            if (data_flag == false)
            {
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[CHORUS_MODE_FUNCTION_ENABLED]); 
 
            }          
            break; 

        case 2:    //舞台收起
            if (data_flag == false)
            {
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_CLOSED_OPERATION_FUNCTION_ENABLED]); 
            }          
            break;

        case 3:  //数据刷新
            if (data_flag == false)
            {
                data_flag = true;    //表示数据刷新按钮被按了下去
                lv_obj_add_state(obj, LV_STATE_PRESSED);   //BUTTON高亮
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_RESET_OPERTION_FUNCTION_ENABLED]);
            }
            else
            {
                data_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);   //BUTTON取消高亮
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_RESET_OPERTION_FUNCTION_UNENABLED]);
            }
            break;

        case 4: //舞台急停
            if (data_flag == false && stop_flag == false)
            {
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);   //BUTTON高亮
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_STOP_OPERTION_FUNCTION_ENABLED]);
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);   //BUTTON取消高亮
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[STAGE_STOP_OPERTION_FUNCTION_UNENABLED]);
            }
            break;

        default:
            break;

        }
    }
}

// 升降旋转舞台事件
void revolving_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; // 判断按钮号
    // uint8_t device_num = user_data->num;
    //printf("index %d\n",index);

    if (code == LV_EVENT_CLICKED)   
    {
        switch (index)
        {
        case 0:     //上升
            if (stop_flag == false && rotate_flag == false)
            {   
                rotate_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[REVOLVING_STAGE_RISE_FUNCTION_ENABLED]); 
            }
            else
            {
                rotate_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[REVOLVING_STAGE_RISE_FUNCTION_UNENABLED]);
            }
            break;     

        case 1:   //急停
            if (stop_flag == false)
            {   
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_STOP_FUNCTION_ENABLED]); 
 
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_STOP_FUNCTION_UNENABLED]);
            }
            break;  

        case 2:    //下降
            if (stop_flag == false && rotate_flag == false)
            {   
                rotate_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_DESCENT_FUNCTION_ENABLED]); 
            }  
            else
            {
                rotate_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_DESCENT_FUNCTION_UNENABLED]);
            }        
            break;

        default:
            break;

        }
    }
}

// 升降旋转舞台详情页面事件
void stage_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    index_t *user_data = lv_event_get_user_data(e);
    uint8_t index = user_data->ind; // 判断按钮号
    // uint8_t device_num = user_data->num;
    //printf("index %d\n",index);

    if (code == LV_EVENT_CLICKED)   
    {
        switch (index)
        {
        case 0:     //上升
            if (stop_flag == false && rotate_flag == false)
            {   
                rotate_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[REVOLVING_STAGE_RISE_FUNCTION_ENABLED]); 
            }
            else
            {
                rotate_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[REVOLVING_STAGE_RISE_FUNCTION_UNENABLED]);
            }
            break;      

        case 1:   //急停
            if (stop_flag == false)
            {   
                stop_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_STOP_FUNCTION_ENABLED]); 
 
            }
            else
            {   
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_STOP_FUNCTION_UNENABLED]);
            }
            break;    

        case 2:    //下降
            if (stop_flag == false && rotate_flag == false)
            {   
                rotate_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_DESCENT_FUNCTION_ENABLED]); 
            }  
            else
            {
                rotate_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_DESCENT_FUNCTION_UNENABLED]);
            }        
            break;

        case 3:    //正转
            if (stop_flag == false && rotate_flag == false)
            {   
                rotate_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_TURNING_FUNCTION_ENABLED]);
            }   
            else
            {
                rotate_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_TURNING_FUNCTION_UNENABLED]);
            }       
            break;

        case 4:    //反转
            if (stop_flag == false && rotate_flag == false)
            {   
                rotate_flag = true;
                lv_obj_add_state(obj, LV_STATE_PRESSED);
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_REVERSAL_FUNCTION_ENABLED]);
            } 
            else
            {
                rotate_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);
               OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[ROTARY_STAGE_REVERSAL_FUNCTION_UNENABLED]);
            }         
            break;

        default:
            break;

        }
    }
}

/**
 * 伸缩舞台详情页面数据
 * @param device_num            设备编号
*/
void CreateContractionPage(uint8_t device_num)
{

  cs_data->deviceName = "伸缩舞台";
  cs_data->intro = "设备简介: 采用伸缩调节，适应不同表演、演出\n"
                   "需求，高效快捷转换\n";
  cs_data->date = "2023-7-4";
  cs_data->period = "6 months";
  cs_data->phoneNumber = "+86 755 26490688";
  cs_data->image = &telescopic_stage_big;
  cs_data->expandBtnText = "合唱模式";
  cs_data->emergencyStopBtnText = "表演模式";
  cs_data->collapseBtnText = "一键收合";

  cs_data->StandOpenBtnText = "数据刷新";
  cs_data->StandClosureBtnText = "舞台急停";
  cs_data->SeatRiseBtnText = NULL;
  cs_data->SeatTipBtnText = NULL;

  cs_data->ManufacturingTel = "12345678";
  cs_data->SalehAfterotline = "12345679";
  cs_data->SolveProblem = "Solution";
  cs_data->Description = "Condition";
  cs_data->PopupTitlie = "Inspection personnel must perform the inspection in a timely manner\n"
                            "            according to the inspection time and contents";
  cs_data->PopupContent = " Maintenance\n"
                            "normal report";

  CreateContractionPageBg(cs_data, device_num);

}

/**
 * 升降旋转舞台详情页面数据
 * @param device_num            设备编号
*/
void CreateRevolvingPage(uint8_t device_num)
{

  rs_data->deviceName = "升降旋转舞台";
  rs_data->intro = "设备简介: 采用伸缩调节，适应不同表演、演出\n"
                   "需求，高效快捷转换\n";
  rs_data->date = "2023-7-4";
  rs_data->period = "6 months";
  rs_data->phoneNumber = "+86 755 26490688";
  rs_data->image = &telescopic_stage_big;
  rs_data->expandBtnText = "上升";
  rs_data->emergencyStopBtnText = "急停";
  rs_data->collapseBtnText = "下降";

  rs_data->StandOpenBtnText = "正转";
  rs_data->StandClosureBtnText = "反转";
  rs_data->SeatRiseBtnText = NULL;
  rs_data->SeatTipBtnText = NULL;

  rs_data->ManufacturingTel = "12345678";
  rs_data->SalehAfterotline = "12345679";
  rs_data->SolveProblem = "Solution";
  rs_data->Description = "Condition";
  rs_data->PopupTitlie = "Inspection personnel must perform the inspection in a timely manner\n"
                            "            according to the inspection time and contents";
  rs_data->PopupContent = " Maintenance\n"
                            "normal report";

  CreateRevolvingPageBg(rs_data, device_num);

}