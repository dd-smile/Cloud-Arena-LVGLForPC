#include "../../lv_examples.h"

#if LV_USE_ARC && LV_BUILD_EXAMPLES

void lv_example_arc_1(void)
{
  /*Create an Arc*/
  static lv_style_t style;
  static lv_style_t style_part_indic; //创建样式
  lv_style_init(&style);              //初始化样式
  lv_style_init(&style_part_indic);
  lv_style_set_arc_color(&style, lv_color_hex(0xFECF3D)); //设置圆弧颜色
  lv_style_set_radius(&style_part_indic, 0);

  // 设置圆弧前景指示器颜色
  lv_style_set_arc_color(&style_part_indic, lv_color_hex(0x14E7F0));

  lv_obj_t *arc = lv_arc_create(lv_scr_act());
  lv_obj_set_size(arc, 120, 120);
  lv_arc_set_rotation(arc, 270);
  lv_arc_set_bg_angles(arc, 0, 360);
  lv_arc_set_value(arc, 60);
  lv_obj_center(arc);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);  /*Be sure the knob is not displayed*/
  lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE); /*To not allow adjusting by click*/
  lv_obj_add_style(arc, &style, 0);              //将样式添加到圆弧中
  lv_obj_add_style(arc, &style_part_indic, LV_PART_INDICATOR);
}

#endif
