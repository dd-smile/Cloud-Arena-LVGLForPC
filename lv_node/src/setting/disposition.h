/*** 
 * @Description: json转换库
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-06-16 09:55:17
 * @LastEditTime: 2023-04-01 09:58:06
 * @FilePath: lv_node/src/setting/disposition.h
 */
#ifndef __DISPOSITION_H__
#define __DISPOSITION_H__

void updateSettingData(lv_setting_data_t* settingData, const char* jsonFilePath) ;
void saveSettingData(const lv_setting_data_t* settingData, const char* jsonFilePath) ;
void print_setting(lv_setting_data_t* settingData);
void readSettingData(lv_setting_data_t *settingData, const char *jsonFilePath);

#endif