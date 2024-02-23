/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lv_examples/lv_demo.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_drivers/indev/mousewheel.h"
#include "lvgl/demos/music/lv_demo_music.h"
#include "ui_app.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);

int lfd = 0;
int cfd = 0;
char *cBuf;   //用于存放从套接字中接收到的数据

lv_obj_t * pcr_obj;

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

// // 获取温度数据
// int searchTemp(char *Buf, char *Res)
// {
// 	char *Begin = NULL;    //定义开始指针
// 	char *End = NULL;      //定义结束指针

//     /*寻找第一次出现<temp>的位置  <temp>温度</temp>...
//     再偏移strlen("<temp>")个地址可以得到温度</temp>...*/
// 	Begin = strstr(Buf, "<temp>");//寻找第一次出现<temp>的位置
//   End = strstr(Buf, "</temp>");//寻找第一次出现</temp>的位置

// 	if(Begin == NULL || End == NULL || Begin > End)
//   {
//     printf("寻找错误!\n");
//   }else{
// 		Begin = Begin + strlen("<temp>");//温度</temp>...
// 		memcpy(Res, Begin, End-Begin);//获得Begin和End之间的值
// 	}
// 	return 0;
// }

// //获取湿度数据
// int searchHum(char *Buf, char *Res)
// {
// 	char *Begin = NULL;
// 	char *End = NULL;

// 	Begin = strstr(Buf, "<hum>");
// 	End = strstr(Buf, "</hum>");

// 	if(Begin == NULL || End == NULL || Begin > End)
//   {
//     printf("寻找错误!\n");
//   }else{
// 		Begin = Begin + strlen("<hum>");
// 		memcpy(Res, Begin, End-Begin);
// 	}
// 	return 0;
// }

// void *listening(void * parg)
// {
//   // 4. 阻塞等待并接受客户端连接
//   struct sockaddr_in cliaddr;
//   int clilen = sizeof(cliaddr);
//   cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
//   if(cfd == -1)
//   {
//       perror("accept");
//       exit(0);
//   }
//   // 打印客户端的地址信息
//   char ip[24] = {0};
//   printf("客户端的IP地址: %s, 端口: %d\n",
//           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
//           ntohs(cliaddr.sin_port));
//   while (1)
//   {
//     // 5. 和客户端通信
//     // 接收数据
//     char buf[1024];
//     memset(buf, 0, sizeof(buf));
//     int len = read(cfd, buf, sizeof(buf));
//     if(len > 0)
//     {
//       cBuf = buf;
//       searchTemp(cBuf, temp_data);
//       searchHum(cBuf, hum_data);
//       printf("温度: %s, 湿度: %s\n", temp_data, hum_data);
//     }
//   }

// }


int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

/**
 * 移植时需要注意一下显示器的水平、垂直分辨率
 * 
 * 初始化ｌｖｇｌ
 * １　调用lv_init()初始化lvgl库
 * ２　初始化驱动程序
 * ３　在lvgl中注册显示和输入设备驱动程序
 * ４　在中断中每隔ｘ毫秒调用lv_tick_inc(x)用于告知lvgl经过的时间
 * ５　每隔ｘ毫秒定期调用lv_task_handler()用于处理与lvgl相关的任务
 */

  lv_init();                     //初始化lvgl库

  hal_init();                   //硬件初始化：包括显示设备、输入设备   

  // lv_obj_t *scr = lv_disp_get_scr_act(NULL);
  // pcr_obj = lv_obj_create(scr);
  // lv_obj_remove_style_all(pcr_obj);
  // lv_obj_set_style_opa(pcr_obj, LV_OPA_COVER, LV_PART_MAIN);
  // lv_obj_set_style_bg_color(pcr_obj, lv_color_black(), LV_PART_MAIN);
  // lv_obj_set_size(pcr_obj, 1024, 600);
  create_lv_layout(lv_disp_get_scr_act(NULL));

 
  // pthread_t tid_listen;  //用于监听
  // //创建监听的套接字
  // lfd = socket(AF_INET, SOCK_STREAM, 0);
  // if(lfd == -1)
  // {
  //     perror("socket");
  //     exit(0);
  // }

  // char pp[24] = {0};
  // // 2. 将socket()返回值和本地的IP端口绑定到一起
  // struct sockaddr_in addr;
  // addr.sin_family = AF_INET;
  // addr.sin_port = htons(8266);   // 大端端口
  // //addr.sin_addr.s_addr = inet_addr("192.168.17.218"); 

  // addr.sin_addr.s_addr = INADDR_ANY; 
  // printf("服务器的IP地址: %s, 端口: %d\n",
  //         inet_ntop(AF_INET, &addr.sin_addr.s_addr, pp, sizeof(pp)),
  //         ntohs(addr.sin_port));
  // // 设置ＳＯ_ＲＥＵＳＥＡＤＤＲ，快速重启服务器，防止出现Address in use
  // int on = 1;
  // setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  // int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
  // if(ret == -1)
  // {
  //     perror("bind");
  //     exit(0);
  // }

  // // 3. 设置监听
  // ret = listen(lfd, 128);
  // if(ret == -1)
  // {
  //     perror("listen");
  //     exit(0);
  // }

  // pthread_create(&tid_listen, NULL, listening, NULL);
  



  // ui_label_set_text();
  // ui_init();
  // ();
  // lv_style_set_border_side  (lv_scr_act(), LV_STATE_DEFAULT, LV_BORDER_SIDE_NONE);

  //周期性运行 lv_timer_handler　接口，该接口内部主要处理：绘制、获取输入信息等。
  while (1)
  {
    lv_timer_handler();
    usleep(5 * 1000);
  }

  // close(cfd);
  // close(lfd);
  // closeSocket(multitrack_fd);
  // closeSocket(mqtt_fd);
  // closeSocket(g_light_fd);

  // threadpool_distory(thp);

  lv_timer_del(timer);

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  monitor_init();
  /* Tick init.
   * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about
   * how much time were elapsed Create an SDL thread to do this*/
  //创建 tick　线程
  SDL_CreateThread(tick_thread, "tick", NULL);

  /*Create a display buffer*/
  //创建绘制缓冲区
  static lv_disp_draw_buf_t disp_buf1;                                      // 创建一个显示缓冲区
  static lv_color_t buf1_1[MONITOR_HOR_RES * 100];                          // 创建一个缓冲区
  static lv_color_t buf1_2[MONITOR_HOR_RES * 100];                          // 创建一个缓冲区
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, MONITOR_HOR_RES * 100); // 初始化缓冲区

  /*Create a display*/
  //注册显示驱动，用于拷贝渲染信息到显示区域
  static lv_disp_drv_t disp_drv;                        // 创建一个显示驱动
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/ // 初始化显示驱动
  disp_drv.draw_buf = &disp_buf1;                       // 设置显示缓冲区
  disp_drv.flush_cb = monitor_flush;                    // 设置显示驱动的刷新回调函数
  disp_drv.hor_res = MONITOR_HOR_RES;                   // 设置显示驱动的水平分辨率
  disp_drv.ver_res = MONITOR_VER_RES;                   // 设置显示驱动的垂直分辨率
  disp_drv.antialiasing = 1;                            // 设置显示驱动的抗锯齿

  lv_disp_t *disp = lv_disp_drv_register(&disp_drv); // 注册显示驱动

  lv_theme_t *th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT); // 初始化默认主题
  lv_disp_set_theme(disp, th);                                                                                                                             // 设置默认主题

  lv_group_t *g = lv_group_create(); // 创建一个组
  lv_group_set_default(g);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  mouse_init();
  static lv_indev_drv_t indev_drv_1;
  lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
  indev_drv_1.type = LV_INDEV_TYPE_POINTER;

  /*This function will be called periodically (by the library) to get the mouse position and state*/
  indev_drv_1.read_cb = mouse_read;
  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

  keyboard_init();
  static lv_indev_drv_t indev_drv_2;
  lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv_2.read_cb = keyboard_read;
  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
  lv_indev_set_group(kb_indev, g);
  mousewheel_init();
  static lv_indev_drv_t indev_drv_3;
  lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
  indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
  indev_drv_3.read_cb = mousewheel_read;

  lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
  lv_indev_set_group(enc_indev, g);

  /*Set a cursor for the mouse*/
  LV_IMG_DECLARE(mouse_cursor_icon);                  /*Declare the image file.*/
  lv_obj_t *cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
  lv_img_set_src(cursor_obj, &mouse_cursor_icon);     /*Set the image source*/
  lv_indev_set_cursor(mouse_indev, cursor_obj);       /*Connect the image  object to the driver*/
}

/**
 * A task to measure the elapsed time for LVGL
 * 该线程为　lvgl　提供时间基准，如果在 linux　这种比较完善的OS上，可以不用专门开个线程调用lv_tick_inc,可以设置 LV_TICK_CUSTOM
 * @param data unused
 * @return never return
 */
static int tick_thread(void *data)
{
  (void)data;

  while (1)
  {
    SDL_Delay(5);
    lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
  }

  return 0;
}
