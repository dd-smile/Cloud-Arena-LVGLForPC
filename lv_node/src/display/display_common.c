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
    lv_obj_set_style_text_font(destlabel, &PuHuiTi_Regular_16, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(destlabel, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_align_to(destlabel, alignlabel, align, x_ofs, y_ofs);
}

/**
 * 创建一个温湿度数据卡片
 * @param parent                     指向父对象的指针
*/
lv_obj_t *CreateTemHumCard(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 295, 235);
    lv_obj_set_style_bg_color(card, lv_color_make(31, 38, 51), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(card, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 0, LV_STATE_DEFAULT);
    return card;
}


/**
 * 创建一个用电趋势数据卡片
 * @param parent                     指向父对象的指针
*/
lv_obj_t *CreateElectricityCard(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 789, 235);
    lv_obj_set_style_bg_color(card, lv_color_make(31, 38, 51), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(card, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 0, LV_STATE_DEFAULT);
    return card;
}

/**
 * 创建一个数据看板标题
 * @param parent                       指向父对象的指针
 * @param text                         要设置的文本
 * @param align                        对齐方式
 * @param x_ofs                        x轴偏移量
 * @param y_ofs                        y轴偏移量
 * @return                             返回一个文本对象
*/
lv_obj_t *CreateDataBoardtitle(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *title = lv_label_create(parent);
    lv_label_set_text(title, text);

    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(title, 2, LV_STATE_DEFAULT);

    lv_obj_align_to(title, parent, align, x_ofs, y_ofs);

    return title;
}


/**
 * 创建一个数据看板文本
 * @param parent                       指向父对象的指针
 * @param text                         要设置的文本
 * @param align                        对齐方式
 * @param x_ofs                        x轴偏移量
 * @param y_ofs                        y轴偏移量
 * @return                             返回一个文本对象
*/
lv_obj_t *CreateDataBoardtxt(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *txt = lv_label_create(parent);
    lv_label_set_text(txt, text);

    lv_obj_set_style_text_color(txt, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(txt, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);

    lv_obj_align_to(txt, parent, align, x_ofs, y_ofs);

    return txt;
}