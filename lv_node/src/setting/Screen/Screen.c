#include "ui_app.h"


/*------------------------------------------无法控制先不管----------------------------------------*/

lv_Brightting_data_t setting; //屏幕设置结构体
lv_obj_t *In_slider;   //暂时没有用到
lv_obj_t *On_slider;   //暂时没有用到
lv_obj_t *In_slider;   //暂时没有用到

uint32_t btnmatrix_id;   // 屏保按钮id
lv_obj_t *lv_scr_obj;    // 屏保对象
lv_obj_t *lv_Scrsw_obj;  // 开关对象
lv_obj_t *slider_label;  // 滑动条的文字（外面）
lv_obj_t *InSliderLabel; // 滑动条的文字（里面）

/**开关点击事件**/
static void event_handler(lv_event_t *e)
{
    lv_Scrsw_obj = lv_event_get_target(e);
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        bool switch_state = lv_obj_has_state(lv_Scrsw_obj, LV_STATE_CHECKED);
        printf("111===state:%d\n", switch_state);
    }
}

static lv_obj_t *create_switch(lv_obj_t *parent)
{
    lv_obj_t *swich = lv_switch_create(parent); // 创建开关
    lv_obj_align(swich, LV_ALIGN_DEFAULT, 110, 85);
    lv_obj_add_event_cb(swich, event_handler, LV_EVENT_ALL, swich);

    return swich;
}

// 弹出页面返回上一级按钮
lv_obj_t *create_page_back(lv_obj_t *obj)
{
    lv_obj_t *lv_obj = lv_obj_create(obj);
    lv_obj_set_size(lv_obj, 90, 50);
    lv_obj_add_flag(lv_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_LEFT, 0, -13);
    lv_obj_set_style_bg_opa(lv_obj, 0, LV_STATE_DEFAULT); // 设置背景透明
    lv_obj_set_style_border_width(lv_obj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(lv_obj, LV_OBJ_FLAG_SCROLLABLE);                 // 禁用滚动条
    lv_obj_add_event_cb(lv_obj, lv_back_Mask, LV_EVENT_CLICKED, time); // 添加返回事件
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
}

// 屏保按钮点击事件
static void scr_event_cb(lv_event_t *e)
{
    lv_scr_obj = lv_event_get_target(e);                      // 获取事件对象
    btnmatrix_id = lv_btnmatrix_get_selected_btn(lv_scr_obj); // 获取选中的按钮id
    
    printf("btnmatrix_id:%d\n", btnmatrix_id);
}

// 创建屏保按钮
lv_obj_t *create_screen_btn(lv_obj_t *parent)
{
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 50);
    static const char *map[] = {"1m", "10m", "20m", "30m", "1h", ""};
    lv_obj_t *btnm = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(btnm, map);
    lv_obj_add_style(btnm, &style_btn, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(btnm, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(btnm, 0, 0);
    lv_obj_add_event_cb(btnm, scr_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_size(btnm, 980, 100);
    lv_btnmatrix_set_btn_ctrl_all(btnm, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(btnm, true);
    lv_obj_center(btnm);
    return btnm;
}

// 点击保存事件
static void save_btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) // 如果是点击事件
    {
        printf("点击保存按钮\n");
        setting.switch_state = lv_obj_has_state(lv_Scrsw_obj, LV_STATE_CHECKED);
        printf("开关状态 = %s\n", setting.switch_state ? "开启" : "关闭");

        setting.ScreenBtnId = btnmatrix_id; // 获取选中的按钮id
        printf("选中的按钮id = %d\n", setting.ScreenBtnId);

        setting.brightness = lv_slider_get_value(In_slider);
        sprintf(setting.In_screen, "%d", setting.brightness);
        sprintf(setting.On_screen, "%d", setting.brightness);

        lv_slider_set_value(On_slider, setting.brightness, LV_ANIM_ON);
        lv_label_set_text(InSliderLabel, setting.In_screen);
        lv_label_set_text(slider_label, setting.On_screen);
        printf("亮度 = %s\n", setting.In_screen);
        printf("亮度 = %d\n", setting.brightness);
    }
}

// 创建设备设置页面
void *create_allset_stlye(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_popup_page(scr, "Screen Settings", 0, 0);

    card_create_20_text(popup_page, "开关", -450, -180);
    lv_obj_t *lv_switch = create_switch(popup_page); // 开关
    if (setting.switch_state == 1)
    {
        lv_obj_add_state(lv_switch, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(lv_switch, LV_STATE_CHECKED);
    }

    card_create_20_text(popup_page, "屏幕亮度设置", -410, -100);
    image_create(popup_page, &light, -450, -30);                                                                       // 创建图标
    In_slider = CreateSlider(popup_page, 50, -30, 0, 100, lv_color_hex(0x00d1fe), setting.brightness, 814, 44, false); // 创建进度条
    InSliderLabel = card_create_20_text(popup_page, setting.On_screen, -400, -30);
    lv_obj_add_event_cb(In_slider, Inslider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    card_create_20_text(popup_page, "自动进入屏幕保护", -395, 65);
    lv_obj_t *lv_screen = create_screen_btn(popup_page); // 屏保按钮
    lv_obj_align(lv_screen, LV_ALIGN_CENTER, -5, 145);
    lv_btnmatrix_set_btn_ctrl(lv_screen, setting.ScreenBtnId, LV_BTNMATRIX_CTRL_CHECKED);

    lv_obj_t *ScreenSaveBtn = btn_create_text(popup_page, false, "保存", 450, 500);
    lv_obj_set_style_bg_color(ScreenSaveBtn, lv_color_make(24, 57, 154), LV_PART_MAIN);
    lv_obj_set_size(ScreenSaveBtn, 120, 35);
    lv_obj_add_event_cb(ScreenSaveBtn, save_btn_event_cb, LV_EVENT_CLICKED, NULL);

    return NULL;
}

/*------------------------------点击事件----------------------------------*/

/**点击屏保事件**/
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
    card_create_24_text(parent, "Screen settings", 20, -30); // 创建标题
    lv_obj_add_event_cb(ImageObj, add_Screen_event_cb, LV_EVENT_ALL, NULL);

    setting.brightness = 63; // 什么都没有设置的时候默认值为63
    sprintf(setting.On_screen, "%d", setting.brightness);
    On_slider = CreateSlider(parent, 17, 35, 0, 100, lv_color_hex(0xffffff), setting.brightness, 200, 6, false); // 创建进度条
    slider_label = card_create_16_text(parent, setting.On_screen, -100, 35);                             // 创建标题
    lv_obj_add_event_cb(On_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}
