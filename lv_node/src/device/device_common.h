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
    const char *deviceName;   //设备名称
    const char *intro;  //设备简介
    const char *date;   //设备购置日期
    const char *period; //设备检修周期
    const char *phoneNumber;   //制造商服务电话
    const char *title;   
    const lv_img_dsc_t *image;  //设备图片
    const char *expandBtnText;  //展开按钮文本
    const char *emergencyStopBtnText;  //急停按钮文本
    const char *collapseBtnText;   //收合按钮文本

    const char *StandOpenBtnText;  //看台展开按钮文本
    const char *StandClosureBtnText; //看台收合按钮文本
    const char *SeatRiseBtnText;  //座椅展开按钮文本
    const char *SeatTipBtnText;   //座椅收合按钮文本

    const char *ManufacturingTel;   //制造商服务电话
    const char *SalehAfterotline;   //制造商售后电话
    const char *SolveProblem;    //解决措施初始文本
    const char *Description;     //设备故障现象描述初始文本
    const char *PopupTitlie;     //检修正常申报标题
    const char *PopupContent;    //检修正常申报内容

} DevicePageData;    //设备页面数据结构体
extern DevicePageData *pPageData;
extern DevicePageData *bs_data;
extern lv_obj_t *Input_label;


lv_obj_t *CreateDevicePageBg(const DevicePageData *data, uint8_t device_num);
lv_obj_t *CreatebasketballPageBg(const DevicePageData *data, uint8_t device_num);
lv_obj_t *CreateBreakdownPageBg(lv_obj_t *parent);
lv_obj_t *BsCreateBreakdownPageBg(lv_obj_t *parent);
lv_obj_t *CreateInspectTable(lv_obj_t *parent);
#endif // __DEVICE_COMMON_H__