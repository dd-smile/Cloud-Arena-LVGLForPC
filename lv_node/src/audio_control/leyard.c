/*
 * @Description: 音频控制面板－利亚德
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-26 11:28:17
 * @LastEditTime: 2023-09-28 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/audio_control/leyard.c
 */
#include "ui_app.h"

// int synchronous_fd = 0;  //实时控制客户端的文件描述符
int multitrack_fd = 0;   //多轨控制客户端的文件描述符
int led_fd = 0;   //LED大屏客户端的文件描述符
int multitrack_red = -1;   //多轨控制客户端是否连接上服务器
struct sockaddr_in seraddr;
char buf[1024];  //用于存放发送的数据
unsigned char packet[16];  //存放静音16进制数据
unsigned char packet_control[19];  //存放控制16进制数据

unsigned char packet_led1[39] = {0xED,0xCB,0x28,0x00,0x30,0x02,0x01,0x00,0x01,0x00,0x00,0x55,0x55,0x55,
                0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x2B};

unsigned char packet_led2[39] = {0xED,0xCB,0x28,0x00,0x30,0x02,0x01,0x00,0x01,0x01,0x00,0x55,0x55,0x55,
                0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x2C}; 

unsigned char packet_led3[39] = {0xED,0xCB,0x28,0x00,0x30,0x02,0x01,0x00,0x01,0x02,0x00,0x55,0x55,0x55,
                0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x2D};

static void led_Controls_event_cb(lv_event_t *e);
static void multitrack_Controls_event_cb(lv_event_t *e);
static void lv_back_leyard(lv_event_t *e);

static lv_obj_t *connect_label;  //连接提示框


/*--------------------------------通信--------------------------------*/

// //发送静音的数据
// void *synchronous_working(void *parg)
// {
//     // 和客户端通信
//     // 发送数据
//     write(synchronous_fd, buf, strlen(buf)+1);
// }

// //发送控制的数据
// void *synchronous_c_working(void *parg)
// {
//     // 和客户端通信
//     // 发送数据
//     write(synchronous_fd, buf, strlen(buf)+1);
// }

//检测音频服务器是否断开
void audio_working()
{
    if (socketconnected(multitrack_fd) == 0)
    {   
        closeSocket(multitrack_fd);
        lv_label_set_text(connect_label, "请检查服务器是否开启");
    }
    else
    {
        lv_label_set_text(connect_label, "");
        write(multitrack_fd, buf, strlen(buf)+1);
    }
}


/**
 * 创建实时控制客户端
 * @param parent            指向父对象的指针     
 * */
// static void *create_client_sy()
// {   
//     if(synchronous_fd == 0)
//     {
//         synchronous_fd = createSocket();  //创建套接字
//         connectToHost(synchronous_fd, "192.168.17.20", 11011);  //连接服务器
//     }
// }


/**
 * 创建LED客户端
 * @param parent            指向父对象的指针     
 * */
static void *create_client_led()
{
    if(led_fd == 0)
    {
        // 1. 创建通信的套接字
        led_fd= socket(AF_INET, SOCK_DGRAM, 0);
        if(led_fd == -1)
        {
            perror("socket");
            exit(0);
        }
        
        // 初始化服务器地址信息
        seraddr.sin_family = AF_INET;
        seraddr.sin_port = htons(48631);    // 大端
        // inet_pton(AF_INET, "192.168.1.223", &seraddr.sin_addr.s_addr);
        inet_pton(AF_INET, "192.168.1.101", &seraddr.sin_addr.s_addr);
    }
}

/*------------------------------界面ＵＩ----------------------------------*/

/**
 * 创建矩形对象，作为按钮的模板
 * @param obj           指向父对象的指针    
 * @return              返回一个矩形新对象
 * */
static lv_obj_t *swich_btn_card(lv_obj_t *obj)
{
    lv_obj_t *box = lv_obj_create(obj);        //创建一个矩形对象
    lv_obj_set_size(box, 500, 300);
    lv_obj_set_style_bg_color(box, lv_color_hex(0x1A1F2E), LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(box, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(box, 10, LV_STATE_DEFAULT);          //设置背景圆角半径
    lv_obj_clear_flag(box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(box, LV_ALIGN_CENTER, 0, 0);
    return box;
}

/**
 * 创建按钮
 * @param obj          指向父对象的指针    
 * @param text         按钮的名称
 * @param x_ofs        水平偏移 x_ofs
 * @param y_ofs        水平偏移 y_ofs
 * @param type         判断是什么控制类型
 * @param index        按到第几个按钮
 * */
static void create_button(lv_obj_t *obj, const char *text, lv_coord_t x_ofs, lv_coord_t y_ofs, uint8_t type, uint8_t index)
{
    lv_obj_t *btn = lv_obj_create(obj);
    lv_obj_set_size(btn, 70, 40);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(btn, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, 80, LV_PART_MAIN);

    
    //判断是什么设备按钮，从而添加控制事件
    switch(type)
    {
        case 0:     //LED大屏控制
            lv_obj_set_user_data(btn, index);   //设置用户数据，表示哪个按钮
            lv_obj_add_event_cb(btn, led_Controls_event_cb, LV_EVENT_ALL, NULL);  
            break;
        case 1:     //多轨控制
            lv_obj_set_user_data(btn, index);   //设置用户数据，表示哪个按钮
            lv_obj_add_event_cb(btn, multitrack_Controls_event_cb, LV_EVENT_ALL, NULL);  
            break;
    }
    
    lv_obj_align(btn, LV_ALIGN_CENTER, x_ofs, y_ofs);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_center(label);
}

// 弹出页面返回上一级按钮
static lv_obj_t *create_leyard_back(lv_obj_t *obj)
{
    lv_obj_t *lv_obj = lv_obj_create(obj);
    lv_obj_set_size(lv_obj, 90, 50);
    lv_obj_add_flag(lv_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_LEFT, 0, -13);
    lv_obj_set_style_bg_opa(lv_obj, 0, LV_STATE_DEFAULT); // 设置背景透明
    lv_obj_set_style_border_width(lv_obj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(lv_obj, LV_OBJ_FLAG_SCROLLABLE);                 // 禁用滚动条
    lv_obj_add_event_cb(lv_obj, lv_back_leyard, LV_EVENT_CLICKED, time); // 添加返回事件
    lv_obj_t *back_btn = lv_label_create(lv_obj);
    lv_label_set_text(back_btn, LV_SYMBOL_LEFT);
    lv_obj_align(back_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(back_btn, lv_color_hex(0xffffff), LV_STATE_DEFAULT);

    return lv_obj;
}

// 创建一个弹出页面
static lv_obj_t *create_leyard_page(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *popup_page = lv_obj_create(parent);
    lv_obj_set_size(popup_page, 1024, 600);
    lv_obj_align(popup_page, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(popup_page, lv_color_hex(0x141a23), 0);
    lv_obj_set_style_border_width(popup_page, 0, LV_STATE_DEFAULT);

    lv_obj_t *Titleobj = lv_obj_create(parent);
    lv_obj_set_size(Titleobj, 1024, 64);
    lv_obj_align(Titleobj, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(Titleobj, lv_color_hex(0x1f2633), 0);
    lv_obj_set_style_border_width(Titleobj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(Titleobj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(Titleobj, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动条
    card_create_24_text(Titleobj, title, x, y);
    create_leyard_back(Titleobj); // 返回上一级按钮

    return popup_page;
}

/**
 * 创建LED大屏控制界面
 * @param parent            指向父对象的指针     
 * */
static void *LedControl(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_leyard_page(scr, "LED切屏", 0, 0);


    // lv_obj_t *card_1 = lv_img_create(popup_page);
    // lv_img_set_src(card_1, &BG_1);
    // lv_obj_set_pos(card_1, 20, 400);

    // lv_obj_t *card_2 = lv_img_create(popup_page);
    // lv_img_set_src(card_2, &BG_2);
    // lv_obj_set_pos(card_2, 50, 400);

    // lv_obj_t *card_3 = lv_img_create(popup_page);
    // lv_img_set_src(card_3, &BG_3);
    // lv_obj_set_pos(card_3, 100, 400);

    // lv_obj_t *card_4 = lv_img_create(popup_page);
    // lv_img_set_src(card_4, &BG_4);
    // lv_obj_set_pos(card_4, 150, 400);
    // lv_obj_add_flag(card_4,LV_OBJ_FLAG_HIDDEN);
    






    //连接服务器
    create_client_led();

    lv_obj_t *btn_synchronous = swich_btn_card(popup_page);
    create_button(btn_synchronous, "HDMI1", -90, 0, 0, 0);
    create_button(btn_synchronous, "HDMI2", 0, 0, 0, 1);
    create_button(btn_synchronous, "HDMI3", 90, 0, 0, 2);
}

/**
 * 创建多轨控制界面
 * @param parent            指向父对象的指针     
 * */
static void *MultitrackControl(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_leyard_page(scr, "多轨控制", 0, 0);

    lv_obj_t *btn_multitrack = swich_btn_card(popup_page);
    create_button(btn_multitrack, "音乐1", -90, -100, 1, 0);
    create_button(btn_multitrack, "音乐2", 0, -100, 1, 1);
    create_button(btn_multitrack, "音乐3", 90, -100, 1 ,2);
    create_button(btn_multitrack, "音乐4", -90, -40, 1 ,3);
    create_button(btn_multitrack, "大自然", 0, -40, 1 ,4);
    create_button(btn_multitrack, "下一首", 90, -40, 1 ,8);
    create_button(btn_multitrack, "停止", 0, 10, 1 ,9);
    create_button(btn_multitrack, "音量增", -90, 40, 1 ,5);
    create_button(btn_multitrack, "音量减", 90, 40, 1 ,6);
    // create_button(btn_multitrack, "初始音量", 0, 100, 1 ,7);
    connect_label = lv_label_create(btn_multitrack);
    static lv_style_t connect_label_style;
    lv_style_reset(&connect_label_style);
    lv_style_init(&connect_label_style);
    lv_style_set_radius(&connect_label_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&connect_label_style, 0); //设置边框宽度
    lv_style_set_text_color(&connect_label_style , lv_palette_main(LV_PALETTE_RED));  // 字体颜色设置为红色
    lv_label_set_long_mode(connect_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 长文本循环滚动
    lv_obj_add_style(connect_label, &connect_label_style, 0); // 给btn_label添加样式
    lv_obj_set_style_text_font(connect_label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_align(connect_label, LV_ALIGN_CENTER, 0, 110);
	lv_label_set_text(connect_label, ""); // 设置文本内容

}

/*------------------------------点击事件----------------------------------*/

// 返回上一级
static void lv_back_leyard(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {   
        // //关闭套接字
        // closeSocket(synchronous_fd);
        // closeSocket(multitrack_fd);
        lv_obj_del(obj->parent->parent);
    }
}

/**点击卡片事件**/
void add_Audio_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_current_target(e);   //获取当前点击对象

    if (code == LV_EVENT_CLICKED)
    {
        switch ((int)obj->user_data)
        {
        case 0:  //led控制
            LedControl(lv_scr_act());
            break;

        case 1:   //多轨控制
            MultitrackControl(lv_scr_act());
            break;
        }
    }
}

/**LED大屏控制按钮点击事件**/
static void led_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_current_target(e);   //获取当前点击对象

    if (code == LV_EVENT_CLICKED)
    {
        //判断是哪个按钮，进行封装数据
        switch ((int)obj->user_data)
        {
            case 0: 
                sendto(led_fd, packet_led1, sizeof(packet_led1)/sizeof(packet_led1[0]), 0, (struct sockaddr*)&seraddr, sizeof(seraddr));
                break;

            case 1:  
                sendto(led_fd, packet_led2, sizeof(packet_led2)/sizeof(packet_led2[0]), 0, (struct sockaddr*)&seraddr, sizeof(seraddr));
                break;
        
            case 2:
                sendto(led_fd, packet_led3, sizeof(packet_led3)/sizeof(packet_led3[0]), 0, (struct sockaddr*)&seraddr, sizeof(seraddr));
                break;
        }
    }

}

/**多轨控制按钮点击事件**/
static void multitrack_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_current_target(e);   //获取当前点击对象


    if (code == LV_EVENT_CLICKED)
    {
        //判断是哪个按钮，进行封装数据
        switch ((int)obj->user_data)
        {
        case 0:  //音乐1
                sprintf(buf, "music1@5F");  //播放
                /* 向线程池中添加任务 */
                //threadpool_add(thp, (void *)&multitrack_working, 0);
                audio_working();
            break;
        case 1:   //音乐2
                sprintf(buf, "music2@5F");  //播放
                audio_working();
            break;
        case 2:   //音乐3
                sprintf(buf, "music3@5F");  //播放
                audio_working();
            break;
        case 3:   //音乐4
                sprintf(buf, "music4@5F");  //播放
                audio_working();
            break;
        case 4:   //大自然音乐
                sprintf(buf, "daziran@5F");  //播放
                audio_working();
            break;
        case 5:   //音量增加5db
                sprintf(buf, "V+@5F");  
                audio_working();
            break;
        case 6:   //音量减少5db
                sprintf(buf, "V-@5F");
                audio_working();
            break;
        // case 7:   //初始音量-15db
        //     sprintf(buf, "-15db@5F");
        //     write(multitrack_fd, buf, strlen(buf)+1);
        //     break;
        case 8:   //切换下一首
                sprintf(buf, "nest@5F");
                audio_working();
            break;
        case 9:   //暂停播放
                sprintf(buf, "stop@5F");
                audio_working();
            break;
        }
    }
}


/**
 * 创建卡片页面,用于展示要进入的控制类型
 * @param parent          指向父对象的指针 
 * @param x               设置卡片x坐标
 * @param y               设置卡片y坐标
 * @param name            卡片类名称
 * @param image           要显示的图片id号(控制类型)
 * */
static void *card_audio(lv_obj_t *parent, int x, int y, const char *name, uint8_t image)
{   

    static const lv_img_dsc_t *img_table[] = {&synchronous_control_img, &multitrack_control_img}; //控制类型图片数组

    lv_obj_t *card = lv_img_create(parent);
    lv_img_set_src(card, &card_background);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_style_bg_opa(card, LV_OPA_0, 0);

    lv_obj_t *card_son = lv_img_create(card);
    lv_img_set_src(card_son, img_table[image]);

    lv_obj_t *label = lv_label_create(card);
    lv_label_set_text(label, name);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 3);

    lv_obj_t *label_son = lv_label_create(card_son);
    lv_obj_align(label_son, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(label_son, "");
    lv_obj_set_style_text_color(label_son, lv_color_hex(0xffffff), LV_STATE_DEFAULT);    //默认状态
    lv_obj_set_style_text_font(label_son, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label_son, lv_color_hex(0x14E7F0), LV_STATE_CHECKED);    //选中状态
    //添加两种标志位
    lv_obj_add_flag(card_son, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(card_son, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_user_data(card_son, image);   //设置用户数据，表示进入哪个页面
    lv_obj_add_event_cb(card_son, add_Audio_event_cb, LV_EVENT_ALL, NULL);

}



/**
 * 创建音频控制面板－利亚德
 * @param parent            指向父对象的指针     
 * */
void CreateAudioPage(lv_obj_t *parent)
{
    card_audio(parent, 30, 50, "LED切屏", 0);
    card_audio(parent, 400, 50, "多轨控制", 1);
}