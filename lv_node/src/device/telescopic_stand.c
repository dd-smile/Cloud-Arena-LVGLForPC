/*
 * @Description: 伸缩活动看台
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-03 13:58:49
 * @LastEditTime: 2023-07-05 11:39:31
 * @FilePath: /new_home_smart/lv_node/src/device/telescopic_stand.c
 */
#include "ui_app.h"
DevicePageData lvPageData = {0};
DevicePageData *pPageData = &lvPageData;

void CreateTelescopicStandPage(void)
{

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

    CreateDevicePageBg(pPageData);
}
