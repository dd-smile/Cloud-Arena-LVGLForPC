/*
 * @Description: 数据看板展示页面
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-21 11:28:17
 * @LastEditTime: 2023-09-22 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/display/display.h
 */
#include "ui_app.h"

char temp_data[1024];  //用于输出获取到的温度数据
char hum_data[1024];   //用于输出获取到的湿度数据
lv_timer_t *timer = NULL;
Sensor_Data sensor_data;

/**
 * 创建温湿度历史图表的点击事件
 * 在按下点的位置绘制一个矩形，矩形的格式化为一个字符串，显示点的值。
 * @param e               指向事件描述符的指针
 * */
static void event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * chart = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_invalidate(chart);  //如果发生滑动，更新图表
    }
    if(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        lv_coord_t * s = lv_event_get_param(e);
        *s = LV_MAX(*s, 20);
    }
    else if(code == LV_EVENT_DRAW_POST_END) {
        int32_t id = lv_chart_get_pressed_point(chart);
        if(id == LV_CHART_POINT_NONE) return;

        lv_chart_series_t * ser = lv_chart_get_series_next(chart, NULL);
        while(ser) {
            lv_point_t p;
            lv_chart_get_point_pos_by_id(chart, ser, id, &p);  //获取点的位置

            lv_coord_t * y_array = lv_chart_get_y_array(chart, ser);  //获取ｙ坐标的值，用于显示
            lv_coord_t value = y_array[id];

            char buf[16];
            lv_snprintf(buf, sizeof(buf), LV_SYMBOL_DUMMY"%d", value);

            //绘制一个矩形
            lv_draw_rect_dsc_t draw_rect_dsc;
            lv_draw_rect_dsc_init(&draw_rect_dsc);
            draw_rect_dsc.bg_color = lv_color_black();
            draw_rect_dsc.bg_opa = LV_OPA_50;
            draw_rect_dsc.radius = 3;
            draw_rect_dsc.bg_img_src = buf;  //格式化成字符串，显示点的ｙ坐标值
            draw_rect_dsc.bg_img_recolor = lv_color_white();

            //设置矩形的区域
            lv_area_t a;
            a.x1 = chart->coords.x1 + p.x - 20;
            a.x2 = chart->coords.x1 + p.x + 20;
            a.y1 = chart->coords.y1 + p.y - 30;
            a.y2 = chart->coords.y1 + p.y - 10;

            lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);
            lv_draw_rect(draw_ctx, &draw_rect_dsc, &a);

            ser = lv_chart_get_series_next(chart, ser);
        }
    }
    else if(code == LV_EVENT_RELEASED) {
        lv_obj_invalidate(chart);
    }
}

/**
 * 创建温湿度历史图表的标签更改事件
 * @param e               指向事件描述符的指针
 * */
static void chart_draww_event_cb(lv_event_t * e)
{
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    if(!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL))
        return;
    if(dsc->text)
    {
        if(dsc->id == LV_CHART_AXIS_PRIMARY_X )
        {
            const char *time[] = {"00:00", "01:00", "02:00", "03:00", "04:00", "05:00", "06:00", "07:00", "08:00", "09:00", "10:00", "11:00", "12:00"
            , "13:00", "14:00", "15:00", "16:00", "17:00", "18:00", "19:00", "20:00", "21:00", "22:00", "23:00"};
            lv_snprintf(dsc->text, dsc->text_length, "%s", time[dsc->value]);
        }
    }
}

/**
 * 格式化浮点数为字符串，保留两位小数
 * @param label          要设置的标签控件
 * @param number         要设置的数字
 * @param type           显示的类型
 * */
void display_float_number(lv_obj_t *label, char *number, int type)
{
    char buffer[20];
    switch (type)
    {
    case 0:  //温度
        snprintf(buffer, sizeof(buffer), "室内温度: %s", number);
        lv_label_set_text(label, buffer);
        break;
    case 1:  //湿度
        snprintf(buffer, sizeof(buffer), "室内湿度: %s", number);
        lv_label_set_text(label, buffer);
        break;
    }
}

/**
 * 获取温度数据
*/
static char *get_temp()
{
    return temp_data;
}

/**
 * 获取湿度数据
*/
static char *get_hum()
{
    return hum_data;
}

/**
 * 创建定时器回调函数
*/
void timer_data_callback(lv_timer_t * timer) 
{
    // 从传感器获取数据
    char *sensor_temp = get_temp();
    char *sensor_hum = get_hum();

    lv_obj_t *label = (lv_obj_t *)timer->user_data;

    char buffer_temp[20];
    char buffer_hum[20];
    snprintf(buffer_temp, sizeof(buffer_temp), "室内温度: %s", sensor_temp);
    snprintf(buffer_hum, sizeof(buffer_hum), "室内湿度: %s", sensor_hum);


    lv_label_set_text(sensor_data.label_temp, buffer_temp);
    lv_label_set_text(sensor_data.label_hum, buffer_hum);
    // lv_label_set_text(sensor_data.label_weather, "今天天气晴朗,外温30摄氏度");  //调用心知天气ａｐｉ

}

/**
 * 创建温湿度的实时数据
 * @param parent          指向父对象的指针   
 * */
static void Create_TempeHumData(lv_obj_t *parent)
{
    card_create_20_text(parent, "实时温湿度数据:", -345,-200);

    lv_obj_t *card = Createcard(parent);
    lv_obj_set_pos(card, 5, 65);

    timer = lv_timer_create(timer_data_callback, 60000, card);

    
    sensor_data.label_temp = lv_label_create(card);
    display_float_number(sensor_data.label_temp, "26", 0);  
    lv_obj_set_style_text_font(sensor_data.label_temp, &PuHuiTi_Regular_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(sensor_data.label_temp, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_align(sensor_data.label_temp, LV_ALIGN_CENTER, -60, -45);

    sensor_data.label_hum = lv_label_create(card);
    display_float_number(sensor_data.label_hum, "45", 1);  
    lv_obj_set_style_text_font(sensor_data.label_hum, &PuHuiTi_Regular_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(sensor_data.label_hum, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_align(sensor_data.label_hum, LV_ALIGN_CENTER, -60, -10);

    sensor_data.label_weather = lv_label_create(card);
    lv_label_set_text(sensor_data.label_weather, "今天天气晴朗,外温30摄氏度");  //调用心知天气ａｐｉ
    lv_obj_set_style_text_font(sensor_data.label_weather, &PuHuiTi_Regular_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(sensor_data.label_weather, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_align_to(sensor_data.label_weather, sensor_data.label_hum, LV_ALIGN_BOTTOM_MID, 50, 40);
}

/**
 * 创建温湿度历史数据图表
 * @param parent          指向父对象的指针   
 * */
static void Create_TempeHumChart(lv_obj_t *parent)
{

    card_create_20_text(parent, "历史温湿度数据:", 10,-200);

    /*创建图表*/
    lv_obj_t * chart;
    chart = lv_chart_create(parent);
    lv_obj_set_style_bg_color(chart, lv_color_make(31, 38, 51), LV_STATE_DEFAULT);
    lv_obj_set_size(chart, 420, 179);
    lv_obj_set_pos(chart, 350, 65);

    lv_obj_set_style_line_color(chart, lv_color_make(31, 38, 51), LV_PART_MAIN); //设置网格线颜色（隐藏它）
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    lv_obj_add_event_cb(chart, event_cb, LV_EVENT_ALL, NULL);
    lv_obj_refresh_ext_draw_size(chart);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 24, 1, true, 40);
    lv_obj_add_event_cb(chart, chart_draww_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

    /*把X放大一点*/
    lv_chart_set_zoom_x(chart, 800);

    lv_chart_set_point_count(chart, 24);  //设置２４个点

    /*添加两个数据系列*/
    lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);  //红色数据线
    lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);  //绿色数据线
    uint32_t i;
    for(i = 0; i < 24; i++) {
        lv_chart_set_next_value(chart, ser1, lv_rand(60,90));
        lv_chart_set_next_value(chart, ser2, lv_rand(10,40));
    }
}

/**
 * 创建数据看板页面
 * @param parent            指向父对象的指针     
 * */
void CreateDisplayPage(lv_obj_t *parent)
{
    Create_TempeHumData(parent);
    Create_TempeHumChart(parent);
}