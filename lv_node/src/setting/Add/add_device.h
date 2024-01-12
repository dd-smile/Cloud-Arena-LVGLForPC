/***
 * @Description:添加设备
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-04-25 15:59:03
 * @LastEditTime: 2023-04-27 15:55:19
 * @FilePath: /SmartBox_Power/lv_node/src/setting/add_device/add_device.h
 */
#ifndef _LV_ADD_DEVICE_H
#define _LV_ADD_DEVICE_H

#define MAX_DEVICES 50  // 定义最大设备数量

typedef struct
{
    uint8_t SlaveAddr; // 从机地址 
    int device_num;       // 设备编号
    int device_sence;      // 设备场景
    int device_type;      // 设备类型
    char device_name[20]; // 设备名称
} add_device;


typedef struct {
    int index;
    int type;
    const char *name;
    uint8_t addr;
} DeviceInfo;


extern add_device add_device_list[MAX_DEVICES]; // 定义一个结构体数组，用来存储添加的设备信息，把所有数组元素初始化为0
extern uint32_t add_device_count;      // 定义一个全局变量，用来记录添加的设备数量

void CreateAddDevice(lv_obj_t *parent);
void add_device_event_cb(lv_event_t *e);


#endif