/*
 * @Description:设备控制通用代码
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-07-03 17:19:59
 * @LastEditTime: 2024-04-01 14:49:19
 * @FilePath: lv_node/src/device/device_common.c
 */
#include "ui_app.h"
#include "device_common.h"

// static lv_timer_t *timer = NULL;
static lv_style_t style_radio;
static lv_style_t style_radio_chk;
static uint32_t active_index_1 = 0;
lv_obj_t *act_cb;
const char *RadioText;
lv_obj_t *Input_label;
static lv_obj_t *WinpageLabel;

lv_obj_t *light_slider;  
lv_obj_t *lightSliderLabel; // 滑动条的文字

/**
 * 创建设备详情页中的巡检表
 * @param parent         指向父对象的指针
 * */
static void CreateBiaoge(lv_obj_t *parent)
{
    lv_obj_t *lvLogo = lv_img_create(parent);
    lv_img_set_src(lvLogo, &biaoge);
    lv_obj_align(lvLogo, LV_ALIGN_CENTER, 185, 100);

    card_create_12_text(lvLogo, "Inspection time", -105, -30);
    card_create_12_text(lvLogo, "Inspection status", 0, -30);
    card_create_12_text(lvLogo, "Result", 105, -30);

    create_12_Green_text(lvLogo, "2023/06/27", -105, -7);
    create_12_Green_text(lvLogo, "exception", 0, -7);
    create_12_Green_text(lvLogo, "solved", 105, -7);

    create_12_Red_text(lvLogo, "2023/06/26", -104, 20);
    create_12_Red_text(lvLogo, "exception", 1, 20);
    create_12_Red_text(lvLogo, "pending", 106, 20);
}

/**
 * 创建制造商logo
 * @param parent          指向父对象的指针
 * */
static void CreateLoge(lv_obj_t *parent)
{

    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(obj, 322, 139);
    lv_obj_set_pos(obj, 415, 40);
    lv_obj_set_style_bg_color(obj, lv_color_make(20, 26, 35), LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(obj, lv_color_make(49, 61, 85), LV_STATE_DEFAULT);

    lv_obj_t *lvLogo = lv_img_create(obj);
    lv_img_set_src(lvLogo, &lingxianLOGo);
    lv_obj_align(lvLogo, LV_ALIGN_CENTER, 0, 0);
}

/**
 * 创建功能按钮
 * @param obj             指向父对象的指针
 * @param text            文本，以'\0'结尾的字符串
 * @param x_ofs           x坐标偏移量
 * @param event_cb        要注册的事件       
 * @param device_type     设备类型    
 * @return                返回指向按钮的指针
 * */
static lv_obj_t *CreateFeatureButton(lv_obj_t *obj, const char *text, lv_coord_t x_ofs, lv_event_cb_t event_cb, u_int8_t device_type)
{

    lv_obj_t *btn = lv_btn_create(obj);
    lv_obj_set_size(btn, 85, 35);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(btn, 20, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, 80, LV_PART_MAIN);
    lv_obj_align(btn, LV_ALIGN_CENTER, x_ofs, 185);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, device_type);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    // 红色字体
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);

    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_center(label);
    return btn;
}

/**
 * 创建设备详情控制按钮
 * @param obj             指向父对象的指针
 * @param text            文本，以'\0'结尾的字符串
 * @param x_ofs           x坐标偏移量
 * @param y_ofs           y坐标偏移量
 * @param index            (传入设备号和按钮号，取出按钮号)按到第几个按钮
 * */
static void CreateControlsButton(lv_obj_t *obj, const char *text, lv_coord_t x_ofs, lv_coord_t y_ofs, index_t *index, lv_event_cb_t event_cb)
{

    lv_obj_t *btn = lv_obj_create(obj);

    lv_obj_set_size(btn, 90, 35);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(btn, 20, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, 80, LV_PART_MAIN);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, index);  //设备详情页控制按钮事件
    lv_obj_align(btn, LV_ALIGN_CENTER, x_ofs, y_ofs);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_center(label);
}


/**
 * 创建分割线
 * @param parent          指向父对象的指针
 * */
static void CreatrLine(lv_obj_t *parent)
{
    static lv_point_t line_points[] = {{0, 5}, {0, 409}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_color_hex(0x34415B));
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t *line1;
    line1 = lv_line_create(parent);
    lv_line_set_points(line1, line_points, 2); /*设置线的坐标*/
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_center(line1);
}

/**
 * 创建滚动窗口页面
 * @param obj             指向父对象的指针 
 * @param value           要设置的字体的指针。LVGL有许多预定义的字体，你也可以添加自定义字体。
 * @param text            文本,以'\0'结尾的字符串。
 * @param x_ofs           x坐标偏移量
 * @param y_ofs           y坐标偏移量
 * @param width           要设置的宽度
 * @param height          要设置的高度
 * @return                返回一个指向窗口的内容区域的指针
 * */
static lv_obj_t *CreateWinPage(lv_obj_t *obj, const lv_font_t *value, const char *text, lv_coord_t x_ofs, lv_coord_t y_ofs, uint16_t width, uint16_t height)
{

    lv_obj_t *win = lv_win_create(obj, 0);
    lv_obj_set_size(win, width, height);
    lv_obj_align(win, LV_ALIGN_BOTTOM_LEFT, x_ofs, y_ofs);
    lv_obj_t *cont = lv_win_get_content(win); /*可以在cont添加内容*/
    lv_obj_set_style_bg_color(cont, lv_color_make(26, 37, 57), LV_STATE_DEFAULT);
    lv_obj_set_style_width(cont, 0, LV_PART_SCROLLBAR);
    lv_obj_t *label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, value, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 0, LV_STATE_DEFAULT);
    lv_label_set_text(label, text);
    lv_obj_set_pos(label, -20, -24);

    return cont;
}

/**
 * 创建背景底图
 * @param mask           指向父对象的指针    
 * @return               返回指向背景底图的指针  
 * */
static lv_obj_t *CreateDeviceBgCard(lv_obj_t *mask)
{

    lv_obj_t *bg = lv_obj_create(mask); // 创建背景
    lv_obj_set_size(bg, 814, 468);
    lv_obj_align(bg, LV_ALIGN_CENTER, 40, 20);
    lv_obj_set_style_bg_color(bg, lv_color_make(26, 37, 57), LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(bg, lv_color_make(49, 61, 85), LV_STATE_DEFAULT);

    return bg;
}

/*创建输入框*/
static lv_obj_t *CreateInputBox(lv_obj_t *parent, lv_coord_t x_ofs, lv_coord_t y_ofs, uint16_t width, uint16_t height)
{

    lv_obj_t *box = lv_obj_create(parent);
    lv_obj_set_size(box, width, height);
    lv_obj_align(box, LV_ALIGN_TOP_RIGHT, x_ofs, y_ofs);
    lv_obj_set_style_bg_color(box, lv_color_make(0, 0, 0), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(box, 1, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(box, lv_color_make(49, 61, 85), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(box, 0, LV_PART_MAIN);
    lv_obj_clear_flag(box, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动条

    return box;
}

// 单选框选择点击事件
static void radio_event_handler(lv_event_t *e)
{
    uint32_t *active_id = lv_event_get_user_data(e);
    lv_obj_t *cont = lv_event_get_current_target(e);
    act_cb = lv_event_get_target(e);
    lv_obj_t *old_cb = lv_obj_get_child(cont, *active_id);

    /*Do nothing if the container was clicked*/
    if (act_cb == cont)
        return;

    lv_obj_clear_state(old_cb, LV_STATE_CHECKED); /*Uncheck the previous radio button*/
    lv_obj_add_state(act_cb, LV_STATE_CHECKED);   /*Uncheck the current radio button*/

    // 获取索引
    *active_id = lv_obj_get_index(act_cb);
    // 获取单选框的值
    RadioText = lv_checkbox_get_text(act_cb);
    printf("选择类型：%d ====================== %s\n", (int)active_index_1, RadioText);
}

// 创建选择框
static void CreateRadioButton(lv_obj_t *parent, const char *txt)
{
    lv_obj_t *obj = lv_checkbox_create(parent);
    lv_checkbox_set_text(obj, txt);
    lv_obj_set_style_text_font(obj, &fout_12_text, 0); // set font
    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_STATE_DEFAULT);

    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_style(obj, &style_radio, LV_PART_INDICATOR);
    lv_style_set_bg_opa(&style_radio, LV_OPA_TRANSP);
    lv_style_set_border_color(&style_radio, lv_color_hex(0xffffff));

    lv_obj_add_style(obj, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_style_set_bg_color(&style_radio_chk, lv_color_hex(0xffffff));
}

// 创建单选框
static lv_obj_t *CreateRadioBox(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{

    lv_style_init(&style_radio);                         // 初始化单选按钮的样式
    lv_style_set_radius(&style_radio, LV_RADIUS_CIRCLE); // 设置单选框圆形形状
    lv_style_init(&style_radio_chk);                     // 初始化选中状态
    lv_style_set_bg_img_src(&style_radio_chk, NULL);     // 不使用任何背景图

    lv_obj_t *cont1 = lv_obj_create(parent); // 创建一个标签
    lv_obj_set_flex_flow(cont1, LV_FLEX_FLOW_ROW);
    lv_obj_set_size(cont1, 300, 70);
    lv_obj_align(cont1, LV_ALIGN_TOP_MID, x, y); // 230, 249
    lv_obj_set_style_bg_opa(cont1, LV_OPA_0, 0);
    lv_obj_set_style_border_width(cont1, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(cont1, radio_event_handler, LV_EVENT_CLICKED, &active_index_1);

    // 创建单选框
    CreateRadioButton(cont1, "Solved");
    CreateRadioButton(cont1, "Pending");

    lv_obj_add_state(lv_obj_get_child(cont1, 0), LV_STATE_CHECKED);

    return cont1;
}

static void draw_part_event_cb(lv_event_t *e)
{ // 初始化表格样式
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e);
    if (dsc->part == LV_PART_ITEMS)
    {
        uint32_t row = dsc->id / lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);
        dsc->rect_dsc->bg_color = lv_color_hex(0x313d55);
        dsc->label_dsc->color = lv_color_hex(0xffffff);
        dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
        if (row != 0 && col == 2)
        {
            dsc->label_dsc->align = LV_TEXT_ALIGN_LEFT;
            dsc->label_dsc->ofs_x = 10;
        }
        const char *cell_value = lv_table_get_cell_value(obj, row, col);
        if (row == 0)
        {
            dsc->rect_dsc->bg_color = lv_color_hex(0x313d55);
            dsc->label_dsc->color = lv_color_hex(0x00bfff);
        }
        if (row % 2 == 1)
            dsc->rect_dsc->bg_color = lv_color_hex(0x1a2539);

        if (strcmp(cell_value, "exception") == 0)
        {
            dsc->label_dsc->color = lv_palette_main(LV_PALETTE_RED);
            const char *solution = lv_table_get_cell_value(obj, row, 2);
            const char *result = lv_table_get_cell_value(obj, row, 4);
            if (strcmp(solution, "repaired") == 0)
            {
                dsc->label_dsc->color = lv_palette_main(LV_PALETTE_GREEN);
            }
            else if (strcmp(solution, "arranging") == 0)
            {
                dsc->label_dsc->color = lv_palette_main(LV_PALETTE_RED);
            }
            if (strcmp(result, "solved") == 0)
            {
                dsc->label_dsc->color = lv_palette_main(LV_PALETTE_GREEN);
            }
            else if (strcmp(result, "pending") == 0)
            {
                dsc->label_dsc->color = lv_palette_main(LV_PALETTE_RED);
            }
        }
        const char *Celsolution = lv_table_get_cell_value(obj, row, col);
        if (strcmp(Celsolution, "repaired") == 0)
            dsc->label_dsc->color = lv_palette_main(LV_PALETTE_GREEN);

        if (strcmp(Celsolution, "arranging") == 0)
            dsc->label_dsc->color = lv_palette_main(LV_PALETTE_RED);

        const char *Celresult = lv_table_get_cell_value(obj, row, col);
        if (strcmp(Celresult, "solved") == 0)
            dsc->label_dsc->color = lv_palette_main(LV_PALETTE_GREEN);

        if (strcmp(Celresult, "pending") == 0)
            dsc->label_dsc->color = lv_palette_main(LV_PALETTE_RED);
    }
}

/**
 * 设置表格中单元格的内容(一列一列设置)
 * @param halert_table                 指向表格对象的指针 
 * @param id                           行的索引（从0开始）
 * @param InspectionTime               要设置的单元格文本:检验时间
 * @param InspectionStatus             要设置的单元格文本:检查状态
 * @param SolutionMeasure              要设置的单元格文本:解决方案措施
 * @param DeviceFaultSymptom           要设置的单元格文本:设备故障描述
 * @param Result　　　　　　　　　　　　　　要设置的单元格文本:结果
 * */
void lv_create_alarm(lv_obj_t *halert_table, uint8_t id, const char *InspectionTime, const char *InspectionStatus, const char *SolutionMeasure, const char *DeviceFaultSymptom, const char *Result)
{
    lv_table_set_cell_value(halert_table, id, 0, InspectionTime);
    lv_table_set_cell_value(halert_table, id, 1, InspectionStatus);
    lv_table_set_cell_value(halert_table, id, 2, SolutionMeasure);
    lv_table_set_cell_value(halert_table, id, 3, DeviceFaultSymptom);
    lv_table_set_cell_value(halert_table, id, 4, Result);
}

/**
 * 设置检查故障表参数内容
 * @param parent           指向父对象的指针
 * */
static lv_obj_t *ui_alert_obj(lv_obj_t *parent)
{ // 长1000宽480的参数设置页面
    lv_obj_t *halert_table = lv_table_create(parent);
    lv_obj_align(halert_table, LV_ALIGN_TOP_LEFT, 10, 50);
    lv_obj_set_style_outline_width(halert_table, 1, LV_PART_ITEMS);
    lv_obj_set_style_border_width(halert_table, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(halert_table, lv_color_hex(0x313d55), LV_PART_MAIN);
    lv_obj_set_style_text_font(halert_table, &PuHuiTi_Regular_16, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(halert_table, 2, LV_PART_ITEMS);

    lv_obj_add_event_cb(halert_table, draw_part_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

    lv_table_set_col_width(halert_table, 0, 140);
    lv_table_set_col_width(halert_table, 1, 100);
    lv_table_set_col_width(halert_table, 2, 140);
    lv_table_set_col_width(halert_table, 3, 350);
    lv_table_set_col_width(halert_table, 4, 100);

    lv_table_set_cell_value(halert_table, 0, 0, "检验时间");
    lv_table_set_cell_value(halert_table, 0, 1, "检查状态");
    lv_table_set_cell_value(halert_table, 0, 2, "解决方案措施");
    lv_table_set_cell_value(halert_table, 0, 3, "设备故障描述");
    lv_table_set_cell_value(halert_table, 0, 4, "结果");

    lv_create_alarm(halert_table, 1, "2023/06/26", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 2, "2023/06/20", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 3, "2023/06/19", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 4, "2023/06/12", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 5, "2023/06/05", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 6, "2023/06/01", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 7, "2023/05/29", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 8, "2023/05/18", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 9, "2023/05/15", "异常", "修理", "点击操作异常，无旋转，无噪音", "解决了");
    lv_create_alarm(halert_table, 10, "2023/05/13", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 11, "2023/05/05", "正常", " ", " ", " ");
    lv_create_alarm(halert_table, 12, "2023/04/12", "正常", " ", " ", " ");

    // lv_create_alarm();
}


/**
 * 创建检查故障表
 * @param parent           指向父对象的指针
 * @return                 返回指向背景底图的指针
 * */
lv_obj_t *CreateInspectTable(lv_obj_t *parent)
{
    lv_obj_t *mask = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *Tablebg = CreateDeviceBgCard(mask);
    lv_obj_set_size(Tablebg, 900, 500);
    card_create_16_text(Tablebg, "设备名称: ", -380, -220);       // 创建设备名称
    card_create_16_text(Tablebg, pPageData->deviceName, -220, -220); // 创建设备名称

    ui_alert_obj(Tablebg);

    return Tablebg;
}

/**
 * 设备详情页的基础ＵＩ
 * @param parent         指向一个对象的指针，它将是新对象的父类
 * @param data           要创建设备的基本数据
*/
static void Device_details_page_basicUi(lv_obj_t *parent, const DevicePageData *data)
{
    CreatrLine(parent);                                      // 创建分割线
    card_create_16_text(parent, UI_MLANG_STR(DEVICENAME), -320, 30);  
    if(get_cur_language_type() == zhT)
    {
        card_create_16_text(parent, data->deviceName, -240, 30); // 创建设备名称
        card_create_12_text(parent, UI_MLANG_STR(SUPPLIER_SERVICE_TELEPHONE), 95, 25);   //创建供应商服务电话
    }
    else
    {
        card_create_16_text(parent, data->deviceName, -160, 30); // 创建设备名称
        card_create_12_text(parent, UI_MLANG_STR(SUPPLIER_SERVICE_TELEPHONE), 75, 25);   //创建供应商服务电话
    }
    card_create_16_text(parent, UI_MLANG_STR(MANUFACTURER_PROFILE), 100, -195);
    card_create_12_text(parent, UI_MLANG_STR(PURCHASING_PERIOD), 75, -5);
    card_create_12_text(parent, data->date, 140, -5);
    card_create_12_text(parent, UI_MLANG_STR(REPAIR_CYCLE), 260, -5);
    card_create_12_text(parent, data->period, 330, -5);

    card_create_12_text(parent, data->phoneNumber, 200, 25);
    image_create(parent, data->image, -200, -100);                        // 创建图片
    CreateWinPage(parent, &fout_16_text, data->intro, 31, -100, 300, 59); // 创建产品简介

    CreateLoge(parent); // 制造商Logo
    CreateBiaoge(parent); // 表格
} 

/*------------------------------------------创建按钮点击事件-----------------------------------------*/

/**
 * 添加描述故障点击事件
 * @param e               指向事件描述符的指针
 * */
static void AddDescriptionEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if ( code == LV_EVENT_CLICKED)
    {
        char *inputVal = lv_textarea_get_text(Textarea_Pinyin);
        printf("描述故障： %s\n", inputVal);

        lv_label_set_text(WinpageLabel, inputVal);  //设置标签的值
        lv_obj_del(obj->parent);
    }
}

/**
 * 添加重命名按钮的点击事件
 * @param e               指向事件描述符的指针
 * */
static void AddRenameEventBtn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if ( code == LV_EVENT_CLICKED)
    {
        char *inputVal = lv_textarea_get_text(Textarea_Pinyin);
        printf("添加文本 %s\n", inputVal);
        lv_obj_del(obj->parent);
    }
}

/**
 * 创建重命名按钮的点击事件
 * @param e               指向事件描述符的指针
 * */
static void RenameCreateButton(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_keypage_create(lv_scr_act(), AddRenameEventBtn);
    }
}

/**
 * 创建检修正常按钮的点击事件
 * @param e               指向事件描述符的指针
 * */
static void MaintenanceCreateButton(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        //事件要进行更换,点击确认检修正常，巡检记录表自动生成一条当天的记录，巡检情况为正常
        create_popup(lv_scr_act(), pPageData->PopupTitlie, UI_MLANG_STR(CONFIRM), UI_MLANG_STR(CANCEL), pPageData->PopupContent, lv_back_Mask, NULL);
    }
}

/**
 * 灯光亮度设置进度条事件
 * @param e               指向事件描述符的指针
*/
static void lightslider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    // uint8_t num = lv_event_get_user_data(e);

    lv_label_set_text_fmt(lightSliderLabel, "%d", (int)lv_slider_get_value(slider));
    printf("灯光的亮度 = %x\n", (int)lv_slider_get_value(slider));
    // switch (num)
    // {
    //     case 1:
    //         Setdimming_vsu(0x01, 0x01, (uint8_t)lv_slider_get_value(slider));   //发送调光指令
    //         break;

    //     case 2: 
    //         Setdimming_vsu(0x01, 0x02, (uint8_t)lv_slider_get_value(slider));
    //         break;

    //     case 3:
    //         Setdimming_vsu(0x01, 0x03, (uint8_t)lv_slider_get_value(slider));
    //         break;

    //     case 4:
    //         Setdimming_vsu(0x01, 0x04, (uint8_t)lv_slider_get_value(slider));
    //         break;
        
    //     default:
    //         break;
    // }
    Setdimming_vsu(0x01, 0x01, (uint8_t)lv_slider_get_value(slider));   //发送调光指令
    Setdimming_vsu(0x01, 0x02, (uint8_t)lv_slider_get_value(slider));
    Setdimming_vsu(0x01, 0x03, (uint8_t)lv_slider_get_value(slider));
    Setdimming_vsu(0x01, 0x04, (uint8_t)lv_slider_get_value(slider));

}

/**
 * 创建故障申报按钮的点击事件
 * @param e               指向事件描述符的指针
 * */
static void ReportCreateButton(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t *user_data = lv_event_get_user_data(e);


    if (code == LV_EVENT_CLICKED)
    {
        switch ((int)user_data)
        {
        case 0:
            CreateBreakdownPageBg(lv_scr_act(), pPageData);
            break;

        case 1:
            CreateBreakdownPageBg(lv_scr_act(), bs_data);
            break;

        case 2:
            CreateBreakdownPageBg(lv_scr_act(), pc_data);
            break;

        case 3:
            CreateBreakdownPageBg(lv_scr_act(), wa_data);
            break;

        case 4:
            CreateBreakdownPageBg(lv_scr_act(), fs_data);
            break;

        case 5:
            CreateBreakdownPageBg(lv_scr_act(), cs_data);
            break;

        case 6:
            CreateBreakdownPageBg(lv_scr_act(), rs_data);
            break;
        
        case 7:
            CreateBreakdownPageBg(lv_scr_act(), l_data);
            break;

        case 8:
            
            break;

        default:
            break;

        }
    }
}


/**
 * 创建检查记录表按钮的点击事件
 * @param e               指向事件描述符的指针
 * */
static void InspectCreateButton(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        CreateInspectTable(lv_scr_act());
        printf("Create\n");
    }
}

/**
 * 创建描述故障输入框的点击事件
 * @param e               指向事件描述符的指针
 * */
static void DescriptionInputHandler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if ( code == LV_EVENT_CLICKED)
    {
        lv_keypage_create(lv_scr_act(), AddDescriptionEvent);
    }
}

/**
 * 创建描述故障输入框
 * @param parent           指向父对象的指针
 * */
static lv_obj_t *CreateDescriptionInput(lv_obj_t *parent)
{
    card_create_16_text(parent, "故障现象的描述: ", 152, -190);  // 设备故障现象描述
    lv_obj_t *WinPage = CreateWinPage(parent, &fout_16_text, pPageData->Description, 408, -300, 300, 80); // 设备故障现象描述
    lv_obj_set_style_bg_color(WinPage, lv_color_make(0, 0, 0), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(WinPage, 1, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(WinPage, lv_color_make(49, 61, 85), LV_STATE_DEFAULT);
    lv_obj_add_event_cb(WinPage, DescriptionInputHandler, LV_EVENT_ALL, NULL);
}

/**
 * 创建设备故障详情页
 * @param parent           指向父对象的指针
 * @return                 返回指向设备故障详情页的指针
 * */
lv_obj_t *CreateBreakdownPageBg(lv_obj_t *parent, const DevicePageData *data)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);                               // 创建背景
    CreatrLine(bg);                                                        // 创建分割线
    card_create_16_text(bg, UI_MLANG_STR(DEVICENAME), -300, 30);                    // 创建设备名称
    card_create_16_text(bg, data->deviceName, -140, 30);              // 创建设备名称
    image_create(bg, data->image, -200, -100);                        // 创建图片
    CreateWinPage(bg, &fout_16_text, data->intro, 31, -100, 300, 59); // 创建产品简介

    card_create_12_text(bg, UI_MLANG_STR(SUPPLIER_SERVICE_TELEPHONE), 88, -50); // 制造商服务电话
    card_create_12_text(bg, data->ManufacturingTel, 180, -50);
    card_create_12_text(bg, UI_MLANG_STR(AFTER_SALES_HOTLINE), 87, 5);
    card_create_12_text(bg, data->SalehAfterotline, 180, 5);

    card_create_12_text(bg, UI_MLANG_STR(SOLUTION), 88, 50);

    lv_obj_t *SolveInputBox = CreateInputBox(bg, -43, 244, 190, 35);
    lv_obj_set_style_text_font(SolveInputBox, &lv_font_montserrat_12, 0); // set font
    lv_obj_set_style_text_color(SolveInputBox, lv_color_hex(0xffffff), LV_STATE_DEFAULT);


    // Input_label = lv_label_create(SolveInputBox); // 创建标签
    // lv_label_set_text(Input_label, "解决方案");
    // lv_obj_align(Input_label, LV_ALIGN_LEFT_MID, -10, 0);

    card_create_12_text(bg, "结果: ", 55, 105);
    CreateRadioBox(bg, 230, 287);  //结果选择框

    CreateDescriptionInput(bg);  //描述设备故障输入框

    lv_obj_t *InspectButton = CreateFeatureButton(bg, "检查记录表", 50, InspectCreateButton, NULL); // 创建检查记录表按钮
    lv_obj_set_size(InspectButton, 320, 39);
    lv_obj_align(InspectButton, LV_ALIGN_DEFAULT, 22, 350);

    lv_obj_t *ConfirmBtn = btn_create_text(bg, false, "OK", 480, 353);     // 创建按钮
    lv_obj_add_event_cb(ConfirmBtn, lv_back_Mask, LV_EVENT_CLICKED, NULL); // 添加返回事件
    lv_obj_t *BackBtn = btn_create_text(bg, false, "Cancel", 620, 353);    // 创建按钮
    lv_obj_add_event_cb(BackBtn, lv_back_Mask, LV_EVENT_CLICKED, NULL);    // 添加返回事件

    return bg;
}


/**
 * 创建活动看台设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
 * */
lv_obj_t *CreateDevicePageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    //在确定设备类型和设备号后，设置按钮号（活动看台，拥有７个按钮）
    index_t *index1 = add_index_t(device_num, 0);
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    // index_t *index3 = add_index_t(device_num, 3);
    // index_t *index4 = add_index_t(device_num, 4);
    // index_t *index5 = add_index_t(device_num, 5);
    // index_t *index6 = add_index_t(device_num, 6);

    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, telescoopic_Controls_event_cb);                // 创建一键打开按钮
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, telescoopic_Controls_event_cb);         // 创建急停按钮
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, telescoopic_Controls_event_cb);               // 创建一键收合按钮

    // CreateControlsButton(bg, data->StandOpenBtnText, -350, 188, index3);     //看台展开
    // CreateControlsButton(bg, data->StandClosureBtnText, -250, 188, index4);  //看台收合
    // CreateControlsButton(bg, data->SeatRiseBtnText, -150, 188, index5);      //座椅展开
    // CreateControlsButton(bg, data->SeatTipBtnText, -50, 188, index6);        //座椅收合

    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 0); // 创建故障申报按钮  
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);            // 创建改名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL); // 创建检修正常按钮 
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                   // 创建返回按钮

    return bg;
}

/**
 * 创建悬空球架设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
 * */
lv_obj_t *CreatebasketballPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    //在确定设备类型和设备号后，设置按钮号（活动看台，拥有７个按钮）
    index_t *index1 = add_index_t(device_num, 0);   //传进来的device_num是设备号
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, basketball_Controls_event_cb);                // 创建一键打开按钮
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, basketball_Controls_event_cb);         // 创建急停按钮
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, basketball_Controls_event_cb);               // 创建一键收合按钮

    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 1); // 创建故障申报按钮  
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);            // 创建改名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL); // 创建检修正常按钮 
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                   // 创建返回按钮

    return bg;

}


/**
 * 创建分隔幕设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
 * */
lv_obj_t *CreatePartitionPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    index_t *index1 = add_index_t(device_num, 0);
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, partition_Controls_event_cb);        // 一键打开按钮
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, partition_Controls_event_cb); // 创建急停按钮
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, partition_Controls_event_cb);       // 一键收合按钮
   
    
    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 2);       // 故障申报按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);          // 重命名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL);  // 检修正常按钮   
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                 // 返回按钮

    return bg;
}


/**
 * 创建壁挂球架设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
*/
lv_obj_t *CreateWallhangingPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    index_t *index1 = add_index_t(device_num, 0);
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, wallhanging_Controls_event_cb);        // 一键打开按钮
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, wallhanging_Controls_event_cb); // 创建急停按钮
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, wallhanging_Controls_event_cb);       // 一键收合按钮
   
    
    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 3);       // 故障申报按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);          // 重命名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL);  // 检修正常按钮   
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                 // 返回按钮

    return bg;
}

/**
 * 创建地面翻折座椅设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
*/
lv_obj_t *CreateFoldingPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    index_t *index1 = add_index_t(device_num, 0);
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, folding_Controls_event_cb);        // 一键打开按钮
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, folding_Controls_event_cb); // 创建急停按钮
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, folding_Controls_event_cb);       // 一键收合按钮
   
    
    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 4);       // 故障申报按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);          // 重命名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL);  // 检修正常按钮   
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                 // 返回按钮

    return bg;
}

/**
 * 创建伸缩舞台设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
*/
lv_obj_t *CreateContractionPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    index_t *index1 = add_index_t(device_num, 0);
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    index_t *index4 = add_index_t(device_num, 3);
    index_t *index5 = add_index_t(device_num, 4);
    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, telescopic_Controls_event_cb);       
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, telescopic_Controls_event_cb); 
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, telescopic_Controls_event_cb);       
    CreateControlsButton(bg, data->StandOpenBtnText, -320, 188, index4, telescopic_Controls_event_cb);
    CreateControlsButton(bg, data->StandClosureBtnText, -80, 188, index5, telescopic_Controls_event_cb);
    
    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 5);       // 故障申报按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);          // 重命名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL);  // 检修正常按钮   
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                 // 返回按钮

    return bg;
}

/**
 * 创建升降旋转舞台设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
*/
lv_obj_t *CreateRevolvingPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    index_t *index1 = add_index_t(device_num, 0);
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    index_t *index4 = add_index_t(device_num, 3);
    index_t *index5 = add_index_t(device_num, 4);
    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, stage_Controls_event_cb);       
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, stage_Controls_event_cb); 
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, stage_Controls_event_cb);       
    CreateControlsButton(bg, data->StandOpenBtnText, -320, 188, index4, stage_Controls_event_cb);
    CreateControlsButton(bg, data->StandClosureBtnText, -80, 188, index5, stage_Controls_event_cb);
    
    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 6);       // 故障申报按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);          // 重命名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL);  // 检修正常按钮   
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                 // 返回按钮

    return bg;
}

/**
 * 创建灯光设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
*/
lv_obj_t *CreateLightsPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    // index_t *index1 = add_index_t(device_num, 0);
    // index_t *index2 = add_index_t(device_num, 1);
    // CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, lights_Controls_event_cb);  // 一键打开按钮
    // CreateControlsButton(bg, data->collapseBtnText, -80, 142, index2, lights_Controls_event_cb); // 一键收合按钮

    card_create_20_text(bg, UI_MLANG_STR(BRIGHTNESS_SETTING), -330, 112);
    image_create(bg, &light, -370, 162);                                                                       // 创建图标
    light_slider = CreateSlider(bg, -190, 162, 5, 100, lv_color_hex(0x00d1fe), 100, 300, 22, false); // 创建进度条
    lightSliderLabel = card_create_20_text(bg, "100", -320, 162);
    // lv_obj_add_event_cb(light_slider, lightslider_event_cb, LV_EVENT_VALUE_CHANGED, device_num);
    lv_obj_add_event_cb(light_slider, lightslider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

   
    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 7);       // 故障申报按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);          // 重命名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL);  // 检修正常按钮   
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                 // 返回按钮

    return bg;
}


/**
 * 创建舞台大幕设备详情页
 * @param data           要创建设备的基本数据
 * @param device_num     设备号
 * @return               返回指向已创建的设备详情页面的指针
*/
lv_obj_t *CreatecurtainPageBg(const DevicePageData *data, uint8_t device_num)
{
    lv_obj_t *mask = lv_c_create_mask_box(lv_scr_act()); // 创建遮罩层

    lv_obj_t *bg = CreateDeviceBgCard(mask);             // 创建背景
    Device_details_page_basicUi(bg, data);

    index_t *index1 = add_index_t(device_num, 0);
    index_t *index2 = add_index_t(device_num, 1);
    index_t *index3 = add_index_t(device_num, 2);
    CreateControlsButton(bg, data->expandBtnText, -320, 142, index1, curtain_Controls_event_cb);       
    CreateControlsButton(bg, data->emergencyStopBtnText, -200, 142, index2, curtain_Controls_event_cb); 
    CreateControlsButton(bg, data->collapseBtnText, -80, 142, index3, curtain_Controls_event_cb);       

    
    CreateFeatureButton(bg, UI_MLANG_STR(REPORT), 245, ReportCreateButton, 6);       // 故障申报按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RENAME), 50, RenameCreateButton, NULL);          // 重命名按钮
    CreateFeatureButton(bg, UI_MLANG_STR(RECONDITIONED), 145, MaintenanceCreateButton, NULL);  // 检修正常按钮   
    CreateFeatureButton(bg, UI_MLANG_STR(BACK), 340, lv_back_Mask, NULL);                 // 返回按钮

    return bg;
}