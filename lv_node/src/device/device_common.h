/*** 
 * @Description: 
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-03 17:20:08
 * @LastEditTime: 2023-07-04 11:15:53
 * @FilePath: /new_home_smart/lv_node/src/device/device_common.h
 */
#ifndef __DEVICE_COMMON_H__
#define __DEVICE_COMMON_H__


typedef struct {
    const char *deviceName;
    const char *intro;
    const char *date;
    const char *period;
    const char *phoneNumber;
    const char *title;
    const lv_img_dsc_t *image;
    const char *expandBtnText;
    const char *emergencyStopBtnText;
    const char *collapseBtnText;

    const char *StandOpenBtnText;
    const char *StandClosureBtnText;
    const char *SeatRiseBtnText;
    const char *SeatTipBtnText;

    const char *ManufacturingTel;
    const char *SalehAfterotline;
    const char *SolveProblem;
    const char *Description;
    const char *PopupTitlie;
    const char *PopupContent;

} DevicePageData;
extern  DevicePageData *pPageData;
extern lv_obj_t *Input_label;


lv_obj_t *CreateDevicePageBg(const DevicePageData *data);
lv_obj_t *CreateBreakdownPageBg(lv_obj_t *parent);
 lv_obj_t *CreateInspectTable(lv_obj_t *parent);
#endif // __DEVICE_COMMON_H__