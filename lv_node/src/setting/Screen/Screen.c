/*
 * @Description:屏幕设置
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-03-09 11:28:42
 * @LastEditTime: 2023-12-25 10:34:31
 * @FilePath: lv_node/src/setting/Screen/Screen.c
 */
#include "ui_app.h"

lv_setting_data_t setting; //屏幕设置结构体   用于退出页面时候的保存
lv_setting_data_t setting_buf; //屏幕设置结构体   用于处于页面详情时候的保存
// lv_obj_t *In_slider;   
lv_obj_t *On_slider;   
lv_obj_t *In_slider;   

uint32_t btnmatrix_id;   // 屏保按钮id
lv_obj_t *lv_scr_obj;    // 屏保对象
lv_obj_t *lv_Scrsw_obj;  // 开关对象
lv_obj_t *slider_label;  // 滑动条的文字（外面）
lv_obj_t *InSliderLabel; // 滑动条的文字（里面）

static lv_obj_t *s_pass_setting_label;  //密码设置提示框


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
            create_popup(lv_scr_act(), "", UI_MLANG_STR(CONFIRM), UI_MLANG_STR(CANCEL), UI_MLANG_STR(WHETHER_TO_SAVE_SETTINGS), save_setting_event, NULL);
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
    printf("外面的亮度 = %s\n", setting.On_screen);
    // char cmd[128];
    // sprintf(cmd, "echo %d > /sys/class/pwm/pwmchip0/pwm0/duty_cycle", (int)lv_slider_get_value(slider));
    // system(cmd);
    // saveSettingData(&setting, SCREEN_SETTING_JSON);
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
    printf("里面的亮度 = %s\n", setting.In_screen);
    // char cmd[128];
    // sprintf(cmd, "echo %d > /sys/class/pwm/pwmchip0/pwm0/duty_cycle", (int)lv_slider_get_value(slider));
    // system(cmd);
    // saveSettingData(&setting, SCREEN_SETTING_JSON);
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
 *  判断传入字符串是否全数字
 *  @param                char *str     字符串
 *  @retval               1: 全数字符串，0:非数全字符串     
 */
static int isdigitstr(char *str)
{
    return (strspn(str, "0123456789")==strlen(str));
}

/**
 * 设置密码的点击事件
 * @param e               指向事件描述符的指针
 * */
static void AddPassEventBtn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if ( code == LV_EVENT_CLICKED)
    {
        char *keyUpdates = lv_textarea_get_text(Textarea_Pinyin);
        printf("修改密码文本 %s\n", keyUpdates);
        if (isdigitstr(keyUpdates) == 1 && strlen(keyUpdates) == 6)
        {
            int len = password_write(keyUpdates);
            printf("设置成功 %s,长度为 %d\n", keyUpdates, len);
            lv_label_set_text(s_pass_setting_label, "设置成功");
        }
        else
        {
            lv_label_set_text(s_pass_setting_label, "设置失败,请输入6位数字");
        }
        lv_obj_del(obj->parent);
    }
}

/**
 * 点击屏保密码设置事件
 * @param e         指向事件描述符的指针
*/
static void lv_screen_pass_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_keypage_create(lv_scr_act(), AddPassEventBtn);
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
    static const char *map[] = {"1minute", "10minute", "20minute", "30minute", "1hour", ""};
    lv_obj_t *btnm = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(btnm, map);
    lv_obj_add_style(btnm, &style_btn, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(btnm, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(btnm, 0, 0);
    lv_obj_set_style_text_font(btnm, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);

    lv_obj_add_event_cb(btnm, scr_event_cb, LV_EVENT_ALL, NULL);
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
    lv_obj_t *popup_page = create_popup_page(scr, UI_MLANG_STR(SCREEN_SETUP), 0, 0);

    card_create_20_text(popup_page, UI_MLANG_STR(BRIGHTNESS_SETTINGS), -410, -180);
    image_create(popup_page, &light, -450, -120);                                                                       // 创建图标
    In_slider = CreateSlider(popup_page, 50, -120, 5, 100, lv_color_hex(0x00d1fe), setting.brightness, 814, 44, false); // 创建进度条
    InSliderLabel = card_create_20_text(popup_page, setting.On_screen, -400, -120);
    lv_obj_add_event_cb(In_slider, Inslider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    card_create_20_text(popup_page, UI_MLANG_STR(ENTER_SCREENSAVER), -395, -15);
    lv_obj_t *lv_screen = create_screen_btn(popup_page); // 屏保按钮
    lv_obj_align(lv_screen, LV_ALIGN_CENTER, -5, 65);
    lv_btnmatrix_set_btn_ctrl(lv_screen, setting.Btnmid, LV_BTNMATRIX_CTRL_CHECKED);

    setting_buf.brightness       = setting.brightness;
    setting_buf.Btnmid           = setting.Btnmid;
    setting_buf.screen_save_time = setting.screen_save_time;
    setting_buf.ScreenSaveid     = setting.ScreenSaveid;

    card_create_20_text(popup_page, UI_MLANG_STR(SCREEN_PASSWORD_SETTINGS), -410, 130);
    lv_obj_t *lv_screen_pass = btn_create_text(popup_page, false, UI_MLANG_STR(PASSWORD_SETTING), 450, 450); 
    lv_obj_set_style_bg_color(lv_screen_pass, lv_color_make(0, 209, 254), LV_PART_MAIN);
    lv_obj_set_size(lv_screen_pass, 120, 35);
    lv_obj_add_event_cb(lv_screen_pass, lv_screen_pass_event_cb, LV_EVENT_CLICKED, NULL);

    s_pass_setting_label = lv_label_create(popup_page);
    static lv_style_t pass_setting_label_style;
    lv_style_reset(&pass_setting_label_style);
    lv_style_init(&pass_setting_label_style);
    lv_style_set_radius(&pass_setting_label_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&pass_setting_label_style, 0); //设置边框宽度
    lv_style_set_text_color(&pass_setting_label_style , lv_palette_main(LV_PALETTE_RED));  // 字体颜色设置为红色
    lv_label_set_long_mode(s_pass_setting_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 长文本循环滚动
    lv_obj_add_style(s_pass_setting_label, &pass_setting_label_style, 0); // 给btn_label添加样式
    lv_obj_set_style_text_font(s_pass_setting_label, &PuHuiTi_Regular_20, LV_PART_MAIN);
    lv_obj_align(s_pass_setting_label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(s_pass_setting_label, ""); // 设置文本内容



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
    card_create_24_text(parent, UI_MLANG_STR(SCREEN_SETUP), 20, -30); // 创建标题
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
