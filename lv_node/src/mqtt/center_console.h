/*
 * @Description: 中央控制系统，用于接收平台端下发的指令，并执行相应的操作
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-05-16 15:32:17
 * @LastEditTime: 2024-05-23 15:59:05
 * @FilePath: lv_node/src/mqtt/center_console.h
 */
#ifndef _CENTER_CONSOLE_H_
#define _CENTER_CONSOLE_H_


void OneNet_ParseJsonNow(char *msg, Soundlight_Data *info);
void OneNet_ParseJsonMulti(const char *json_string, Soundlight_Data **devices, size_t *device_count);
char* construct_json_string(const Soundlight_Data *info);
char* construct_json_stringMulti(const Soundlight_Data *devices, size_t device_count);

int Query_Device_type(const char *device_type);
void TICS_Issue_instruction(int dev, const char *pid, const char *vid);

#endif