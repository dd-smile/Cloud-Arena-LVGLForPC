/*
 * @Description:  主页面ＵＩ
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-04-25 16:02:39
 * @LastEditTime: 2024-01-31 17:26:07
 * @FilePath: lv_node/src/ui_main.c
 */
#include "ui_app.h"

//要切换导航页按钮的名字
static const enum Chinese_English_shift language_obj_buttons_id[] = {
    facility,
    MODE,
    SETTING,
};

//左边导航页按钮结构体 (结构体数组)    
struct
{
  const void *img_off;
  const void *img_on;
  int x;
  int y;
} obj_buttons[] = {
    {&home_off, &home_on, 10, -5},
    {&mode_off, &mode_on, 10, -15},    //-2
    {&setting_off, &setting_on, 10, -15},    //-2
};

int g_lfd = 0;
int g_cfd = 0;
char *g_cBuf;   //用于存放从套接字中接收到的数据
static bool s_enteredScreenMode = false; // 屏保标志位
static bool s_password_flag = false;  //给第一次弹出密码框
int g_light_fd;   //连接灯光套接字
char PUB_BUF[256];      //上传给ＭＱＴＴ服务器数据的buf
All_Data home_data;

static const lv_img_dsc_t *img_arr[] = { &Image1_big,
                          &Image2_big,
                          &Image3_big, 
                          &Image4_big, 
                          &Image5_big, 
                          &Image6_big};

/**
 * 滑动页面时切换页面
 * @param e            指向事件描述符的指针
 * */
static void home_tileview(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *active_tile = lv_tileview_get_tile_act(home_data.content);  //获取当前显示的 tile 对象

  //当页面滑动时
  if (code == LV_EVENT_VALUE_CHANGED)
  {
    for (int i = 0; i < 3; i++)
    {
      lv_imgbtn_set_state(home_data.home_button[i], LV_IMGBTN_STATE_RELEASED);      //初始化按钮的状态
      if (active_tile == home_data.home_pages[i])   //判断现在是pages[1]\pages[2]\pages[3]哪个页面活跃
      {
        lv_imgbtn_set_state(home_data.home_button[i], LV_IMGBTN_STATE_PRESSED);  //使左边导航栏按钮处于按下状态
      }
    }
  }
}

/**
 * 创建滑动页面
 * @param parent      指向一个对象的指针，它将是新平铺视图的父类
 * */
static void home_page_box(lv_obj_t *parent)
{
  home_data.content = lv_tileview_create(parent); // content card
  lv_obj_add_event_cb(home_data.content, home_tileview, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_set_style_bg_opa(home_data.content, 0, LV_STATE_DEFAULT);       //设置背景颜色透明度
  lv_obj_set_size(home_data.content, 904, 591);
  lv_obj_set_pos(home_data.content, 120, 9);
  lv_obj_set_style_width(home_data.content, 0, LV_PART_SCROLLBAR);       //用于设置对象（如按钮、标签等）的宽度样式,这里用于设置主页内容的宽度样式（selector：这是样式选择器，用于指定你想修改的具体样式属性或状态。例如，你可以指定默认状态 (LV_STATE_DEFAULT)、按下状态 (LV_STATE_PRESSED) 等。)

  for (int i = 0; i < 3; i++)
  {
    home_data.home_pages[i] = lv_tileview_add_tile(home_data.content, 0, i, LV_DIR_BOTTOM | LV_DIR_TOP);    //添加页面(上下滚动)
  }

  home_data.home_page = home_data.home_pages[0];
  home_data.mode_page = home_data.home_pages[1];
  home_data.setting_page = home_data.home_pages[2];
}

/**
 * 导航点击事件
 * @param e           指向事件描述符的指针     
 * */
static void home_btn_event(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_user_data(e);

  //当发生点击事件或页面滑动时，判断哪个图像按钮处于该事件，对图像按钮的样式发生改变
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_VALUE_CHANGED)
  {
    for (int i = 0; i < 3; i++)
    {
      lv_imgbtn_set_state(home_data.home_button[i], obj == home_data.home_button[i] ? LV_IMGBTN_STATE_PRESSED : LV_IMGBTN_STATE_RELEASED);
      if (obj == home_data.home_button[i])
      {
        //主页是平铺视图,每页都有个id
        lv_obj_set_tile_id(home_data.content, 0, i, LV_ANIM_ON);   //设置当前显示的视图,根据页面ｉｄ设置  (平铺视图部件lv_tileview)
      }
    }
  }
}

/**
 * 导航栏按键范围
 * @param obj          指向父对象的指针
 * @param y_ofs        y坐标对齐后的偏移量 (对齐后不能再调坐标)
 * @return             返回一个矩形的基本对象
 * */
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
 * @param obj          指向图像对象的指针
 * @param img_src      指向图像按钮释放状态时，图像源的指针
 * @param img_crs      指向图像按钮按下状态时，图像源的指针
 * @param x            图像按钮对象的x坐标
 * @param x_ofs        对齐后的X坐标偏移量 (对齐后可以再调坐标)     中文显示没有用到
 * @param y_ofs        y坐标对齐后的偏移量 (对齐后不能再调坐标)
 * @param label_text   文本字符串
 * @return             返回图像按钮对象
 * **/
static lv_obj_t *lv_imgbtn_create_with_label(lv_obj_t *obj, const void *img_src, const void *img_crs, lv_coord_t x, lv_coord_t x_ofs, lv_coord_t y_ofs, const char *label_text)
{

  lv_obj_t *home_fout = lv_fout_create(obj, y_ofs);
  lv_obj_t *img_btn = lv_imgbtn_create(home_fout);   //图像按钮对象
  lv_imgbtn_set_src(img_btn, LV_IMGBTN_STATE_RELEASED, NULL, img_src, NULL);     //按钮处于释放状态时，按钮中间图像为 img_src
  lv_imgbtn_set_src(img_btn, LV_IMGBTN_STATE_PRESSED, NULL, img_crs, NULL);      //按钮处于按下状态时，按钮中间图像为 img_crs
  lv_obj_set_size(img_btn, 40, 50);
  lv_obj_set_x(img_btn, x);
  lv_obj_t *label = lv_label_create(obj);
  lv_label_set_text(label, label_text);
  lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_align_to(label, img_btn, LV_ALIGN_CENTER, -16, 30);
  lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, 0);                    //设置文本字体
  lv_obj_add_event_cb(home_fout, home_btn_event, LV_EVENT_ALL, img_btn);
  lv_obj_add_event_cb(img_btn, home_btn_event, LV_EVENT_ALL, img_btn);
  return img_btn;
}

/**
 * 创建导航页面
 * @param obj         指向图像对象的指针
 * */
static void ui_navigation_main_menu(lv_obj_t *obj)
{
  for (int i = 0; i < 3; i++)
  {
    //创建三个按钮@param  obj        指向一个对象的指针，它将是新图像的父对象
    home_data.home_button[i] = lv_imgbtn_create_with_label(obj, obj_buttons[i].img_off, obj_buttons[i].img_on, obj_buttons[i].x, obj_buttons[i].y, 70 + i * 160, UI_MLANG_STR(language_obj_buttons_id[i]));
  }
  lv_imgbtn_set_state(home_data.home_button[0], LV_IMGBTN_STATE_PRESSED);   //初始化默认选中第一个图标按钮，让第一个按钮的样式处于高亮状态。
}

/**
 * 创建主页背景
 * @param obj         指向一个对象的指针，它将是新图像的父对象
 * */
static void home_bg(lv_obj_t *obj)
{
  lv_obj_t *lv_background = lv_img_create(obj); // 创建一个背景页对象
  lv_img_set_src(lv_background, &background);   //设置背景页对象的背景数据
  lv_obj_t *lv_detailed = lv_img_create(obj); // 创建一个左边导航页对象
  lv_img_set_src(lv_detailed, &detailed);     //设置左边导航页对象的图像数据
  ui_navigation_main_menu(lv_detailed);
}

/**
 * 屏保模式时的点击事件
 * @param e           指向事件描述符的指针     
 * */
static void SettingScreenBack_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);

  if (code == LV_EVENT_CLICKED)
  {

    lv_obj_del(obj);
    g_password_lock_open = false;
    lv_gui_password_keyboard_display();
    s_enteredScreenMode = false;
  }

}

/**
 * 进入屏保模式
 * @param obj         指向一个对象的指针，它将是新图像的父对象
 * */
static void SettingScreenBox(lv_obj_t *parent)
{
  log_printf("屏保函数");
  if (s_password_flag == true && g_password_lock_open == false)
  { 
    if (g_pwd_main_cont != NULL)
    {
      log_printf("删除密码框");
      lv_obj_del(g_pwd_main_cont);  //删除密码框
      g_pwd_main_cont = NULL;
    }
  }

  log_printf("设置屏保图片源");
  lv_obj_t *screensaverCard = lv_img_create(parent);
  lv_img_set_src(screensaverCard, img_arr[setting.ScreenSaveid]);
  lv_obj_add_flag(screensaverCard, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(screensaverCard, SettingScreenBack_event_cb, LV_EVENT_CLICKED, NULL);
}

/**
 * 屏保任务
 * @param timer         指向定时器
 * */
void UpdateTask(lv_timer_t *timer)
{
    if (setting.screen_save_time == 0)
    {
      setting.screen_save_time = 60*1000;
    }  

    uint32_t screenModeTime = setting.screen_save_time;
    if (lv_disp_get_inactive_time(lv_disp_get_default()) >= screenModeTime)
    {
      if (!s_enteredScreenMode)
      {
        log_printf("进入屏保模式%ld",lv_disp_get_inactive_time(lv_disp_get_default()));
        SettingScreenBox(lv_scr_act());
        s_enteredScreenMode = true;
      }
    }
  
}


void create_lv_layout(lv_obj_t *scr)
{

/*******************************开线程********************************/

  pthread_t tid_mu;
  pthread_create(&tid_mu, NULL, create_client_mu, NULL);

/******************************定时器任务*****************************/

  // 读取屏幕配置文件
  updateSettingData(&setting, SCREEN_SETTING_JSON);

  // 创建屏保任务
  lv_timer_create(UpdateTask, 500, NULL);

  //创建更新温湿度数据任务
  // lv_timer_create(timer_data_callback, 50000, NULL);

  // //创建更新天气数据任务
  lv_timer_create(timer_weather_callback, 60000, NULL);

  // 创建ＭＱＴＴ心跳任务
  lv_timer_create(timer_mqtt_callback, 40000, NULL);

  // 创建更新历史温湿度数据
  // lv_timer_create(timer_average_callback, 60000, NULL);


/******************************界面UI*****************************/

  home_bg(scr);              // 设置主页背景
  home_page_box(scr);        // 框架(创建滑动页面)
  create_wifi_and_time(scr); // wifi 和 时间

  CreateHomePage(home_data.home_page);       // 主页
  CreateModePage(home_data.mode_page);       // 模式页面
  CreateSettingPage(home_data.setting_page); // 设置页面

  lv_gui_password_keyboard_display();
  s_password_flag = true;

/******************************创建服务器*****************************/

  create_serverTemHum();

/******************************连接服务器*****************************/

  connect_mqtt();   // 连接mqtt服务器
  create_client_light();  //　连接灯光服务器
  // create_client_zhongli();
  create_client_abesn();  //连接艾比森ｐｌｃ服务器
  create_client_kls();   //连接播控软件
  create_client_led();   //连接led大屏

  /* 创建线程池，池里最小3个线程，最大10，队列最大10 */
  // thp = threadpool_create(3, 10, 10);
  // if (thp == NULL) {
  //     printf("threadpool_create fail\n");
  //     return 0;
  // }

}
