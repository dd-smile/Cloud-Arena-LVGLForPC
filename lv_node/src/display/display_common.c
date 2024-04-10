/*
 * @Description: 数据看板通用代码
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-04-09 11:28:17
 * @LastEditTime: 2023-04-10 15:59:05
 * @FilePath: lv_node/src/display/display_common.c
 */
#include "ui_app.h"

/**
 * 设置设备数据卡片的图片样式
 * @param parent                    指向父对象的指针
 * @param src                       要设置的图片
 * @param align                     对齐方式
 * @param x_ofs                     x轴偏移量
 * @param y_ofs                     y轴偏移量
 * @return                          返回一个图片对象
*/
lv_obj_t *equipment_data_style(lv_obj_t *parent, const void *src, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *temp_image = lv_img_create(parent);
    lv_img_set_src(temp_image, src);
    lv_obj_align_to(temp_image, parent, align, x_ofs, y_ofs);
    return temp_image;
}

/**
 * 设置温湿度数据文本样式
 * @param destlabel                  指向要设置的文本对象的指针
 * @param alignlabel                 指向要对齐的文本对象的指针
 * @param align                      对齐方式
 * @param x_ofs                      x轴偏移量
 * @param y_ofs                      y轴偏移量
*/
void tempehum_label_style(lv_obj_t *destlabel, lv_obj_t *alignlabel, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_set_style_text_font(destlabel, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(destlabel, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_align_to(destlabel, alignlabel, align, x_ofs, y_ofs);
}