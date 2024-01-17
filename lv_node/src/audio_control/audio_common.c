/*
 * @Description: 音频通用控制
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-12-25 11:28:17
 * @LastEditTime: 2023-01-12 15:59:05
 * @FilePath: lv_node/src/audio_control/audio_common.c
 */
#include "ui_app.h"


static void *LedControl(lv_obj_t *parent);
static void *MultitrackControl(lv_obj_t *parent);

/**
 * 标准crc16校验，   crc-modbus
 * 低字节在前，高字节在后:  & 0xFF            >> 8
 * @param data       要检验的数据
 * @param length     数据长度
*/
uint16_t calculate_crc16(uint8_t *data, size_t length)
{
   uint16_t crc = 0xFFFF;
   
   for (size_t i = 0; i < length; i++)
   {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
        {
            if(crc & 0x0001)
            {
                    crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
   }

   return crc;
}

/**点击卡片事件**/
static void add_Audio_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_current_target(e);   //获取当前点击对象

    if (code == LV_EVENT_CLICKED)
    {
        switch ((int)obj->user_data)
        {
        case 0:  //led控制
            LedControl(lv_scr_act());
            break;

        case 1:   //多轨控制
            MultitrackControl(lv_scr_act());
            break;

        default:
            break;
        }
    }
}

/**LED大屏控制按钮点击事件**/
static void led_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_current_target(e);   //获取当前点击对象

    if (code == LV_EVENT_CLICKED)
    {
        //判断是哪个按钮，进行封装数据
        switch ((int)obj->user_data)
        {
            case 0: 
                SetLedinputsource_aoto(0x01, 0x01, 0x00);   //图层２，卡槽２，第１路输入
                break;

            case 1:  
                SetLedinputsource_aoto(0x01, 0x01, 0x01);
                break;
        
            case 2:
                SetLedinputsource_aoto(0x01, 0x01, 0x02);
                break;
            case 3:
                SetLedinputsource_aoto(0x01, 0x01, 0x03);
                break;
            case 4:
                synchronous_mutework();
                break;
            case 5:
                synchronous_unmutework();
                break;

            default:
                break;
        }
    }

}

/**
 * 创建矩形对象，作为按钮的模板
 * @param obj           指向父对象的指针    
 * @return              返回一个矩形新对象
 * */
static lv_obj_t *swich_btn_card(lv_obj_t *obj)
{
    lv_obj_t *box = lv_obj_create(obj);        //创建一个矩形对象
    lv_obj_set_size(box, 500, 300);
    lv_obj_set_style_bg_color(box, lv_color_hex(0x1A1F2E), LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(box, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(box, 10, LV_STATE_DEFAULT);          //设置背景圆角半径
    lv_obj_clear_flag(box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(box, LV_ALIGN_CENTER, 0, 0);
    return box;
}

/**
 * 创建按钮
 * @param obj          指向父对象的指针    
 * @param text         按钮的名称
 * @param x_ofs        水平偏移 x_ofs
 * @param y_ofs        水平偏移 y_ofs
 * @param type         判断是什么控制类型
 * @param index        按到第几个按钮
 * */
static void create_button(lv_obj_t *obj, const char *text, lv_coord_t x_ofs, lv_coord_t y_ofs, uint8_t type, lv_event_cb_t event_cb, uint8_t index)
{
    lv_obj_t *btn = lv_obj_create(obj);
    lv_obj_set_size(btn, 70, 40);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(btn, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, 80, LV_PART_MAIN);

    
    //判断是什么设备按钮，从而添加控制事件
    switch (type)
    {
        case 0:     //LED大屏控制
            lv_obj_set_user_data(btn, index);   //设置用户数据，表示哪个按钮
            lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);  
            break;
        case 1:     //多轨控制
            lv_obj_set_user_data(btn, index);   //设置用户数据，表示哪个按钮
            lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);  
            break;

        default:
            break;
    }
    
    lv_obj_align(btn, LV_ALIGN_CENTER, x_ofs, y_ofs);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_center(label);
}

/**
 * 创建LED大屏控制界面
 * @param parent            指向父对象的指针     
 * */
static void *LedControl(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_popup_page(scr, "LED切屏", 0, 0);


    //连接服务器
    create_client_led();

    lv_obj_t *btn_synchronous = swich_btn_card(popup_page);
    create_button(btn_synchronous, "HDMI1", -90, 0, 0, led_Controls_event_cb, 0);
    create_button(btn_synchronous, "HDMI2", 0, 0, 0, led_Controls_event_cb, 1);
    create_button(btn_synchronous, "HDMI3", 90, 0, 0, led_Controls_event_cb, 2);
}

/**
 * 创建多轨控制界面
 * @param parent            指向父对象的指针     
 * */
static void *MultitrackControl(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_popup_page(scr, "多轨控制", 0, 0);

    lv_obj_t *btn_multitrack = swich_btn_card(popup_page);
    create_button(btn_multitrack, "音乐1", -90, -100, 1, multitrack_Controls_event_cb, 0);
    create_button(btn_multitrack, "音乐2", 0, -100, 1, multitrack_Controls_event_cb, 1);
    create_button(btn_multitrack, "音乐3", 90, -100, 1 , multitrack_Controls_event_cb, 2);
    create_button(btn_multitrack, "音乐4", -90, -40, 1 , multitrack_Controls_event_cb, 3);
    create_button(btn_multitrack, "大自然", 0, -40, 1 , multitrack_Controls_event_cb, 4);
    create_button(btn_multitrack, "下一首", 90, -40, 1 , multitrack_Controls_event_cb, 8);
    create_button(btn_multitrack, "停止", 0, 10, 1 , multitrack_Controls_event_cb, 9);
    create_button(btn_multitrack, "音量增", -90, 40, 1 , multitrack_Controls_event_cb, 5);
    create_button(btn_multitrack, "音量减", 90, 40, 1 , multitrack_Controls_event_cb, 6);
    // create_button(btn_multitrack, "初始音量", 0, 100, 1 ,7);
    connect_label = lv_label_create(btn_multitrack);
    static lv_style_t connect_label_style;
    lv_style_reset(&connect_label_style);
    lv_style_init(&connect_label_style);
    lv_style_set_radius(&connect_label_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&connect_label_style, 0); //设置边框宽度
    lv_style_set_text_color(&connect_label_style , lv_palette_main(LV_PALETTE_RED));  // 字体颜色设置为红色
    lv_label_set_long_mode(connect_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 长文本循环滚动
    lv_obj_add_style(connect_label, &connect_label_style, 0); // 给btn_label添加样式
    lv_obj_set_style_text_font(connect_label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_align(connect_label, LV_ALIGN_CENTER, 0, 110);
	lv_label_set_text(connect_label, ""); // 设置文本内容

}


/**
 * 创建卡片页面,用于展示要进入的控制类型
 * @param parent          指向父对象的指针 
 * @param x               设置卡片x坐标
 * @param y               设置卡片y坐标
 * @param name            卡片类名称
 * @param image           要显示的图片id号(控制类型)
 * */
static void *card_audio(lv_obj_t *parent, int x, int y, const char *name, uint8_t image)
{   

    static const lv_img_dsc_t *img_table[] = {&synchronous_control_img, &multitrack_control_img}; //控制类型图片数组

    lv_obj_t *card = lv_img_create(parent);
    lv_img_set_src(card, &card_background);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_style_bg_opa(card, LV_OPA_0, 0);

    lv_obj_t *card_son = lv_img_create(card);
    lv_img_set_src(card_son, img_table[image]);

    lv_obj_t *label = lv_label_create(card);
    lv_label_set_text(label, name);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 3);

    lv_obj_t *label_son = lv_label_create(card_son);
    lv_obj_align(label_son, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(label_son, "");
    lv_obj_set_style_text_color(label_son, lv_color_hex(0xffffff), LV_STATE_DEFAULT);    //默认状态
    lv_obj_set_style_text_font(label_son, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label_son, lv_color_hex(0x14E7F0), LV_STATE_CHECKED);    //选中状态
    //添加两种标志位
    lv_obj_add_flag(card_son, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(card_son, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_user_data(card_son, image);   //设置用户数据，表示进入哪个页面
    lv_obj_add_event_cb(card_son, add_Audio_event_cb, LV_EVENT_ALL, NULL);

}


/**
 * 创建音频控制面板－利亚德
 * @param parent            指向父对象的指针     
 * */
void CreateAudioPage(lv_obj_t *parent)
{
    card_audio(parent, 30, 50, "LED切屏", 0);
    card_audio(parent, 400, 50, "多轨控制", 1);
}