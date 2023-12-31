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
uint16_t hour;   //当前时间
uint16_t last_hour;  //上一次时间
weather_t weather = {0};
lv_obj_t * chart;
lv_chart_series_t * ser1; //温度数据系列
lv_chart_series_t * ser2; //湿度数据系列
static char buf[1024];   //用来发送get请求
static char buffer[1024];   //用来接收心知天气的数据
static int fd;   //用于与心知天气通信
int iDataNum;
static lv_coord_t temp_average_buf[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static lv_coord_t hum_average_buf[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//解析json
void aita_ParseJsonNow(char *msg, weather_t *w) 
{
    cJSON *json, *ja, *jo, *josub, *item;
    json = cJSON_Parse(msg); //parse string to cJSON type
    if (json == NULL) 
    {
        printf("json type cast error: %s", cJSON_GetErrorPtr());
        return;
    } 
    else 
    {
        printf("parse now pack\n");
        if ((ja=cJSON_GetObjectItem(json, "results")) != NULL) { //get results array
            if ((jo=cJSON_GetArrayItem(ja, 0)) != NULL) {        //get array[0](the only item)
                //get location object
                if ((josub=cJSON_GetObjectItem(jo, "location")) != NULL) 
                {
                    if ((item=cJSON_GetObjectItem(josub, "id")) != NULL) 
                    {
                        memcpy(w->id, item->valuestring, strlen(item->valuestring));
                    }
                    if ((item=cJSON_GetObjectItem(josub, "name")) != NULL) 
                    {
                        memcpy(w->name, item->valuestring, strlen(item->valuestring));
                    }
                    if ((item=cJSON_GetObjectItem(josub, "country")) != NULL) 
                    {
                        memcpy(w->country, item->valuestring, strlen(item->valuestring));
                    }
                    if ((item=cJSON_GetObjectItem(josub, "path")) != NULL) 
                    {
                        memcpy(w->path, item->valuestring, strlen(item->valuestring));
                    }
                    if ((item=cJSON_GetObjectItem(josub, "timezone")) != NULL) 
                    {
                        memcpy(w->timezone, item->valuestring, strlen(item->valuestring));
                    }
                    if ((item=cJSON_GetObjectItem(josub, "timezone_offset")) != NULL) 
                    {
                        memcpy(w->tz_offset, item->valuestring, strlen(item->valuestring));
                    }
                }
                //get now object
                if ((josub=cJSON_GetObjectItem(jo, "now")) != NULL) 
                {
                    if ((item=cJSON_GetObjectItem(josub, "text")) != NULL) 
                    {
                        memcpy(w->text, item->valuestring, strlen(item->valuestring));
                    }
                    if ((item=cJSON_GetObjectItem(josub, "code")) != NULL) 
                    {
                        memcpy(w->code, item->valuestring, strlen(item->valuestring));
                    }
                    if ((item=cJSON_GetObjectItem(josub, "temperature")) != NULL) 
                    {
                        memcpy(w->temp, item->valuestring, strlen(item->valuestring));
                    }
                }
                //get last_update object
                if ((josub=cJSON_GetObjectItem(jo, "last_update")) != NULL) 
                {
                    memcpy(w->last_update, josub->valuestring, strlen(josub->valuestring));                 
                }
            }
        }
    }
    //delete original json pack free memory
    cJSON_Delete(json);
    return;
}

void aita_PrintWeather(weather_t *w) 
{
    printf("id: %s\n", w->id);
    printf("name: %s\n", w->name);
    printf("country: %s\n", w->country);
    printf("path: %s\n", w->path);
    printf("timezone: %s\n", w->timezone);
    printf("timezone_offset: %s\n", w->tz_offset);
    printf("text: %s\n", w->text);
    printf("code: %s\n", w->code);
    printf("temperature: %s\n", w->temp);
    printf("last_update: %s\n", w->last_update);
}

/**
 * 创建温湿度历史图表的点击事件
 * 在按下点的位置绘制一个矩形，矩形的格式化为一个字符串，显示点的值。
 * @param e               指向事件描述符的指针
 * */
static void event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * chart = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        lv_obj_invalidate(chart);  //如果发生滑动，更新图表
    }
    if (code == LV_EVENT_REFR_EXT_DRAW_SIZE) 
    {
        lv_coord_t * s = lv_event_get_param(e);
        *s = LV_MAX(*s, 20);
    }
    else if (code == LV_EVENT_DRAW_POST_END) 
    {
        int32_t id = lv_chart_get_pressed_point(chart);
        if (id == LV_CHART_POINT_NONE) return;

        lv_chart_series_t * ser = lv_chart_get_series_next(chart, NULL);
        while (ser) 
        {
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
    else if (code == LV_EVENT_RELEASED) 
    {
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
    if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL))
        return;

    if (dsc->text)
    {
        if (dsc->id == LV_CHART_AXIS_PRIMARY_X )
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

//获取当前时间
void get_localtime()
{
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    hour = tm_now->tm_hour;
}

/**
 * 计算温湿度数据平均数
 * @param temp     读取到的实时温度
*/
static void display_average_data(char *temp, char *hum)
{
    get_localtime();

    static double temp_average;
    static double hum_average;
    static double temp_f;
    static double hum_f;

    if (temp != NULL && hum != NULL)
    {
        temp_f = atof(temp);
        hum_f = atof(hum);
    }
    else
    {
        temp_f = 0.0;
        hum_f = 0.0;
    }
    if (last_hour != hour)
    {
        last_hour = hour;
        temp_average_buf[hour] = temp_average;
        hum_average_buf[hour] = hum_average;
        printf("小时温度: %d, 小时湿度: %d\n", temp_average_buf[hour], hum_average_buf[hour]);
        lv_chart_set_ext_y_array(chart, ser1, temp_average_buf);
        lv_chart_set_ext_y_array(chart, ser2, hum_average_buf);
        lv_chart_refresh(chart);
    }
    else
    {
        temp_average = (temp_f + (temp_average - temp_f) / 2.0);
        hum_average = (hum_f + (hum_average - hum_f) / 2.0);
        printf("平均温度: %.2f, 平均湿度: %.2f\n", temp_average, hum_average);
    }
}

void timer_average_callback(lv_timer_t * timer) 
{
    display_average_data(temp_data, hum_data);
}

/**
 * 创建心知天气定时器回调函数
*/
void timer_weather_callback(lv_timer_t * timer) 
{   
    get_localtime();

    char buffer_weather[1024];

    if (last_hour != hour)
    {
        last_hour = hour;
        // 1. 创建通信的套接字
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1)
        {
            perror("socket");
            exit(0);
        }

        // 2. 连接服务器
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);   // 大端端口
        inet_pton(AF_INET, "116.62.81.138", &addr.sin_addr.s_addr);

        int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
        if (ret == -1)
        {
            perror("connect");
            exit(0);
        }

        // 3. 和服务器端通信
        // 发送数据
        sprintf(buf, "GET https://api.seniverse.com/v3/weather/now.json?key=SCYXqGB7ZPUSvlsZy&location=shenzhen&language=zh-Hans&unit=c\r\n\r\n");  
        write(fd, buf, strlen(buf)+1);

        buffer[0] = '\0';

		iDataNum = recv(fd, buffer, 1024, 0);

        buffer[iDataNum] = '\0';

        printf("收到消息: %s\n", buffer);

        aita_ParseJsonNow(buffer, &weather);

        snprintf(buffer_weather, sizeof(buffer_weather), "今天天气:%s,外温:%s摄氏度", weather.text, weather.temp);
        lv_label_set_text(sensor_data.label_weather, buffer_weather);  //调用心知天气ａｐｉ

        aita_PrintWeather(&weather);

        close(fd);
    }   
}

/**
 * 创建温湿度定时器回调函数
*/
void timer_data_callback(lv_timer_t * timer) 
{
    lv_obj_t *label = (lv_obj_t *)timer->user_data;

    char buffer_temp[20];
    char buffer_hum[20];
    snprintf(buffer_temp, sizeof(buffer_temp), "室内温度: %s", temp_data);
    snprintf(buffer_hum, sizeof(buffer_hum), "室内湿度: %s", hum_data);

    lv_label_set_text(sensor_data.label_temp, buffer_temp);
    lv_label_set_text(sensor_data.label_hum, buffer_hum);
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
    lv_label_set_text(sensor_data.label_weather, "今天天气:晴朗,外温:30摄氏度");  //调用心知天气ａｐｉ
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
    ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);  //红色数据线
    ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);  //绿色数据线
    uint32_t i;
    for (i = 0; i < 24; i++) 
    {
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