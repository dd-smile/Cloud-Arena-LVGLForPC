/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-03 17:15:24
 * @LastEditTime: 2023-07-05 11:37:54
 * @FilePath: /new_home_smart/lv_node/src/device/SmartSeat.c
 */
#include "ui_app.h"

void CreateSmartSeatPage(void)
{

  DevicePageData data;
  data.deviceName = "Underground Seating System";
  data.intro = "Overview: 使用纹路防滑,加纤材\n"
               "料,并且支持急速安装.适用于中大型\n"
               "体育场馆,体育中心,会展中心,奥体中\n"
               "心等.";
  data.date = "2023-7-4";
  data.period = "6 months";
  data.phoneNumber = "+86 755 26490688";
  ;
  data.image = &Smart_seat_small;
  data.expandBtnText = "Open";
  data.emergencyStopBtnText = "Stop";
  data.collapseBtnText = "Close";

  data.StandOpenBtnText = "StandOpen";
  data.StandClosureBtnText = "StandClose";
  data.SeatRiseBtnText = "SeatRise";
  data.SeatTipBtnText = "SeatTip";

  CreateDevicePageBg(&data);

}
