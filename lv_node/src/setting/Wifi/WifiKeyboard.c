/*
 * @Description:输入WIFI密码界面
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-01-30 11:28:06
 * @LastEditTime: 2024-03-20 14:51:43
 * @FilePath: lv_node/src/setting/Wifi/WifiKeyboard.c
 */
#include "ui_app.h"

static WifiCredentials wifiCredentials;

static lv_style_t style_bg_mask;

static void BackParentEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj->parent->parent);
    }
}
static void on_confirm_button_click(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *parent = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED)
    {
        WifiNode *node = getWifiNodeFromListButton(parent);
        strncpy(wifiCredentials.password, lv_textarea_get_text(node->password_textarea), SSID_MAX_LENGTH - 1);
        strcpy(node->password, wifiCredentials.password);// Save password

        connect_wifi(node); // Connect to the WIFI

        lv_obj_del(obj->parent->parent);
    }
}

static void BackEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj);
    }
}

static void SetStyleMask(lv_obj_t *obj)
{
    lv_style_init(&style_bg_mask); // 初始化遮蔽罩风格
    lv_style_set_bg_color(&style_bg_mask, lv_color_hex(0x11141A));
    lv_style_set_bg_opa(&style_bg_mask, 150);
    lv_obj_add_style(obj, &style_bg_mask, LV_PART_MAIN);
}

lv_obj_t *CreateMaskBox(lv_obj_t *parent, bool back)
{
    lv_obj_t *gray_layer = lv_obj_create(parent); // 主页面灰色遮罩层
    lv_obj_remove_style_all(gray_layer);
    lv_obj_set_pos(gray_layer, 0, 0);
    lv_obj_set_size(gray_layer, lv_obj_get_width(parent), lv_obj_get_height(parent));
    SetStyleMask(gray_layer);
    if (back)
    {
        lv_obj_add_event_cb(gray_layer, BackEvent, LV_EVENT_CLICKED, NULL); // 添加返回事件
    }

    return gray_layer;
}


lv_obj_t *CreadWifiPasswordTextarea(lv_obj_t *SrcParent)
{
    lv_obj_t *Wifibackground = lv_obj_create(SrcParent);
    lv_obj_set_size(Wifibackground, 1024, 600);
    lv_obj_align(Wifibackground, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(Wifibackground, lv_color_hex(0x0b1a2e), LV_PART_MAIN);
    lv_obj_set_style_border_width(Wifibackground, 0, LV_PART_MAIN);

    // // Create the password textarea
    lv_obj_t *password_textarea = lv_textarea_create(Wifibackground);
    lv_textarea_set_password_mode(password_textarea, true);
    lv_textarea_set_one_line(password_textarea, true);
    lv_obj_set_width(password_textarea, lv_pct(40));
    lv_obj_set_pos(password_textarea, 225, 100);

    // Create the keyboard
    lv_obj_t *keyboard = lv_keyboard_create(Wifibackground);
    lv_obj_set_style_bg_color(keyboard, lv_color_hex(0x1b2835), LV_PART_MAIN);
    lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
    lv_keyboard_set_textarea(keyboard, password_textarea);
    lv_obj_set_size(keyboard, 980, 300);
    return password_textarea;
}


lv_obj_t *CreateBackButtonAndText(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data)
{

    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 5);
    lv_style_set_transform_height(&style_pr, 5);

    lv_obj_t *confirm_btn = lv_obj_create(parent);
    lv_obj_set_size(confirm_btn, 90, 31);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_RIGHT, x_ofs, y_ofs);
    lv_obj_add_flag(confirm_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(confirm_btn, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(confirm_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(confirm_btn, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(confirm_btn, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);

    lv_obj_add_style(confirm_btn, &style_pr, LV_STATE_PRESSED);

    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, confirm_text);
    lv_obj_set_style_text_color(confirm_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(confirm_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);

    return confirm_btn;
}


void CreateWifiKeyboard(lv_obj_t *parent)
{

    lv_obj_t *SrcParent = CreateMaskBox(lv_scr_act(), true);
    char WifiNameText[256];
    strncpy(wifiCredentials.ssid, GET_LABEL_TEXT(parent), SSID_MAX_LENGTH - 1);
    snprintf(WifiNameText, sizeof(WifiNameText), "输入“%s” 的密码", wifiCredentials.ssid);

    WifiNode *node = getWifiNodeFromListButton(parent);
    node->password_textarea = CreadWifiPasswordTextarea(SrcParent);

    lv_obj_t *Wifibackground = lv_obj_get_parent(node->password_textarea);
    CreateBackButtonAndText(Wifibackground, "确认", -230, 105, on_confirm_button_click, parent);
    CreateBackButtonAndText(Wifibackground, "取消", -110, 105, BackParentEvent, NULL);
    card_create_24_text(Wifibackground, "请输入WIFI密码: ", -390, -160);
    card_create_24_text(Wifibackground, WifiNameText, -20, -250);
}