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
 * */
static void create_button(lv_obj_t *obj, const char *text, lv_coord_t x_ofs)
{
    lv_obj_t *btn = lv_obj_create(obj);
    lv_obj_set_size(btn, 55, 30);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(btn, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, 80, LV_PART_MAIN);
    lv_obj_align(btn, LV_ALIGN_CENTER, x_ofs, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_center(label);
}

/**
 * 创建卡片页面,用于展示要控制的设备
 * @param parent          指向父对象的指针      
 * @param name            卡片类名称
 * @param image           要显示的图片id号
 * @return                返回一个指向卡片类按钮的指针
 * */
static lv_obj_t *card_class(lv_obj_t *parent, char *name, uint8_t image)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 107, 107);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    set_common_style(cont);

    lv_obj_t *imgbutton = lv_img_create(cont);
    lv_obj_align(imgbutton, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(imgbutton, LV_OBJ_FLAG_SCROLLABLE);

    static const lv_img_dsc_t *img_table[] = {&Smart_seat_big, &telescopic_stand_small};
    lv_img_set_src(imgbutton, img_table[image]);
    lv_obj_add_flag(imgbutton, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(imgbutton, lv_create_device, LV_EVENT_ALL, image);

    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text(label, name);
    set_common_style(label);

    return imgbutton;
}

/**
 * 创建矩形对象，作为按钮的模板
 * @param obj         指向父对象的指针    
 * @return            返回一个矩形新对象
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
 * @return               返回一个指向卡片的指针
 * */
static lv_obj_t *lv_box_create(lv_obj_t *parent, int x, int y, const char *text, const char *text1, const char *text2)
{
    lv_obj_t *card = lv_img_create(parent);       //创建一个图像对象
    lv_img_set_src(card, &card_background);

    lv_obj_t *btn_card = swich_btn_card(card);

    create_button(btn_card, text, -60);
    create_button(btn_card, text1, 0);
    create_button(btn_card, text2, 60);

    lv_obj_set_pos(card, LV_CARD_WIDTH * x, LV_CARD_HEIGHT * y);
    return card;
}

/**
 * 创建设备详情说明的事件
 * @param e             指向事件描述符的指针     
 * */
static void lv_create_device(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t image = lv_event_get_user_data(e);

    //如果发生点击事件
    if (code == LV_EVENT_CLICKED)
    {
        switch (image)
        {
        case 0:
            CreateSmartSeatPage();        //创建地埋翻折座椅页面
            break;

        case 1:
            CreateTelescopicStandPage();    //创建活动看台页面

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

    card_class(lv_box_create(parent, 0, 0, "Open", "Stop", "Close"), "Retractable Seating System", 1);      //创建活动看台卡片
}