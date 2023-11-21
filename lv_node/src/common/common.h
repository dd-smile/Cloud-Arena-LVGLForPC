/***
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-03 17:38:42
 * @LastEditTime: 2023-07-03 17:53:51
 * @FilePath: /new_home_smart/lv_node/src/common/common.h
 */
#ifndef __H_COMMON_H__
#define __H_COMMON_H__

void lv_back_event(lv_event_t *e);
lv_obj_t *image_create(lv_obj_t *parent, const void *src, lv_coord_t x, lv_coord_t y);
lv_obj_t *lv_c_create_mask_box(lv_obj_t *parent);
lv_obj_t *lv_mode_create_mask_box(lv_obj_t *parent);
lv_obj_t *card_create_100_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_72_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_36_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_24_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_20_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_16_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_12_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_14_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *create_16_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);


lv_obj_t *create_12_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *create_12_Green_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *create_12_Red_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *create_12_Bule_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);

void lv_back(lv_event_t *e);
void lv_back_Mask(lv_event_t *e);
lv_obj_t *lv_keypage_create(lv_obj_t *parent, lv_event_cb_t event_cb);
static lv_obj_t *lv_100ask_pinyin_ime_simple_test(lv_obj_t *parent);
lv_obj_t *btn_create_text(lv_obj_t *parent, bool on_back, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs);
lv_obj_t *create_popup(lv_obj_t *parent,const char *title, const char *confirm_text, const char *back_text, const char *text, lv_event_cb_t event_cb, void *user_data);

lv_obj_t *Createcard(lv_obj_t *parent);
lv_obj_t *CreateOBJclick(lv_obj_t *obj);
lv_obj_t *lv_100ask_pinyin_ime_simple_test(lv_obj_t *parent);
lv_obj_t *create_popup_page(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *create_recv_time(lv_obj_t *parent, const lv_font_t *value, lv_coord_t x, lv_coord_t y);
lv_obj_t *CreateSlider(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, int32_t min, int32_t max, lv_color_t value, int32_t num_value, lv_coord_t w, lv_coord_t h, bool status);

extern lv_obj_t *Textarea_Pinyin;



#endif //__COMMON_H__