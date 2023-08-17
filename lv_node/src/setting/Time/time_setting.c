/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-04-25 16:02:39
 * @LastEditTime: 2023-04-25 17:26:07
 * @FilePath: /SmartBox_Power/lv_node/src/setting/time_setting/time_setting.c
 */
#include "ui_app.h"
char year_buf[32];
char month_buf[32];
char day_buf[32];
char hour_buf[32];
char min_buf[32];

static void mask_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if (code == LV_EVENT_COVER_CHECK)
    {
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);
    }
    else if (code == LV_EVENT_DRAW_MAIN_BEGIN)
    {
        /* add mask */
        const lv_font_t *font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_coord_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
        lv_coord_t font_h = lv_font_get_line_height(font);

        lv_area_t roller_coords;
        lv_obj_get_coords(obj, &roller_coords);

        lv_area_t rect_area;
        rect_area.x1 = roller_coords.x1;
        rect_area.x2 = roller_coords.x2;
        rect_area.y1 = roller_coords.y1;
        rect_area.y2 = roller_coords.y1 + (lv_obj_get_height(obj) - font_h - line_space) / 2;

        lv_draw_mask_fade_param_t *fade_mask_top = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_top, &rect_area, LV_OPA_TRANSP, rect_area.y1, LV_OPA_COVER, rect_area.y2);
        mask_top_id = lv_draw_mask_add(fade_mask_top, NULL);

        rect_area.y1 = rect_area.y2 + font_h + line_space - 1;
        rect_area.y2 = roller_coords.y2;

        lv_draw_mask_fade_param_t *fade_mask_bottom = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_bottom, &rect_area, LV_OPA_COVER, rect_area.y1, LV_OPA_TRANSP, rect_area.y2);
        mask_bottom_id = lv_draw_mask_add(fade_mask_bottom, NULL);
    }
    else if (code == LV_EVENT_DRAW_POST_END)
    {
        lv_draw_mask_fade_param_t *fade_mask_top = lv_draw_mask_remove_id(mask_top_id);
        lv_draw_mask_fade_param_t *fade_mask_bottom = lv_draw_mask_remove_id(mask_bottom_id);
        lv_draw_mask_free_param(fade_mask_top);
        lv_draw_mask_free_param(fade_mask_bottom);
        lv_mem_buf_release(fade_mask_top);
        lv_mem_buf_release(fade_mask_bottom);
        mask_top_id = -1;
        mask_bottom_id = -1;
    }
}

static void add_SaveButton_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {

        printf("年：%s\n", year_buf);
        printf("月：%s\n", month_buf);
        printf("日：%s\n", day_buf);
        printf("时：%s\n", hour_buf);
        printf("分：%s\n", min_buf);
    }
}

static void rolleryear_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, year_buf, sizeof(year_buf));
        printf("Selected year: %s\n", year_buf);
    }
}
static void rollermonth_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, month_buf, sizeof(month_buf));
        printf("Selected month: %s\n", month_buf);
    }
}
static void rollerday_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, day_buf, sizeof(day_buf));
        printf("Selected day: %s\n", day_buf);
    }
}
static void rollerhour_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, hour_buf, sizeof(hour_buf));
        printf("Selected hour: %s\n", hour_buf);
    }
}
static void rollermin_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, min_buf, sizeof(min_buf));
        printf("Selected min: %s\n", min_buf);
    }
}

static lv_obj_t *createTimeCard(lv_obj_t *parent, lv_coord_t x, const char *options, uint16_t sel_opt)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_make(12, 27, 51));
    // lv_style_set_opa(&style, 0);
    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);
    lv_obj_add_style(parent, &style, 0);

    lv_obj_t *roller1 = lv_roller_create(parent);
    lv_obj_add_style(roller1, &style, 0);
    lv_obj_set_style_bg_opa(roller1, 200, LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(roller1, 20, LV_PART_MAIN);
    lv_obj_set_style_border_opa(roller1, 0, LV_PART_MAIN);
    lv_obj_set_width(roller1, 130);
    lv_obj_set_style_text_line_space(roller1, 60, LV_PART_MAIN);
    lv_obj_set_style_radius(roller1, 0, LV_PART_MAIN);
    lv_obj_set_pos(roller1, x, 50);

    lv_obj_set_style_text_font(roller1, &PuHuiTi_Regular_20, LV_PART_SELECTED);
    lv_obj_set_style_text_font(roller1, &PuHuiTi_Regular_20, LV_PART_MAIN);
    lv_obj_set_style_bg_color(roller1, lv_color_make(33, 51, 75), LV_PART_SELECTED);
    lv_roller_set_options(roller1, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller1, 3);
    lv_obj_add_event_cb(roller1, mask_event_cb, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller1, sel_opt, LV_ANIM_OFF);

    return roller1;
}

void time_roller(lv_obj_t *parent)
{
    time_t t = time(NULL);
    struct tm *timer = localtime(&t);

    uint16_t t_year = timer->tm_year + 1900; // 年份需要加上 1900
    uint16_t month = timer->tm_mon;          // 月份从 0 开始，所以需要加上 1
    uint16_t day = timer->tm_mday;
    uint16_t hour = timer->tm_hour;
    uint16_t min = timer->tm_min;

    printf("%d:%02d\n", hour, min);
    createTimeCard(parent, 30, TIME_ROLLER_YEAH, 2023);
    printf("%d\n", t_year);

    lv_obj_t *roller1 = createTimeCard(parent, 30, TIME_ROLLER_YEAH, t_year - 1970);

    lv_roller_get_selected_str(roller1, year_buf, sizeof(year_buf));
    printf("Selected year: %s\n", year_buf);
    lv_obj_add_event_cb(roller1, rolleryear_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *roller2 = createTimeCard(parent, 130, TIME_ROLLER_MONTH, month);
    lv_roller_get_selected_str(roller2, month_buf, sizeof(month_buf));

    lv_obj_add_event_cb(roller2, rollermonth_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *roller3 = createTimeCard(parent, 230, TIME_ROLLER_DAY, day - 1);
    lv_roller_get_selected_str(roller3, day_buf, sizeof(day_buf));
    lv_obj_add_event_cb(roller3, rollerday_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *roller4 = createTimeCard(parent, 330, TIME_ROLLER_HOUR, hour - 1);
    lv_roller_get_selected_str(roller4, hour_buf, sizeof(hour_buf));
    
    lv_obj_add_event_cb(roller4, rollerhour_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *roller5 = createTimeCard(parent, 430, TIME_ROLLER_MIN, min);
    lv_roller_get_selected_str(roller5, min_buf, sizeof(min_buf));
    lv_obj_add_event_cb(roller5, rollermin_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}

/**点击时间图片事件**/
static void add_time_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_t *SetMask = lv_c_create_mask_box(lv_scr_act());
        lv_obj_t *iamge = image_create(SetMask, &popover, 0, 0); // 创建图标
        card_create_24_text(iamge, "Time Settings", 10, -150);

        lv_obj_t *SaveButton = btn_create_text(iamge, false, "Save", 255, 300);
        lv_obj_add_event_cb(SaveButton, add_SaveButton_event_cb, LV_EVENT_ALL, NULL);
        time_roller(iamge);
    }
}

void CreateTime(lv_obj_t *parent)
{
    lv_obj_t *ImageObj = CreateOBJclick(parent);
    image_create(parent, &Timming, -100, -30);
    card_create_24_text(parent, "Time settings", 10, -30); // 创建标题
    lv_obj_add_event_cb(ImageObj, add_time_event_cb, LV_EVENT_ALL, NULL);
    create_recv_time(parent, &fout_16_text, 6, 34);
}