
#ifndef _LV_SCREEN_SETTING_H
#define _LV_SCREEN_SETTING_H

typedef struct
{
    bool switch_state;
    uint8_t brightness; //滑动条的值（外面）

    char On_screen[8];  // 外面的亮度
    char In_screen[8];  // 外面的亮度

    uint32_t ScreenBtnId; //屏保按钮id

} lv_Brightting_data_t;
 
extern  lv_Brightting_data_t setting;

void CreateScreen(lv_obj_t *parent);
void add_Screen_event_cb(lv_event_t *e);
lv_obj_t *create_page_back(lv_obj_t *obj);
void *create_allset_stlye(lv_obj_t *parent);
lv_obj_t *create_screen_btn(lv_obj_t *parent);


#endif
