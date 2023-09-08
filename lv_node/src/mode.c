/*
 * @Description:模式切换页面
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-03-09 11:28:42
 * @LastEditTime: 2023-08-25 10:34:31
 * @FilePath: /new_home_smart/lv_node/src/mode.c
 */

#include "ui_app.h"
lv_obj_t *home_modes[4];   //暂时没有用到
static lv_obj_t *mode_labels[4];   //模式卡片模板

// 事件   暂时没有用到
static void toggle_label(lv_obj_t *label, bool state)
{
    const char *text = state ? "已开启" : "未开启";
    lv_color_t color = state ? lv_color_hex(0x14E7F0) : lv_color_hex(0xffffff);

    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, color, 0);
}

/**
 * 设置模式图片是开启还是关闭
 * @param obj           指向事件最初针对的对象(没有开启冒泡的话，就是事件注册对象)
 * @param index         模式id号
 * @param type          状态类型
 * */
static void toggle_image(lv_obj_t *obj, uint8_t index, bool type)
{
    static const lv_img_dsc_t *mode_images[] = {
        &mode2_on, &mode1_on, &mode3_on,
        &mode2_off, &mode1_off, &mode3_off
    };    //六张模式图片

    uint8_t image_index = (type ? index + 3 : index);  //类型为０，返回ｏｆｆ。类型为１，返回ｏｎ
    lv_img_set_src(obj, mode_images[image_index]);
}

/**
 * 点击切换模式事件
 * @param e           指向事件描述符的指针     
 * */
static void lv_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);    //事件最初针对的对象(没有开启冒泡的话，就是事件注册对象)  mode_images[i]

    //如果发生点击事件
    if (code == LV_EVENT_CLICKED)
    {
        static bool states[] = {false, false, false, false};
        for (int i = 0; i < 3; i++)
        {
            // 如果等于事件最初针对的对象(被点击的卡片)
            if (obj == mode_labels[i]->parent)  // mode_labels[i]->parent  =  mode_images[i]
            {
                states[i] = !states[i];         //标志位置为true
                toggle_image(obj, i, 0);        //开启的图片
            }
            else
            {
                states[i] = false;
                toggle_image( mode_labels[i]->parent, i, 1);    //关闭的图片
            }
        }
    }
}

/**
 * 初始化模式卡片-状态文本的位置
 * @param obj            指向对象的指针，它将是新标签的父对象。
 * @return               返回一个指向已初始化完成的模式卡片模板
 * */
static lv_obj_t *lv_d_label_caeate(lv_obj_t *obj)
{
    lv_obj_t *label = lv_label_create(obj);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(label, "");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);    //默认状态
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(0x14E7F0), LV_STATE_CHECKED);    //选中状态
    //添加两种标志位
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    return label;
}

/**
 * 设置模式卡片名字
 * @param obj            指向对象的指针，它将是新标签的父对象。
 * @param txt            标签对象的文本
 * @return               返回一个指向设置好名字的卡片
 * */
static lv_obj_t *lv_c_label_create(lv_obj_t *obj, char *txt)
{
    lv_obj_t *label = lv_label_create(obj);
    lv_label_set_text(label, txt);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);
    return label;
}

/**
 * 创建模式卡片
 * @param parent         指向一个对象的指针，它将是新图像的父对象 
 * @param x              要设置的x坐标
 * @param y              要设置的y坐标
 * @return               返回一个指向卡片的指针 
 * */
static lv_obj_t *lv_home_card_create(lv_obj_t *parent, int x, int y)
{
    lv_obj_t *card = lv_img_create(parent);
    lv_img_set_src(card, &card_background);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_style_bg_opa(card, LV_OPA_0, 0);
    return card;
}

/**
 * 创建模式页面
 * @param obj         指向父对象的指针
 * */
void CreateModePage(lv_obj_t *obj)
{
    static const char *mode_names[] = {"Training Model", "Match Model", "Entertainment Model"};   //要切换模式的名字
    static lv_obj_t *mode_cards[3];   //模式卡片
    static lv_obj_t *mode_images[3];  //模式卡片图片

    for (int i = 0; i < 3; i++)
    {
        mode_cards[i] = lv_home_card_create(obj, 30 + 270 * i, 150);       //创建三张模式卡片
        //以创建好的模式卡片作为父类，赋予模式的图片
        mode_images[i] = lv_img_create(mode_cards[i]);                     
        lv_img_set_src(mode_images[i], i == 0 ? &mode2_on : i == 1 ? &mode1_off
                                                        :  &mode3_off
                                                                   );
        lv_c_label_create(mode_cards[i], mode_names[i]);          //设置卡片名字
        mode_labels[i] = lv_d_label_caeate(mode_images[i]);       //初始化生成模式卡片模板
        lv_obj_add_event_cb(mode_images[i], lv_event_handler, LV_EVENT_ALL, NULL);
    }
}
