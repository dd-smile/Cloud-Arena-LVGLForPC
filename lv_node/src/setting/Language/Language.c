#include "ui_app.h"
// #include "Language.h"


static lv_obj_t *lastClickedObj = NULL;

// 点击切换颜色和切换语言
static void OptionContentClick(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED)
  {
    if (lastClickedObj != NULL)
    {
      lv_obj_set_style_bg_color(lastClickedObj, lv_color_hex(0x141a23), 0); // 恢复上一个点击对象的背景颜色
    }
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x232d3b), 0);
    lastClickedObj = obj;
    switch ((int)obj->user_data)
    {
    case 0:
      set_cur_language_type(zhT);
      break;
    case 1:
      set_cur_language_type(enG);
    }
  }
}

// 创建选项内容
static lv_obj_t *CreateOptionContent(lv_obj_t *parent, const char *text, int16_t y, uint8_t i)
{
  lv_obj_t *Content = lv_obj_create(parent);
  lv_obj_set_size(Content, 460, 40);
  lv_obj_align(Content, LV_ALIGN_TOP_MID, 1, y);
  lv_obj_set_style_radius(Content, 0, LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(Content, lv_color_hex(0x141a23), 0);

  lv_obj_set_style_border_width(Content, 0, LV_PART_MAIN);
  lv_obj_set_style_text_font(Content, &fout_16_text, 0);
  lv_obj_set_style_text_color(Content, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_clear_flag(Content, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_user_data(Content, i);
  lv_obj_add_event_cb(Content, OptionContentClick, LV_EVENT_CLICKED, NULL); // 添加事件
  card_create_24_text(Content, text, -140, 0);

  return Content;
}

// 创建内框
static lv_obj_t *CreateLanguageBox(lv_obj_t *parent)
{
  lv_obj_t *LanguageBox = lv_obj_create(parent);
  lv_obj_set_size(LanguageBox, 465, 201);
  lv_obj_align(LanguageBox, LV_ALIGN_CENTER, 0, -10);
  lv_obj_set_style_bg_color(LanguageBox, lv_color_hex(0x141a23), LV_PART_MAIN);
  lv_obj_set_style_border_width(LanguageBox, 0, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(LanguageBox, 0, LV_PART_MAIN);
  lv_obj_clear_flag(LanguageBox, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动条

  const char *options[] = {"Chinese(CN)", "English(US)"}; // 选项文本列表
  int16_t y = -18;
  for (uint8_t i = 0; i < sizeof(options) / sizeof(options[0]); i++)
  {
    lv_obj_t *option = CreateOptionContent(LanguageBox, options[i], y, i);
    if (i == 0)
    {
      lv_obj_set_style_bg_color(option, lv_color_hex(0x232d3b), 0); // 设置第一个选项的背景颜色
      lastClickedObj = option;                                      // 初始化 lastClickedObj
    }
    y += 41;
  }

  return LanguageBox;
}

static void confirm_btn_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
      lv_obj_invalidate(lv_disp_get_scr_act(NULL));
      create_lv_layout(lv_disp_get_scr_act(NULL));
    }
}

// 创建弹出框
static lv_obj_t *CreatePopupBox(lv_obj_t *parent)
{
  lv_obj_t *mask = lv_c_create_mask_box(parent);         // 创建遮罩
  lv_obj_t *iamge = image_create(mask, &PopupBox, 0, 0); // 创建图标
  lv_obj_t *label = lv_label_create(iamge);              // 创建标题
  lv_label_set_text(label, "Select Language");
  lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, 173, 12);

  lv_obj_t *confirm_btn = btn_create_text(iamge, false, "OK", 135, 260);  // 创建按钮
  lv_obj_add_event_cb(confirm_btn, confirm_btn_handler, LV_EVENT_CLICKED, NULL); // 添加确认事件
  lv_obj_t *Back_btn = btn_create_text(iamge, false, "Cancel", 280, 260); // 创建按钮
  lv_obj_add_event_cb(Back_btn, lv_back_Mask, LV_EVENT_CLICKED, NULL);    // 添加返回事件

  CreateLanguageBox(iamge);

  return iamge;
}

/**点击时间图片事件**/
static void add_Language_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED)
  {
    CreatePopupBox(lv_scr_act());
  }
}

void CreateLanguage(lv_obj_t *parent)
{
  lv_obj_t *ImageObj = CreateOBJclick(parent);
  image_create(parent, &Language, -100, -30);
  card_create_24_text(parent, "Language\n  settings", 10, -15); // 创建标题
  card_create_16_text(parent, "Select language", -40, 35);      // 创建标题
  lv_obj_add_event_cb(ImageObj, add_Language_event_cb, LV_EVENT_ALL, NULL);
}