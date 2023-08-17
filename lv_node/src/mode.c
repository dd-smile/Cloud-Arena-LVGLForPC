/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:42
 * @LastEditTime: 2023-07-05 10:34:31
 * @FilePath: /new_home_smart/lv_node/src/mode.c
 */

#include "ui_app.h"
lv_obj_t *home_modes[4];
static lv_obj_t *mode_labels[4];
// 事件
static void toggle_label(lv_obj_t *label, bool state)
{
    const char *text = state ? "已开启" : "未开启";
    lv_color_t color = state ? lv_color_hex(0x14E7F0) : lv_color_hex(0xffffff);

    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, color, 0);
}
static void toggle_image(lv_obj_t *obj, uint8_t index, bool type)
{
    static const lv_img_dsc_t *mode_images[] = {
        &mode2_on, &mode1_on, &mode3_on,
        &mode2_off, &mode1_off, &mode3_off
    };

    uint8_t image_index = (type ? index + 3 : index);
    lv_img_set_src(obj, mode_images[image_index]);
}

static void lv_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        static bool states[] = {false, false, false, false};
        for (int i = 0; i < 3; i++)
        {
            if (obj == mode_labels[i]->parent)
            {
                states[i] = !states[i];
                toggle_image(obj, i, 0);
            }
            else
            {
                states[i] = false;
                toggle_image( mode_labels[i]->parent, i, 1);
            }
        }
    }
}

static lv_obj_t *lv_d_label_caeate(lv_obj_t *obj)
{
    lv_obj_t *label = lv_label_create(obj);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(label, "");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(0x14E7F0), LV_STATE_CHECKED);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    return label;
}

static lv_obj_t *lv_c_label_create(lv_obj_t *obj, char *txt)
{
    lv_obj_t *label = lv_label_create(obj);
    lv_label_set_text(label, txt);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);
    return label;
}
static lv_obj_t *lv_home_card_create(lv_obj_t *parent, int x, int y)
{
    lv_obj_t *card = lv_img_create(parent);
    lv_img_set_src(card, &card_background);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_style_bg_opa(card, LV_OPA_0, 0);
    return card;
}

void CreateModePage(lv_obj_t *obj)
{
    static const char *mode_names[] = {"Training Model", "Match Model", "Entertainment Model"};
    static lv_obj_t *mode_cards[3];
    static lv_obj_t *mode_images[3];

    for (int i = 0; i < 3; i++)
    {
        mode_cards[i] = lv_home_card_create(obj, 30 + 270 * i, 150);
        mode_images[i] = lv_img_create(mode_cards[i]);
        lv_img_set_src(mode_images[i], i == 0 ? &mode2_on : i == 1 ? &mode1_off
                                                        :  &mode3_off
                                                                   );
        lv_c_label_create(mode_cards[i], mode_names[i]);
        mode_labels[i] = lv_d_label_caeate(mode_images[i]);
        lv_obj_add_event_cb(mode_images[i], lv_event_handler, LV_EVENT_ALL, NULL);
    }
}
