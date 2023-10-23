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

unsigned char packet_open[12] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0xf0, 0x05, 0x00, 0x01, 0xFF, 0x00};
unsigned char packet_close[12] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0xf0, 0x05, 0x00, 0x02, 0xFF, 0x00};

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
        case 0: //看台收拢才能打开球架
            if (stop_flag == false) 
            {
                // sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"Hanging_ball_rack_down_1\",\"v\":\"%d\"}]}",1);
                // OneNet_Publish("/usr/plcnet/Cloud_Arena_sports/edge/d", PUB_BUF);
                write(light_fd, packet_open, sizeof(packet_open));
            }
            break;
        case 1:  //急停
            if (stop_flag == false )
            {
                stop_flag = true;   //表示急停产生
                lv_obj_add_state(obj, LV_STATE_PRESSED);  //添加长按属性，使得按钮保持被点击着的样子
                sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"Hanging_ball_rack_stop_1\",\"v\":\"%d\"}]}",1);
                OneNet_Publish("/usr/plcnet/Cloud_Arena_sports/edge/d", PUB_BUF);
            }
            else
            {
                stop_flag = false;
                lv_obj_clear_state(obj, LV_STATE_PRESSED);  //移除长按属性
            }
            break;
        case 2:  //收拢球架
            if (stop_flag == false)
            {   
                write(light_fd, packet_close, sizeof(packet_close));
                // sprintf(PUB_BUF,"{\"f\":\"s\",\"d\":[{\"sid\":\"FX3U_128MT_sports\",\"pid\":\"Hanging_ball_rack_up_1\",\"v\":\"%d\"}]}",1);
                // OneNet_Publish("/usr/plcnet/Cloud_Arena_sports/edge/d", PUB_BUF);
            }
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
    bs_data->phoneNumber = "+86 755 26490688";
    bs_data->image = &basketball_stands_big;  
    bs_data->expandBtnText = "一键打开";
    bs_data->emergencyStopBtnText = "急停";
    bs_data->collapseBtnText = "一键收合";

    bs_data->StandOpenBtnText = NULL;
    bs_data->StandClosureBtnText = NULL;
    bs_data->SeatRiseBtnText = NULL;
    bs_data->SeatTipBtnText = NULL;

    bs_data->ManufacturingTel = "12345678";
    bs_data->SalehAfterotline = "12345679";
    bs_data->SolveProblem = "解决方案" ;
    bs_data->Description = "Condition";
    bs_data->PopupTitlie = "检验人员必须及时进行检验\n"
                             "            根据检查时间和内容";
    bs_data->PopupContent = "维护\n"
                              "正常报告";

    CreatebasketballPageBg(bs_data, device_num);
}