/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:58:24
 * @LastEditTime: 2023-07-04 14:13:43
 * @FilePath: /new_home_smart/lv_node/src/common/time_wifi.c
 */
#include "ui_app.h"

static char *read_time()
{
  char *time_date_buff = malloc(100 * sizeof(char));
  if (time_date_buff == NULL)
  {
    return NULL;
  }

  time_t t = time(NULL);
  struct tm *tmp = localtime(&t);
  snprintf(time_date_buff, 100, "%02d:%02d", tmp->tm_hour, tmp->tm_min);

  return time_date_buff;
}

/**更新时间标签的回调函数**/
static void lv_set_time(lv_timer_t *timer)
{
  char *time_str = read_time();
  lv_obj_t *obj = timer->user_data;
  lv_label_set_text(obj, time_str);
  free(time_str); // Free the dynamically allocated memory
}

/*wifi图标显示*/
 lv_obj_t *create_wifi(lv_obj_t *parent)
{
  lv_obj_t *wifi = lv_obj_create(parent);
  lv_obj_set_size(wifi, 200, 75);
  
  lv_obj_align(wifi, LV_ALIGN_TOP_RIGHT, -50, -5);
  lv_obj_set_style_bg_opa(wifi, 0, LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(wifi, 0, LV_STATE_DEFAULT);

  lv_obj_t *wifi_img = lv_img_create(wifi);
  lv_img_set_src(wifi_img, &Logo);
  lv_obj_align(wifi_img, LV_ALIGN_CENTER, -30, 0);

  return wifi;
}
/*读取时间*/
static lv_obj_t *lv_create_recv_time(lv_obj_t *parent)
{
  char *time_str = read_time();

  lv_obj_t *recv_time = lv_label_create(parent); // 读取时间
  lv_label_set_text(recv_time, time_str);
  lv_obj_align(recv_time, LV_ALIGN_TOP_RIGHT, -40, 25);
  lv_obj_set_style_text_color(recv_time, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(recv_time, &lv_font_montserrat_20, LV_STATE_DEFAULT);

  free(time_str); // Free the dynamically allocated memory

  return recv_time;
}

void create_wifi_and_time(lv_obj_t *parent)
{
  lv_obj_t* wifi = create_wifi(parent);
  lv_obj_t *recv_time = lv_create_recv_time(parent);
  lv_timer_create(lv_set_time, 1000, recv_time);
}
