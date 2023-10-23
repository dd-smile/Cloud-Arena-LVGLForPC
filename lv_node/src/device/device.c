/*
 * @Description:
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-07-03 15:48:41
 * @LastEditTime: 2023-08-24 12:00:10
 * @FilePath: /new_home_smart/lv_node/src/device/device.c
 */
#include "ui_app.h"

static void lv_create_device(lv_event_t *e);

/**
 * 用于传入设备号和按钮号
 * @param num          设备号
 * @param ind          按钮号
 * @return             返回存有设备号和按钮号的结果体
 * */
index_t *add_index_t(uint8_t num, uint8_t ind)
{
    index_t *index = (index_t *)malloc(sizeof(index_t));

    index->num = num;
    index->ind = ind;

    return index;
}

//这个函数是开发板串口发送的，加上这个防止报错
void SetSerialDo(uint8_t addr, uint8_t SetSerialInedx, uint8_t status)
{
    unsigned char packet[8];

    packet[0] = addr;
    packet[1] = 0x05;
    packet[2] = 0x00;
    packet[3] = SetSerialInedx;
    if(status == 1)
        packet[4] = 0xFF;  //开
    else    
        packet[4] = 0;  //关
    packet[5] = 0x00;
    //计算校验码
    //packet[6]
    //packet[7]

    usleep(100 * 1000);   //休眠
}



/**
 * 设置样式
 * @param obj         指向要设置对象的指针
 * */
static void set_common_style(lv_obj_t *obj)
{
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(obj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(obj, 0, LV_STATE_DEFAULT);
}

/**
 * 创建按钮
 * @param obj         指向父对象的指针    
 * @param text        按钮的名称
 * @param x_ofs       水平偏移 x_ofs
 * @param type        设备类型
 * @param index       (传入设备号和按钮号，取出按钮号)按到第几个按钮
 * */
static void create_button(lv_obj_t *obj, const char *text, lv_coord_t x_ofs, uint8_t type, index_t *index)
{
    lv_obj_t *btn = lv_obj_create(obj);
    lv_obj_set_size(btn, 55, 30);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(btn, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, 80, LV_PART_MAIN);
    
    //判断是什么设备按钮，从而添加控制事件
    switch(type)
    {
        //首页卡片的按钮
        case 0:   //活动看台
            lv_obj_add_event_cb(btn, telescoopic_Controls_event_cb, LV_EVENT_ALL, index);  //传入设备号和按钮号
            break;
        case 1:   //悬空球架
            lv_obj_add_event_cb(btn, basketball_Controls_event_cb, LV_EVENT_ALL, index);  
            break;
        default:
            break;
    }
    
    
    lv_obj_align(btn, LV_ALIGN_CENTER, x_ofs, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_center(label);
}


/**
 * 创建矩形对象，作为按钮的模板
 * @param obj           指向父对象的指针    
 * @return              返回一个矩形新对象
 * */
static lv_obj_t *swich_btn_card(lv_obj_t *obj)
{
    lv_obj_t *box = lv_obj_create(obj);        //创建一个矩形对象
    lv_obj_set_size(box, 201, 70);
    lv_obj_set_style_bg_color(box, lv_color_hex(0x1A1F2E), LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(box, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(box, 10, LV_STATE_DEFAULT);          //设置背景圆角半径
    lv_obj_clear_flag(box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(box, LV_ALIGN_BOTTOM_MID, 0, 0);
    return box;
}

/**
 * 创建一个卡片页面模板，作为实际卡片的父类
 * @param parent         指向一个对象的指针，它将是新图像的父对象  
 * @param x              设置卡片x坐标，与卡片宽度相乘形成新的x坐标。
 * @param y              设置卡片y坐标，与卡片高度相乘形成新的y坐标
 * @param text           按钮1的名称
 * @param text1          按钮2的名称
 * @param text2          按钮3的名称
 * @param index          存有设备号和按钮号的结构体
 * @return               返回一个指向卡片的指针
 * */
static lv_obj_t *lv_box_create(lv_obj_t *parent, int x, int y, const char *text, const char *text1, const char *text2, index_t *index)
{
    lv_obj_t *card = lv_img_create(parent);       //创建一个图像对象
    lv_img_set_src(card, &card_background);

    index_t *index0 = add_index_t(index->num, 0);   //传入设备号, 按钮号为０
    index_t *index1 = add_index_t(index->num, 1);
    index_t *index2 = add_index_t(index->num, 2);

    lv_obj_t *btn_card = swich_btn_card(card);

    //第一次传入的是设备号和设备类型
    if (text != NULL)
    {
        create_button(btn_card, text, -60, index->ind, index0);  
    }
    if (text1 != NULL)
    {
        create_button(btn_card, text1, 0, index->ind, index1);
    }
    if (text2 != NULL)
    {
        create_button(btn_card, text2, 60, index->ind, index2);
    }

    lv_obj_set_pos(card, LV_CARD_WIDTH * x, LV_CARD_HEIGHT * y);
    return card;
}

/**
 * 创建卡片页面,用于展示要控制的设备
 * @param parent          指向父对象的指针 
 * @param x               设置卡片x坐标
 * @param y               设置卡片y坐标
 * @param name            卡片类名称
 * @param image           要显示的图片id号(设备类型)
 * @param device_num      设备号
 * @return                返回一个指向卡片类按钮的指针
 * */
static lv_obj_t *card_class(lv_obj_t *parent, int x, int y, const char *name, uint8_t image, uint8_t device_num)
{

    index_t *index = add_index_t(device_num, image);   //这里为设备号和设备类型

    //创建设备卡片背景
    lv_obj_t *dev_card;
    if(image == 0)  //如果设备是活动看台
        dev_card = lv_box_create(parent, x, y, "展开", "急停", "收合", index);
    else if(image == 1)  //如果设备是悬空球架
        dev_card = lv_box_create(parent, x, y, "放下", "急停", "收起", index);
    else if(image == 2)  //如果设备是分隔幕
        dev_card = lv_box_create(parent, x, y, "放下", "急停", "收起", index);
    else if(image == 3)  //如果设备是灯具
        dev_card = lv_box_create(parent, x, y, "打开", "关闭", NULL, index);

    printf("index:%d %d\n", index->num, index->ind);

    lv_obj_t *cont = lv_obj_create(dev_card);
    lv_obj_set_size(cont, 107, 107);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, -20);
    set_common_style(cont);

    //选择设备图片
    lv_obj_t *imgbutton = lv_img_create(cont);
    lv_obj_align(imgbutton, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(imgbutton, LV_OBJ_FLAG_SCROLLABLE);

    static const lv_img_dsc_t *img_table[] = {&telescopic_stand_small, &basketball_stands_small, &partition_curtain_small}; //设备图片数组
    lv_img_set_src(imgbutton, img_table[image]);
    lv_obj_add_flag(imgbutton, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(imgbutton, lv_create_device, LV_EVENT_ALL, (void *)index);

    lv_obj_t *label = lv_label_create(dev_card);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text(label, name);
    set_common_style(label);

    return dev_card;
}

/**
 * 创建设备详情说明的事件
 * @param e             指向事件描述符的指针     
 * */
static void lv_create_device(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    index_t *user_data = lv_event_get_user_data(e);

    uint8_t type = user_data->ind;  //设备类型传入的　index_t *index = add_index_t(device_num, image);　前一个为设备号，后一个为设备类型。与定义顺序相反了。
    uint8_t device_num = user_data->num; //设备号

    //如果发生点击事件
    if (code == LV_EVENT_CLICKED)
    {
        switch (type)
        {
        case 0:
            CreateTelescopicStandPage(device_num);        //创建活动看台页面
            break;

        case 1:
            CreateBasketballStandsPage(device_num);       //创建悬空球架页面
            break;
        default:
            break;
        }
    }
}

/**
 * 创建设备页面
 * @param parent            指向父对象的指针     
 * */
void CreateDevicePage(lv_obj_t *parent)
{

    card_class(parent, 0, 0,"活动看台", 0, 1);      //创建活动看台卡片   倒数第二个参数是设备类型，倒数第一个是设备号
    card_class(parent, 1, 0,"悬空球架", 1, 1);
    card_class(parent, 2, 0,"自动分隔幕", 2, 1);
}