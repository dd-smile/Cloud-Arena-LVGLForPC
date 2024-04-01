/*
 * @Description:主页面时间与logo标志
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-03-09 11:58:24
 * @LastEditTime: 2023-08-31 14:13:43
 * @FilePath: lv_node/src/common/time_wifi.c
 */
#include "ui_app.h"

lv_obj_t *g_mqtt_detection;

/**
 * 读取时间
 * 
 * */
static char *read_time()
{
  char *time_date_buff = malloc(100 * sizeof(char));    //开辟内存空间
  if (time_date_buff == NULL)
  {
    return NULL;
  }

  time_t t = time(NULL);
  struct tm *tmp = localtime(&t);
  snprintf(time_date_buff, 100, "%02d:%02d", tmp->tm_hour, tmp->tm_min);

  return time_date_buff;
}

/**
 * 更新时间标签的回调函数
 * @param timer       指向lv_timer的指针
 * */
static void lv_set_time(lv_timer_t *timer)
{
  char *time_str = read_time();
  lv_obj_t *obj = timer->user_data;
  lv_label_set_text(obj, time_str);
  free(time_str); // 释放动态分配的内存
}

/**
 * wifi图标显示
 * @param parent         指向一个对象的指针，它将是新标签的父对象
 * */
lv_obj_t *create_wifi(lv_obj_t *parent)
{
  lv_obj_t *wifi = lv_obj_create(parent);
  lv_obj_set_size(wifi, 200, 75);
  
  lv_obj_align(wifi, LV_ALIGN_TOP_RIGHT, -50, -5);
  lv_obj_set_style_bg_opa(wifi, 0, LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(wifi, 0, LV_STATE_DEFAULT);     //设置样式边框透明度

  lv_obj_t *wifi_img = lv_img_create(wifi);
  lv_img_set_src(wifi_img, &Logo);
  lv_obj_align(wifi_img, LV_ALIGN_CENTER, -30, 0);


  g_mqtt_detection = card_create_16_Red_text(parent, "", 0, 0);
  lv_obj_align_to(g_mqtt_detection, wifi, LV_ALIGN_LEFT_MID, -120, 0);

  return wifi;
}

/**
 * 读取时间
 * @param parent         指向一个对象的指针，它将是新标签的父对象
 * @return               返回时间标签对象
 * */
static lv_obj_t *lv_create_recv_time(lv_obj_t *parent)
{
  char *time_str = read_time();   //读取时间

  lv_obj_t *recv_time = lv_label_create(parent);
  lv_label_set_text(recv_time, time_str);
  lv_obj_align(recv_time, LV_ALIGN_TOP_RIGHT, -40, 25);
  lv_obj_set_style_text_color(recv_time, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(recv_time, &lv_font_montserrat_20, LV_STATE_DEFAULT);

  free(time_str); // 释放动态分配的内存

  return recv_time;
}

/**
 * 创建wifi和time的标签对象
 * @param parent         指向一个对象的指针，它将是新标签的父对象
 * */
void create_wifi_and_time(lv_obj_t *parent)
{
  lv_obj_t* wifi = create_wifi(parent);
  lv_obj_t* recv_time = lv_create_recv_time(parent);
  lv_timer_create(lv_set_time, 1000, recv_time);   //以一秒为单位调用周期
}
