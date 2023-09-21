/*** 
 * @Description: 
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-03 15:48:51
 * @LastEditTime: 2023-07-03 15:49:00
 * @FilePath: /new_home_smart/lv_node/src/device/device.h
 */
#ifndef __DEVICE_H__
#define __DEVICE_H__

//设备号按钮号键值对
typedef struct
{
    uint8_t num; //设备号
    uint8_t ind; //按钮号
}index_t;

index_t *add_index_t(uint8_t num, uint8_t ind);
void CreateDevicePage(lv_obj_t *parent);

void SetSerialDo(uint8_t addr, uint8_t SetSerialInedx, uint8_t status);

#endif // __DEVICE_H__