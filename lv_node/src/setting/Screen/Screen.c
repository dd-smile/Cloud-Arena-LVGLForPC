#include "ui_app.h"


lv_setting_data_t setting; //屏幕设置结构体   用于退出页面时候的保存
lv_setting_data_t setting_buf; //屏幕设置结构体   用于处于页面详情时候的保存
lv_obj_t *In_slider;   
lv_obj_t *On_slider;   
lv_obj_t *In_slider;   

uint32_t btnmatrix_id;   // 屏保按钮id
lv_obj_t *lv_scr_obj;    // 屏保对象
lv_obj_t *lv_Scrsw_obj;  // 开关对象
lv_obj_t *slider_label;  // 滑动条的文字（外面）
lv_obj_t *InSliderLabel; // 滑动条的文字（里面）


/**
 * 保存设置的事件
 * @param e         指向事件描述符的指针
 * */
static void save_setting_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        setting.Btnmid           = setting_buf.Btnmid;
        setting.screen_save_time = setting_buf.screen_save_time;
        setting.ScreenSaveid     = setting_buf.ScreenSaveid;
        // 保存设置到json
        saveSettingData(&setting, SCREEN_SETTING_JSON);
        print_setting(&setting);
        
        lv_obj_del(obj->parent->parent);
    }
}

/**
 * 返回上一级弹出是否保存页面的事件
 * @param e         指向事件描述符的指针
 * */
static void screen_back_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        if(setting_buf.Btnmid != setting.Btnmid || setting_buf.ScreenSaveid != setting.ScreenSaveid)
        {
            create_popup(lv_scr_act(), "", "确定", "取消", "是否保存设置", save_setting_event, NULL);
        }
        lv_obj_del(obj->parent->parent);
    }
}

/**
 * 弹出页面返回上一级按钮
 * @param obj         指向一个对象的指针，它将是新图像的父对象
 * */
lv_obj_t *create_page_back(lv_obj_t *obj)
{
    lv_obj_t *lv_obj = lv_obj_create(obj);
    lv_obj_set_size(lv_obj, 90, 50);
    lv_obj_add_flag(lv_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_LEFT, 0, -13);
    lv_obj_set_style_bg_opa(lv_obj, 0, LV_STATE_DEFAULT); // 设置背景透明
    lv_obj_set_style_border_width(lv_obj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(lv_obj, LV_OBJ_FLAG_SCROLLABLE);                 // 禁用滚动条
    lv_obj_add_event_cb(lv_obj, screen_back_event, LV_EVENT_CLICKED, time); // 添加返回事件
    lv_obj_t *back_btn = lv_label_create(lv_obj);
    lv_label_set_text(back_btn, LV_SYMBOL_LEFT);
    lv_obj_align(back_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(back_btn, lv_color_hex(0xffffff), LV_STATE_DEFAULT);

    return lv_obj;
}

// 外面的亮度
void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);

    setting.brightness = lv_slider_get_value(slider);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    sprintf(setting.On_screen, "%s", buf);
    // printf("外面的亮度 = %s\n", setting.On_screen);
    char cmd[128];
    sprintf(cmd, "echo %d > /sys/class/pwm/pwmchip0/pwm0/duty_cycle", (int)lv_slider_get_value(slider));
    system(cmd);
    saveSettingData(&setting, SCREEN_SETTING_JSON);
}

// 里面的亮度
void Inslider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    setting.brightness = lv_slider_get_value(slider);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_slider_get_value(slider));
    lv_label_set_text(InSliderLabel, buf);
    sprintf(setting.In_screen, "%s", buf);
    // printf("里面的亮度 = %s\n", setting.In_screen);
    char cmd[128];
    sprintf(cmd, "echo %d > /sys/class/pwm/pwmchip0/pwm0/duty_cycle", (int)lv_slider_get_value(slider));
    system(cmd);
    saveSettingData(&setting, SCREEN_SETTING_JSON);
}

/**
 * 屏保按钮点击事件
 * @param e         指向事件描述符的指针
 * */
static void scr_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *lv_scr_obj = lv_event_get_target(e);                      // 获取事件对象
    
    if (code == LV_EVENT_CLICKED)
    {                      
        setting_buf.Btnmid = lv_btnmatrix_get_selected_btn(lv_scr_obj); // 获取选中的按钮id
        printf("Btnmid:%d\n", setting_buf.Btnmid);
    
        switch (setting_buf.Btnmid)
        {
        case 0:
            setting_buf.screen_save_time = 60 * 1000;
            break;
        case 1:
            setting_buf.screen_save_time = 600 * 1000;
            break;
        case 2:
            setting_buf.screen_save_time = 1200 * 1000;
            break;
        case 3:
            setting_buf.screen_save_time = 1800 * 1000;
            break;
        case 4:
            setting_buf.screen_save_time = 3600 * 1000;
            break;
        case 5:
            setting_buf.screen_save_time = -1;
            break;
        default:
            break;

        }

    }
}

/**
 * 创建屏保按钮
 * @param parent         指向一个对象的指针，它将是新图像的父对象
 * */
lv_obj_t *create_screen_btn(lv_obj_t *parent)
{
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 50);
    static const char *map[] = {"1分钟", "10分钟", "20分钟", "30分钟", "1小时", ""};
    lv_obj_t *btnm = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(btnm, map);
    lv_obj_add_style(btnm, &style_btn, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(btnm, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(btnm, 0, 0);
    lv_obj_set_style_text_font(btnm, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);

    lv_obj_add_event_cb(btnm, scr_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_size(btnm, 980, 100);
    lv_btnmatrix_set_btn_ctrl_all(btnm, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(btnm, true);
    lv_obj_center(btnm);
    return btnm;
}


/**
 * 创建设备设置页面
 * @param parent         指向一个对象的指针，它将是新图像的父对象
 * */
void *create_allset_stlye(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_popup_page(scr, "屏幕设置", 0, 0);

    card_create_20_text(popup_page, "屏幕亮度设置", -410, -180);
    image_create(popup_page, &light, -450, -120);                                                                       // 创建图标
    In_slider = CreateSlider(popup_page, 50, -120, 5, 100, lv_color_hex(0x00d1fe), setting.brightness, 814, 44, false); // 创建进度条
    InSliderLabel = card_create_20_text(popup_page, setting.On_screen, -400, -120);
    lv_obj_add_event_cb(In_slider, Inslider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    card_create_20_text(popup_page, "自动进入屏幕保护", -395, -15);
    lv_obj_t *lv_screen = create_screen_btn(popup_page); // 屏保按钮
    lv_obj_align(lv_screen, LV_ALIGN_CENTER, -5, 65);
    lv_btnmatrix_set_btn_ctrl(lv_screen, setting.Btnmid, LV_BTNMATRIX_CTRL_CHECKED);

    setting_buf.brightness       = setting.brightness;
    setting_buf.Btnmid           = setting.Btnmid;
    setting_buf.screen_save_time = setting.screen_save_time;
    setting_buf.ScreenSaveid     = setting.ScreenSaveid;

    return NULL;
}

/*------------------------------点击事件----------------------------------*/

/**
 * 点击屏保事件
 * @param e         指向事件描述符的指针
 * */
void add_Screen_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {

        create_allset_stlye(lv_scr_act());
    }
}

/**
 * 创建屏幕设置卡片
 * @param parent         指向父对象的指针
 * */
void CreateScreen(lv_obj_t *parent)
{
    lv_obj_t *ImageObj = CreateOBJclick(parent);
    image_create(parent, &Screen, -100, -30);
    card_create_24_text(parent, "屏幕设置", 20, -30); // 创建标题
    lv_obj_add_event_cb(ImageObj, add_Screen_event_cb, LV_EVENT_ALL, NULL);

    if(setting.brightness == 0)
    {
        setting.brightness = 50; // 什么都没有设置的时候默认值为50
    }

    sprintf(setting.On_screen, "%d", setting.brightness);
    On_slider = CreateSlider(parent, 17, 35, 0, 100, lv_color_hex(0xffffff), setting.brightness, 200, 6, false); // 创建进度条
    slider_label = card_create_16_text(parent, setting.On_screen, -100, 35);                             // 创建标题
    lv_obj_add_event_cb(On_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}
