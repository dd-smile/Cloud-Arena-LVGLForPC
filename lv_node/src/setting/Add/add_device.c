/*
 * @Description:添加设备页面
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-04-25 15:58:59
 * @LastEditTime: 2023-08-30 14:18:12
 * @FilePath: lv_node/src/setting/Add/add_device.c
 */
#include "ui_app.h"
add_device add_device_list[MAX_DEVICES] = {0}; // 定义一个结构体数组，用来存储添加的设备信息，把所有数组元素初始化为0  暂时没用到
uint32_t add_device_count = 0;                 // 已添加设备的数量   暂时没用到


/**
 * 创建弹出框
 * @param parent           指向父对象的指针  
 * @return                 返回弹出框对象   
 * */
static lv_obj_t *CreateDevicePopupBox(lv_obj_t *parent)
{
    lv_obj_t *mask = lv_c_create_mask_box(parent);               // 创建遮罩
    lv_obj_t *iamge = image_create(mask, &DevicePopupbox, 0, 0); // 创建图标
    lv_obj_t *label = lv_label_create(iamge);                    // 创建标题
    lv_label_set_text(label, UI_MLANG_STR(ADD_DEVICE));
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_24_text, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 250, 20);

    lv_obj_t *confirm_btn = btn_create_text(iamge, false, "OK", 200, 302);  // 创建按钮
    lv_obj_add_event_cb(confirm_btn, lv_back_Mask, LV_EVENT_CLICKED, NULL); // 添加返回事件
    lv_obj_t *Back_btn = btn_create_text(iamge, false, "Cancel", 340, 302); // 创建按钮
    lv_obj_add_event_cb(Back_btn, lv_back_Mask, LV_EVENT_CLICKED, NULL);    // 添加返回事件

    return iamge;
}

/**
 * 点击添加图片事件
 * @param e             指向事件描述符的指针     
 * */
void add_device_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_t *obj = CreateDevicePopupBox(lv_scr_act());   //创建弹出框
        CreateDropDownList(obj);   //创建下拉列表
    }
}

/**
 * 创建增加设备的卡片
 * @param parent         指向父对象的指针
 * */
void CreateAddDevice(lv_obj_t *parent)
{
    lv_obj_t *ImageObj = CreateOBJclick(parent);
    image_create(parent, &Add, -100, -30);
    card_create_24_text(parent, UI_MLANG_STR(ADD_DEVICE), 2, -30);               // 创建标题
    card_create_16_text(parent, UI_MLANG_STR(PLEASE_ADD_A_NEW_DEVICE), -16, 35); // 创建标题
    lv_obj_add_event_cb(ImageObj, add_device_event_cb, LV_EVENT_ALL, NULL);
}
