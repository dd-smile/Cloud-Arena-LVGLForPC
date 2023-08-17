#include "ui_app.h"

//左边导航页按钮结构体
struct
{
  const void *img_off;
  const void *img_on;
  int x;
  int y;
  const char *label;
} obj_buttons[] = {
    {&home_off, &home_on, 10, -5, "Facilities"},
    {&mode_off, &mode_on, 10, -2, "Mode"},
    {&setting_off, &setting_on, 10, -2, "Setting"},
};

All_Data home_data;

/*滑动页面时切换页面*/
static void home_tileview(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *active_tile = lv_tileview_get_tile_act(home_data.content);

  if (code == LV_EVENT_VALUE_CHANGED)
  {
    for (int i = 0; i < 3; i++)
    {
      lv_imgbtn_set_state(home_data.home_button[i], LV_IMGBTN_STATE_RELEASED);
      if (active_tile == home_data.home_pages[i])
      {
        lv_imgbtn_set_state(home_data.home_button[i], LV_IMGBTN_STATE_PRESSED);
      }
    }
  }
}

/*创建滑动页面*/
static void home_page_box(lv_obj_t *parent)
{
  home_data.content = lv_tileview_create(parent); // content card
  lv_obj_add_event_cb(home_data.content, home_tileview, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_set_style_bg_opa(home_data.content, 0, LV_STATE_DEFAULT);
  lv_obj_set_size(home_data.content, 904, 591);
  lv_obj_set_pos(home_data.content, 120, 9);
  lv_obj_set_style_width(home_data.content, 0, LV_PART_SCROLLBAR);

  for (int i = 0; i < 3; i++)
  {
    home_data.home_pages[i] = lv_tileview_add_tile(home_data.content, 0, i, LV_DIR_BOTTOM | LV_DIR_TOP);
  }
  home_data.home_page = home_data.home_pages[0];
  home_data.mode_page = home_data.home_pages[1];
  home_data.setting_page = home_data.home_pages[2];
}

/*导航点击事件*/
static void home_btn_event(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_user_data(e);

  if (code == LV_EVENT_CLICKED || code == LV_EVENT_VALUE_CHANGED)
  {
    for (int i = 0; i < 3; i++)
    {
      lv_imgbtn_set_state(home_data.home_button[i], obj == home_data.home_button[i] ? LV_IMGBTN_STATE_PRESSED : LV_IMGBTN_STATE_RELEASED);
      if (obj == home_data.home_button[i])
      {
        lv_obj_set_tile_id(home_data.content, 0, i, LV_ANIM_ON);
      }
    }
  }
}

/*导航栏按键范围*/
static lv_obj_t *lv_fout_create(lv_obj_t *obj, lv_coord_t y_ofs)
{
  lv_obj_t *home_fout = lv_obj_create(obj);
  lv_obj_set_size(home_fout, 100, 120);
  lv_obj_align(home_fout, LV_ALIGN_TOP_LEFT, 0, y_ofs);
  lv_obj_set_style_bg_opa(home_fout, 0, LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(home_fout, 0, LV_STATE_DEFAULT);
  return home_fout;
}

/**
 * 创建主导航栏按钮
 * @param obj     指向图像对象的指针
 * 
 * **/
static lv_obj_t *lv_imgbtn_create_with_label(lv_obj_t *obj, const void *img_src, const void *img_crs, lv_coord_t x, lv_coord_t x_ofs, lv_coord_t y_ofs, const char *label_text)
{

  lv_obj_t *home_fout = lv_fout_create(obj, y_ofs);
  lv_obj_t *img_btn = lv_imgbtn_create(home_fout);
  lv_imgbtn_set_src(img_btn, LV_IMGBTN_STATE_RELEASED, NULL, img_src, NULL);
  lv_imgbtn_set_src(img_btn, LV_IMGBTN_STATE_PRESSED, NULL, img_crs, NULL);
  lv_obj_set_size(img_btn, 40, 50);
  lv_obj_set_x(img_btn, x);
  lv_obj_t *label = lv_label_create(obj);
  lv_label_set_text(label, label_text);
  lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_align_to(label, img_btn, LV_ALIGN_CENTER, x_ofs, 30);
  lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, 0);
  lv_obj_add_event_cb(home_fout, home_btn_event, LV_EVENT_ALL, img_btn);
  lv_obj_add_event_cb(img_btn, home_btn_event, LV_EVENT_ALL, img_btn);
  return img_btn;
}

/*导航页面*/
static void ui_navigation_main_menu(lv_obj_t *obj)
{
  for (int i = 0; i < 3; i++)
  {
    home_data.home_button[i] = lv_imgbtn_create_with_label(obj, obj_buttons[i].img_off, obj_buttons[i].img_on, obj_buttons[i].x, obj_buttons[i].y, 70 + i * 160, obj_buttons[i].label);
  }
  lv_imgbtn_set_state(home_data.home_button[0], LV_IMGBTN_STATE_PRESSED);
}

/*主页背景*/
static void home_bg(lv_obj_t *obj)
{
  lv_obj_t *lv_background = lv_img_create(obj); // 创建一个背景页对象
  lv_img_set_src(lv_background, &background);   //设置背景页对象的背景数据
  lv_obj_t *lv_detailed = lv_img_create(obj); // 创建一个左边导航页对象
  lv_img_set_src(lv_detailed, &detailed);     //设置左边导航页对象的图像数据
  ui_navigation_main_menu(lv_detailed);
}

void create_lv_layout(lv_obj_t *scr)
{

  home_bg(scr);              // 设置主页背景
  home_page_box(scr);        // 框架
  create_wifi_and_time(scr); // wifi 和 时间

  CreateHomePage(home_data.home_page);       // 主页
  CreateModePage(home_data.mode_page);       // 模式页面
  CreateSettingPage(home_data.setting_page); // 设置页面


}
