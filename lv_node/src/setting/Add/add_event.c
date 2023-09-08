/*
 * @Description:
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-05-16 18:13:10
 * @LastEditTime: 2023-08-31 17:09:44
 * @FilePath: /SmartBox_Power/lv_node/src/setting/Add/add_event.c
 */
#include "ui_app.h"


/**
 * 设备地址选项事件
 * @param e             指向事件描述符的指针    
 * */
static void device_addr_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) // 当下拉列表的值改变时
    {
        add_device_list[add_device_count].SlaveAddr = lv_dropdown_get_selected(obj); // 获取下拉列表选中的值
        printf("设备地址：%d\n",add_device_list[add_device_count].SlaveAddr);
    }
}

/**
 * 设备编号选项事件
 * @param e             指向事件描述符的指针    
 * */
static void device_num_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) // 当下拉列表的值改变时
    {
        add_device_list[add_device_count].device_num = lv_dropdown_get_selected(obj); // 获取下拉列表选中的值
        printf("设备编号：%d\n",add_device_list[add_device_count].device_num);

    }
}

/**
 * 设备场景选项事件
 * @param e             指向事件描述符的指针    
 * */
static void device_sence_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        add_device_list[add_device_count].device_sence = lv_dropdown_get_selected(obj);
        printf("设备场景：%d\n",add_device_list[add_device_count].device_sence);
    }
}

/**
 * 设备类型选项事件
 * @param e             指向事件描述符的指针    
 * */
static void device_type_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        add_device_list[add_device_count].device_type = lv_dropdown_get_selected(obj);
        printf("设备类型：%d\n",add_device_list[add_device_count].device_type);
    }
}


/**
 * 创建下拉列表标题
 * @param parent         指向父对象的指针
 * @param text           列表标题文本
 * @param y              y轴的偏移量
 * @return               返回指向列表标题的指针
 * */
lv_obj_t *CreateDownListLabel(lv_obj_t *parent, const char *text, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 40, y);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text(label, text);
    lv_obj_clean(label);
    return label;
}

/**
 * 创建设备类型的选择框和设备名称的输入框
 * @param parent        指向父对象的指针
 * @param options       一个带有'\n'分隔选项的字符串  如　one\n two\n three\n
 * @param x_ofs         x轴的偏移量
 * @param y_ofs         y轴的偏移量
 * @param event_cb      要注册的事件
 * */
void CreateDeviceType(lv_obj_t *parent, const char *options, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb)
{
    lv_obj_t *dd = lv_dropdown_create(parent);
    lv_dropdown_set_options(dd, options);  //从字符串中设置下拉列表中的选项
    lv_obj_set_size(dd, 390, 28);
    lv_obj_align(dd, LV_ALIGN_TOP_MID, x_ofs, y_ofs);
    lv_obj_set_style_radius(dd, 0, LV_PART_MAIN);
    lv_obj_set_style_border_color(dd, lv_color_hex(0x485067), LV_PART_MAIN);
    lv_obj_set_style_border_width(dd, 1, LV_PART_MAIN);
    lv_obj_set_style_text_font(dd, &fout_14_text, LV_PART_MAIN);
    lv_obj_set_style_text_color(dd, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_pad_top(dd, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(dd, lv_color_make(20, 26, 35), LV_PART_MAIN);
    lv_dropdown_set_symbol(dd, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_color(dd, lv_color_hex(0xffffff), LV_PART_SELECTED);

    lv_obj_t *list = lv_dropdown_get_list(dd);  //获取下拉列表以允许进行样式化或其他修改
    lv_obj_set_style_radius(list, 5, LV_PART_MAIN);
    lv_obj_set_style_text_font(list, &fout_16_text, LV_PART_MAIN);
    lv_obj_set_style_bg_color(list, lv_color_hex(0x141A23), LV_PART_MAIN);
    lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(list, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_add_event_cb(dd, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

/**
 * 输入设备名称事件
 * @param e             指向事件描述符的指针    
 * */
static void DeviceTextareaEvent(lv_event_t *e)
{

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) // 如果是点击事件
    {
        printf("输入设备名称\n");
        lv_keypage_create(lv_scr_act());   //需要更改实现方式，有死机ｂｕｇ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
}

/**
 * 创建添加设备名称的输入框
 * @param parent          指向父对象的指针
 * @param c               提示词
 * @param x_ofs           ｘ坐标偏移量
 * @param y_ofs　　　　　　 ｙ坐标偏移量
 * @param event_cb        要注册的事件
 * @return                返回指向添加设备名称输入框的指针          
 * */
lv_obj_t *lv_device_textarea(lv_obj_t *parent, uint32_t c, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb)
{
    lv_obj_t *DeviceTextarea = lv_obj_create(parent);                                              // 创建文本框
    lv_obj_set_size(DeviceTextarea, 390, 28);                                            // 设置大小
    lv_obj_align(DeviceTextarea, LV_ALIGN_TOP_MID, x_ofs, y_ofs);                        // 设置位置
    lv_obj_set_style_radius(DeviceTextarea, 0, LV_PART_MAIN);                            // 设置圆角
    lv_obj_set_style_border_color(DeviceTextarea, lv_color_hex(0x485067), LV_PART_MAIN); // 设置边框颜色
    lv_obj_set_style_border_width(DeviceTextarea, 1, LV_PART_MAIN);                      // 设置边框宽度
    lv_obj_set_style_text_font(DeviceTextarea, &PuHuiTi_Regular_16, LV_PART_MAIN);       // 设置字体
    lv_obj_set_style_text_color(DeviceTextarea, lv_color_hex(0xffffff), LV_PART_MAIN);   // 设置字体颜色
    lv_obj_set_style_bg_color(DeviceTextarea, lv_color_make(20, 26, 35), LV_PART_MAIN);
    lv_obj_add_event_cb(DeviceTextarea, event_cb, LV_EVENT_ALL, NULL); // 添加事件
    lv_obj_set_style_bg_opa(DeviceTextarea, LV_OPA_TRANSP, LV_PART_SCROLLBAR);
    lv_obj_t *DeviceTextareaLabel = lv_label_create(DeviceTextarea); // 创建标签
    lv_label_set_text(DeviceTextareaLabel, "");
    lv_obj_align(DeviceTextareaLabel, LV_ALIGN_LEFT_MID, -10, 0);
    return DeviceTextarea;
}


/**
 * 创建下拉列表
 * @param parent          指向父对象的指针
 * */
lv_obj_t *CreateDropDownList(lv_obj_t *parent)
{
    char device_options[256] = "Please select a device id\n";
    char SlaveAddr[256] = "Please select the device address\n";

    for (int i = 1; i <= MAX_DEVICES; i++)
    {
        char device_address[4];
        snprintf(device_address, sizeof(device_address), "%02d\n", i);   //读取ｉ，并按两位输出(不足补０)\n 到device_address中

        //strcat(char *dest,const char *src)把src所指向的字符串追加到dest所指向的字符串结尾
        strcat(device_options, device_address);   
        strcat(SlaveAddr, device_address);
    }
    
    //创建下拉列表标题
    CreateDownListLabel(parent, "Slave Address", -95);
    CreateDownListLabel(parent, "Device ID", -50);
    CreateDownListLabel(parent, "Sence", -5);
    CreateDownListLabel(parent, "Model", 40);
    CreateDownListLabel(parent, "Name", 85);

    //创建设备类型的选择框和设备名称的输入框
    CreateDeviceType(parent, SlaveAddr, 75, 70, device_addr_event);
    CreateDeviceType(parent, device_options, 30, 115, device_num_event);
    CreateDeviceType(parent, DEVICE_SENCE, 30, 160, device_sence_event);
    CreateDeviceType(parent, DEVICE_MODEl, 30, 205, device_type_event);
    lv_device_textarea(parent, DEVICE_NAME, 30,250, DeviceTextareaEvent);
}


