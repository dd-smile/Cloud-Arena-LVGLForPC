/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-04-25 16:02:15
 * @LastEditTime: 2023-04-25 19:50:29
 * @FilePath: /SmartBox_Power/lv_node/src/setting/all_setting/all_setting.c
 */
#include "ui_app.h"


/*------------------------------------------无法控制先不管----------------------------------------*/


// 创建屏保设置页面
void *CreateScreenSaveStlye(lv_obj_t *parent)
{
    lv_obj_t *scr = lv_c_create_mask_box(parent); // 创建遮罩
    lv_obj_t *popup_page = create_popup_page(scr, "Screensaver Settings", 0, 0);
    image_create(popup_page,&ImageFirst,-300,-100);
}

/**点击事件**/
static void add_Screensave_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        CreateScreenSaveStlye(lv_scr_act());
    }
}

void CreateScreensave(lv_obj_t *parent)
{
    lv_obj_t *ImageObj = CreateOBJclick(parent);
    image_create(parent, &Screensave, -100, -30);
    card_create_24_text(parent, "Screen saver", 2, -30);                        // 创建标题
    card_create_16_text(parent, "Brand new screen saver", -16, 35);  // 创建标题
    lv_obj_add_event_cb(ImageObj, add_Screensave_event_cb, LV_EVENT_ALL, NULL);

}