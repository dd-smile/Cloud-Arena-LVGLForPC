/*** 
 * @Description: 锁屏界面
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-06-16 09:55:17
 * @LastEditTime: 2023-06-16 09:58:06
 * @FilePath: lv_node/src/common/lock.c
 */
#include "ui_app.h"

bool password_lock_open = false;  //密码是否输入正确
bool password_mode_lock = false; //模式转换密码是否输入正确
lv_obj_t *pwd_main_cont = NULL; // 密码输入界面的容器
static lv_obj_t *hint_label = NULL; // 密码提示框
static lv_obj_t *pwd_text_area = NULL; // 密码输入框

lv_style_t style_indic;
lv_obj_t * mask; 
lv_obj_t * anaim_label;

char password_input[6] = "123456";
 
static const char * keyboard_map[] =
{
	"1","2", "3","\n",
	"4", "5", "6", "\n",
	"7", "8", "9" ,"\n",
	".","0",LV_SYMBOL_BACKSPACE,""
};
 
 
static const lv_btnmatrix_ctrl_t keyboard_ctrl[] =
{
	LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
	LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
	LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
	LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT, LV_BTNMATRIX_CTRL_NO_REPEAT,
};

// 模式确认按键的事件回调 
static void confirm_mode_event_callback(lv_event_t* event);

// 模式取消按键的事件回调
static void cancel_mode_event_callback(lv_event_t* event);

// 确认按键的事件回调
static void confirm_btn_event_callback(lv_event_t* event);
 
// 取消按键的事件回调
static void cancel_btn_event_callback(lv_event_t* event);

// 
static void lv_bar_change(void);


void password_read()
{
    int len = -1;

    int fd = open("lv_node/src/common/pwd_txt.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("open txt fail\n");
    }

    len = read(fd, password_input, 6);

    printf("密码文本 %s\n", password_input);

    close(fd);
    
}

int password_write(char *password_upd)
{

    int len = -1;

    int fd = open("lv_node/src/common/pwd_txt.txt", O_RDWR);
    if (fd < 0)
    {
        printf("open txt fail\n");
    }

    len = write(fd, password_upd, strlen(password_upd)); 

    close(fd);

    return len;

}
 
 
// 显示键盘输入界面
void lv_gui_password_keyboard_display(void)
{
    // 当前页面的画布容器
    pwd_main_cont = lv_obj_create(lv_scr_act()); // 基于屏幕创建了一个容器
	if (pwd_main_cont == NULL)
	{
		printf("[%s:%d] create pwd_main_cont failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 画布样式
	static lv_style_t main_cont_style;
    lv_style_reset(&main_cont_style);
	lv_style_init(&main_cont_style); // 初始化样式
    lv_style_set_radius(&main_cont_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&main_cont_style, 0); // 设置边框宽度
	lv_style_set_bg_opa(&main_cont_style, LV_OPA_50); // 设置样式背景的透明度，不透明
    lv_style_set_bg_color(&main_cont_style, lv_color_make(0xea, 0xea, 0xea));
    lv_style_set_pad_all(&main_cont_style, 0); // 设置样式内部填充
    lv_obj_add_style(pwd_main_cont, &main_cont_style, 0);  // 给对象添加样式
	lv_obj_set_size(pwd_main_cont, LV_PCT(100), LV_PCT(100)); //设置大小800x480
    lv_obj_center(pwd_main_cont); // 对象居屏幕中间显示
 
    // 键盘背景容器
    lv_obj_t *pwd_bg_cont = lv_obj_create(pwd_main_cont);
	if (pwd_bg_cont == NULL)
	{
		printf("[%s:%d] pwd_bg_cont create failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 键盘背景样式
	static lv_style_t bg_style;
    lv_style_reset(&bg_style);
	lv_style_init(&bg_style); // 初始化样式
    lv_style_set_radius(&bg_style, 10); // 设置样式的圆角弧度
	lv_style_set_border_width(&bg_style, 0); // 设置边框宽度
    lv_style_set_bg_opa(&bg_style, LV_OPA_COVER); // 设置样式背景的透明度，不透明
	lv_style_set_bg_color(&bg_style, lv_color_hex(0xBBFFFF)); //背景色设置为浅蓝色
    lv_style_set_pad_all(&bg_style, 0); // 设置样式内部填充
     // 样式添加渐变
    lv_style_set_bg_grad_color(&bg_style, lv_color_white()); // 渐变背景色为白色
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER); // 垂直方向渐变
    lv_style_set_bg_main_stop(&bg_style, 30); // 主停止色40
    lv_style_set_bg_grad_stop(&bg_style, 30); // 渐变停止色40
    lv_obj_add_style(pwd_bg_cont, &bg_style, 0);  // 给对象添加样式
	lv_obj_set_size(pwd_bg_cont, 353, 415); // 设置尺寸
	lv_obj_align(pwd_bg_cont, LV_ALIGN_TOP_MID, 0, 33); // 顶部居中显示
 
	// 键盘标题样式
    static lv_style_t title_label_style;
    lv_style_reset(&title_label_style);
	lv_style_init(&title_label_style); // 初始化样式
    lv_style_set_radius(&title_label_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&title_label_style, 0); //设置边框宽度
    lv_style_set_text_color(&title_label_style , lv_color_black());  // 字体颜色设置为白色
#if LAGN_CHN
    lv_style_set_text_font(&title_label_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&title_label_style, &lv_font_montserrat_18); //设置字体
#endif
 
    // 基于键盘背景对象创建键盘标题对象
    lv_obj_t *title_label = lv_label_create(pwd_bg_cont);
	if (title_label == NULL)
	{
		printf("[%s:%d] create title_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 设置长文本循环滚动模式
	//lv_obj_set_height(title_label, 23); // 设置对象的高度
    lv_obj_add_style(title_label, &title_label_style, 0); // 给btn_label添加样式
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10); // 顶部居中显示
	lv_label_set_text(title_label, "Input password"); // 设置文本内容
 
	// 基于键盘背景对象创建密码框
	pwd_text_area = lv_textarea_create(pwd_bg_cont);
	if (pwd_text_area == NULL)
    {
        printf("[%s:%d] create pwd_text_area obj failed\n", __FUNCTION__, __LINE__);
		return;
    }
 
    // 密码框样式
    static lv_style_t pwd_text_style;
    lv_style_reset(&pwd_text_style);
	lv_style_init(&pwd_text_style); // 初始化样式
    lv_style_set_radius(&pwd_text_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&pwd_text_style, 1); // 设置边框宽度
    lv_style_set_pad_all(&pwd_text_style, 0); // 设置样式的内部填充
#if LAGN_CHN
    lv_style_set_text_font(&pwd_text_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&pwd_text_style, &lv_font_montserrat_18); //设置字体
#endif
	lv_textarea_set_text(pwd_text_area, ""); // 文本框置空
	lv_textarea_set_password_mode(pwd_text_area, true); // 文本区域开启密码模式
	lv_textarea_set_max_length(pwd_text_area, 8); // 设置可输入的文本的最大长度
	lv_obj_add_style(pwd_text_area, &pwd_text_style, 0); // 给btn_label添加样式
 
    lv_obj_set_size(pwd_text_area, 295, 41); // 设置对象大小
	lv_obj_align_to(pwd_text_area, title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 30); // 顶部居中显示
 
	// 基于键盘背景对象创建密码校验提示标签
	hint_label = lv_label_create(pwd_bg_cont);
	if (hint_label == NULL)
	{
		printf("[%s:%d] create hint_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 密码提示文本样式
    static lv_style_t hint_label_style;
    lv_style_reset(&hint_label_style);
	lv_style_init(&hint_label_style); // 初始化样式
    lv_style_set_radius(&hint_label_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&hint_label_style, 0); //设置边框宽度
    lv_style_set_text_color(&hint_label_style , lv_palette_main(LV_PALETTE_RED));  // 字体颜色设置为红色
#if LAGN_CHN
    lv_style_set_text_font(&hint_label_style, (const lv_font_t *)(ft_info_14.font)); //设置字体
#else
	 lv_style_set_text_font(&hint_label_style, &lv_font_montserrat_12); //设置字体
#endif
    lv_label_set_long_mode(hint_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 长文本循环滚动
    lv_obj_add_style(hint_label, &hint_label_style, 0); // 给btn_label添加样式
    lv_obj_align(hint_label, LV_ALIGN_TOP_MID, 0, 110); // 顶部居中显示
	lv_label_set_text(hint_label, ""); // 设置文本内容
 
	// 基于键盘背景对象创建键盘对象
    lv_obj_t * pwd_keyboard = lv_keyboard_create(pwd_bg_cont);
    if (pwd_keyboard == NULL)
	{
		printf("[%s:%d] create pwd_keyboard obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 键盘样式
	static lv_style_t pwd_kb_style;
    lv_style_reset(&pwd_kb_style);
	lv_style_init(&pwd_kb_style); // 初始化样式
    lv_style_set_radius(&pwd_kb_style, 5); // 设置样式的圆角弧度
	lv_style_set_border_width(&pwd_kb_style, 0); //设置边框宽度
	lv_style_set_bg_color(&pwd_kb_style, lv_color_hex(0xFFFAFA));
	lv_style_set_text_opa(&pwd_kb_style, LV_OPA_COVER);
#if LAGN_CHN
    lv_style_set_text_font(&pwd_kb_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&pwd_kb_style, &lv_font_montserrat_20); //设置字体
#endif
 
	lv_obj_set_size(pwd_keyboard, 300, 220); //设置键盘大小
	lv_keyboard_set_mode(pwd_keyboard, LV_KEYBOARD_MODE_NUMBER); //设置键盘模式为数字键盘
	lv_keyboard_set_map(pwd_keyboard, LV_KEYBOARD_MODE_NUMBER, keyboard_map, keyboard_ctrl); // 设置键盘映射
	lv_keyboard_set_textarea(pwd_keyboard, pwd_text_area); // 键盘对象和文本框绑定
	lv_obj_add_style(pwd_keyboard, &pwd_kb_style, 0); // 添加样式
	lv_obj_align(pwd_keyboard, LV_ALIGN_TOP_MID, 0, 130);
 
 
    static lv_style_t btn_style;
    lv_style_reset(&btn_style);
	lv_style_init(&btn_style); // 初始化样式
    lv_style_set_radius(&btn_style, 5); // 设置样式的圆角弧度
	lv_style_set_border_width(&btn_style, 0); //设置边框宽度
	lv_style_set_bg_color(&btn_style, lv_color_hex(0xBBFFFF));
	lv_style_set_text_color(&btn_style, lv_color_hex(0x000000));
#if LAGN_CHN
    lv_style_set_text_font(&btn_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&btn_style, &lv_font_montserrat_18); //设置字体
#endif
 
    // 基于键盘背景对象创建确认按键
	lv_obj_t * confirm_btn = lv_btn_create(pwd_bg_cont);
	if (pwd_bg_cont == NULL)
	{
		printf("[%s:%d] pwd_bg_cont create failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_obj_add_event_cb(confirm_btn, confirm_btn_event_callback, LV_EVENT_CLICKED, NULL); // 给对象添加CLICK事件和事件处理回调函数
    lv_obj_add_style(confirm_btn, &btn_style, 0); // 添加按键样式
    lv_obj_set_size(confirm_btn, 112, 37); // 设置按键对象大小
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_MID, -65, 365); // 设置按键对象位置
 
     // 基于confirm_btn对象创建标签
    lv_obj_t *confirm_btn_label = lv_label_create(confirm_btn);
	if (confirm_btn_label == NULL)
	{
		printf("[%s:%d] create confirm_btn_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_label_set_text(confirm_btn_label, "OK"); // 设置文本内容
    lv_obj_center(confirm_btn_label); // 居中显示
 
    lv_obj_t * cancel_btn = lv_btn_create(pwd_bg_cont);
	if (cancel_btn == NULL)
	{
		printf("[%s:%d] cancel_btn create failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_obj_add_event_cb(cancel_btn, cancel_btn_event_callback, LV_EVENT_CLICKED, NULL); // 给对象添加CLICK事件和事件处理回调函数
    lv_obj_add_style(cancel_btn, &btn_style, 0);
    lv_obj_set_size(cancel_btn, 112, 37);
    lv_obj_align(cancel_btn, LV_ALIGN_TOP_MID, 65, 365);
 
    lv_obj_t *cancel_btn_label = lv_label_create(cancel_btn);
	if (cancel_btn_label == NULL)
	{
		printf("[%s:%d] create cancel_btn_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_label_set_text(cancel_btn_label, "Cancel"); // 设置文本内容
    lv_obj_center(cancel_btn_label);
}
 

// 模式转换显示键盘输入界面
void lv_mode_password_keyboard_display(void)
{
    // 当前页面的画布容器
    pwd_main_cont = lv_obj_create(lv_scr_act()); // 基于屏幕创建了一个容器
	if (pwd_main_cont == NULL)
	{
		printf("[%s:%d] create pwd_main_cont failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 画布样式
	static lv_style_t main_cont_style;
    lv_style_reset(&main_cont_style);
	lv_style_init(&main_cont_style); // 初始化样式
    lv_style_set_radius(&main_cont_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&main_cont_style, 0); // 设置边框宽度
	lv_style_set_bg_opa(&main_cont_style, LV_OPA_50); // 设置样式背景的透明度，不透明
    lv_style_set_bg_color(&main_cont_style, lv_color_make(0xea, 0xea, 0xea));
    lv_style_set_pad_all(&main_cont_style, 0); // 设置样式内部填充
    lv_obj_add_style(pwd_main_cont, &main_cont_style, 0);  // 给对象添加样式
	lv_obj_set_size(pwd_main_cont, LV_PCT(100), LV_PCT(100)); //设置大小800x480
    lv_obj_center(pwd_main_cont); // 对象居屏幕中间显示
 
    // 键盘背景容器
    lv_obj_t *pwd_bg_cont = lv_obj_create(pwd_main_cont);
	if (pwd_bg_cont == NULL)
	{
		printf("[%s:%d] pwd_bg_cont create failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 键盘背景样式
	static lv_style_t bg_style;
    lv_style_reset(&bg_style);
	lv_style_init(&bg_style); // 初始化样式
    lv_style_set_radius(&bg_style, 10); // 设置样式的圆角弧度
	lv_style_set_border_width(&bg_style, 0); // 设置边框宽度
    lv_style_set_bg_opa(&bg_style, LV_OPA_COVER); // 设置样式背景的透明度，不透明
	lv_style_set_bg_color(&bg_style, lv_color_hex(0xBBFFFF)); //背景色设置为浅蓝色
    lv_style_set_pad_all(&bg_style, 0); // 设置样式内部填充
     // 样式添加渐变
    lv_style_set_bg_grad_color(&bg_style, lv_color_white()); // 渐变背景色为白色
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER); // 垂直方向渐变
    lv_style_set_bg_main_stop(&bg_style, 30); // 主停止色40
    lv_style_set_bg_grad_stop(&bg_style, 30); // 渐变停止色40
    lv_obj_add_style(pwd_bg_cont, &bg_style, 0);  // 给对象添加样式
	lv_obj_set_size(pwd_bg_cont, 353, 415); // 设置尺寸
	lv_obj_align(pwd_bg_cont, LV_ALIGN_TOP_MID, 0, 33); // 顶部居中显示
 
	// 键盘标题样式
    static lv_style_t title_label_style;
    lv_style_reset(&title_label_style);
	lv_style_init(&title_label_style); // 初始化样式
    lv_style_set_radius(&title_label_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&title_label_style, 0); //设置边框宽度
    lv_style_set_text_color(&title_label_style , lv_color_black());  // 字体颜色设置为白色
#if LAGN_CHN
    lv_style_set_text_font(&title_label_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&title_label_style, &lv_font_montserrat_18); //设置字体
#endif
 
    // 基于键盘背景对象创建键盘标题对象
    lv_obj_t *title_label = lv_label_create(pwd_bg_cont);
	if (title_label == NULL)
	{
		printf("[%s:%d] create title_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 设置长文本循环滚动模式
	//lv_obj_set_height(title_label, 23); // 设置对象的高度
    lv_obj_add_style(title_label, &title_label_style, 0); // 给btn_label添加样式
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10); // 顶部居中显示
	lv_label_set_text(title_label, "Input password"); // 设置文本内容
 
	// 基于键盘背景对象创建密码框
	pwd_text_area = lv_textarea_create(pwd_bg_cont);
	if (pwd_text_area == NULL)
    {
        printf("[%s:%d] create pwd_text_area obj failed\n", __FUNCTION__, __LINE__);
		return;
    }
 
    // 密码框样式
    static lv_style_t pwd_text_style;
    lv_style_reset(&pwd_text_style);
	lv_style_init(&pwd_text_style); // 初始化样式
    lv_style_set_radius(&pwd_text_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&pwd_text_style, 1); // 设置边框宽度
    lv_style_set_pad_all(&pwd_text_style, 0); // 设置样式的内部填充
#if LAGN_CHN
    lv_style_set_text_font(&pwd_text_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&pwd_text_style, &lv_font_montserrat_18); //设置字体
#endif
	lv_textarea_set_text(pwd_text_area, ""); // 文本框置空
	lv_textarea_set_password_mode(pwd_text_area, true); // 文本区域开启密码模式
	lv_textarea_set_max_length(pwd_text_area, 8); // 设置可输入的文本的最大长度
	lv_obj_add_style(pwd_text_area, &pwd_text_style, 0); // 给btn_label添加样式
 
    lv_obj_set_size(pwd_text_area, 295, 41); // 设置对象大小
	lv_obj_align_to(pwd_text_area, title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 30); // 顶部居中显示
 
	// 基于键盘背景对象创建密码校验提示标签
	hint_label = lv_label_create(pwd_bg_cont);
	if (hint_label == NULL)
	{
		printf("[%s:%d] create hint_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 密码提示文本样式
    static lv_style_t hint_label_style;
    lv_style_reset(&hint_label_style);
	lv_style_init(&hint_label_style); // 初始化样式
    lv_style_set_radius(&hint_label_style, 0); // 设置样式的圆角弧度
	lv_style_set_border_width(&hint_label_style, 0); //设置边框宽度
    lv_style_set_text_color(&hint_label_style , lv_palette_main(LV_PALETTE_RED));  // 字体颜色设置为红色
#if LAGN_CHN
    lv_style_set_text_font(&hint_label_style, (const lv_font_t *)(ft_info_14.font)); //设置字体
#else
	 lv_style_set_text_font(&hint_label_style, &lv_font_montserrat_12); //设置字体
#endif
    lv_label_set_long_mode(hint_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 长文本循环滚动
    lv_obj_add_style(hint_label, &hint_label_style, 0); // 给btn_label添加样式
    lv_obj_align(hint_label, LV_ALIGN_TOP_MID, 0, 110); // 顶部居中显示
	lv_label_set_text(hint_label, ""); // 设置文本内容
 
	// 基于键盘背景对象创建键盘对象
    lv_obj_t * pwd_keyboard = lv_keyboard_create(pwd_bg_cont);
    if (pwd_keyboard == NULL)
	{
		printf("[%s:%d] create pwd_keyboard obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
	// 键盘样式
	static lv_style_t pwd_kb_style;
    lv_style_reset(&pwd_kb_style);
	lv_style_init(&pwd_kb_style); // 初始化样式
    lv_style_set_radius(&pwd_kb_style, 5); // 设置样式的圆角弧度
	lv_style_set_border_width(&pwd_kb_style, 0); //设置边框宽度
	lv_style_set_bg_color(&pwd_kb_style, lv_color_hex(0xFFFAFA));
	lv_style_set_text_opa(&pwd_kb_style, LV_OPA_COVER);
#if LAGN_CHN
    lv_style_set_text_font(&pwd_kb_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&pwd_kb_style, &lv_font_montserrat_20); //设置字体
#endif
 
	lv_obj_set_size(pwd_keyboard, 300, 220); //设置键盘大小
	lv_keyboard_set_mode(pwd_keyboard, LV_KEYBOARD_MODE_NUMBER); //设置键盘模式为数字键盘
	lv_keyboard_set_map(pwd_keyboard, LV_KEYBOARD_MODE_NUMBER, keyboard_map, keyboard_ctrl); // 设置键盘映射
	lv_keyboard_set_textarea(pwd_keyboard, pwd_text_area); // 键盘对象和文本框绑定
	lv_obj_add_style(pwd_keyboard, &pwd_kb_style, 0); // 添加样式
	lv_obj_align(pwd_keyboard, LV_ALIGN_TOP_MID, 0, 130);
 
 
    static lv_style_t btn_style;
    lv_style_reset(&btn_style);
	lv_style_init(&btn_style); // 初始化样式
    lv_style_set_radius(&btn_style, 5); // 设置样式的圆角弧度
	lv_style_set_border_width(&btn_style, 0); //设置边框宽度
	lv_style_set_bg_color(&btn_style, lv_color_hex(0xBBFFFF));
	lv_style_set_text_color(&btn_style, lv_color_hex(0x000000));
#if LAGN_CHN
    lv_style_set_text_font(&btn_style, (const lv_font_t *)(ft_info_18.font)); //设置字体
#else
	 lv_style_set_text_font(&btn_style, &lv_font_montserrat_18); //设置字体
#endif
 
    // 基于键盘背景对象创建确认按键
	lv_obj_t * confirm_btn = lv_btn_create(pwd_bg_cont);
	if (pwd_bg_cont == NULL)
	{
		printf("[%s:%d] pwd_bg_cont create failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_obj_add_event_cb(confirm_btn, confirm_mode_event_callback, LV_EVENT_CLICKED, NULL); // 给对象添加CLICK事件和事件处理回调函数
    lv_obj_add_style(confirm_btn, &btn_style, 0); // 添加按键样式
    lv_obj_set_size(confirm_btn, 112, 37); // 设置按键对象大小
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_MID, -65, 365); // 设置按键对象位置
 
     // 基于confirm_btn对象创建标签
    lv_obj_t *confirm_btn_label = lv_label_create(confirm_btn);
	if (confirm_btn_label == NULL)
	{
		printf("[%s:%d] create confirm_btn_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_label_set_text(confirm_btn_label, "OK"); // 设置文本内容
    lv_obj_center(confirm_btn_label); // 居中显示
 
    lv_obj_t * cancel_btn = lv_btn_create(pwd_bg_cont);
	if (cancel_btn == NULL)
	{
		printf("[%s:%d] cancel_btn create failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_obj_add_event_cb(cancel_btn, cancel_mode_event_callback, LV_EVENT_CLICKED, NULL); // 给对象添加CLICK事件和事件处理回调函数
    lv_obj_add_style(cancel_btn, &btn_style, 0);
    lv_obj_set_size(cancel_btn, 112, 37);
    lv_obj_align(cancel_btn, LV_ALIGN_TOP_MID, 65, 365);
 
    lv_obj_t *cancel_btn_label = lv_label_create(cancel_btn);
	if (cancel_btn_label == NULL)
	{
		printf("[%s:%d] create cancel_btn_label obj failed\n", __FUNCTION__, __LINE__);
		return;
	}
 
    lv_label_set_text(cancel_btn_label, "Cancel"); // 设置文本内容
    lv_obj_center(cancel_btn_label);
}

// 模式确认按键的事件回调
static void confirm_mode_event_callback(lv_event_t* event)
{
	if (event == NULL)
	{
		printf("[%s:%d] event is NULL\n", __FUNCTION__, __LINE__);
		return ;
	}
 
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
	{
		//printf("[%s:%d] confirm button clicked\n", __FUNCTION__, __LINE__);
		if (pwd_text_area != NULL)
        {
            password_read();
            const char *pwd_txt = lv_textarea_get_text(pwd_text_area);
            if ((pwd_txt != NULL))
            {
                if (strcmp(pwd_txt, password_input) == 0)
                {
                    if (hint_label != NULL)
                    {
                        lv_label_set_text(hint_label, "Password correct!");
                        lv_obj_del(pwd_main_cont);
						password_mode_lock = true;
                        Judgmentmode();
						lv_bar_change();
                    }
                }
                else if (pwd_txt[0] == '\0')
                {
                    if (hint_label != NULL)
                    {
                        lv_label_set_text(hint_label, "Password can not be empty!");
                    }
                }
                else
                {
                    if (hint_label != NULL)
                    {
                        lv_label_set_text(hint_label, "Password error!");
                    }
                }
            }
        }
	}
}
 
// 确认按键的事件回调
static void confirm_btn_event_callback(lv_event_t* event)
{
	if (event == NULL)
	{
		printf("[%s:%d] event is NULL\n", __FUNCTION__, __LINE__);
		return ;
	}
 
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
	{
		//printf("[%s:%d] confirm button clicked\n", __FUNCTION__, __LINE__);
		if (pwd_text_area != NULL)
        {
            password_read();
            const char *pwd_txt = lv_textarea_get_text(pwd_text_area);
            if ((pwd_txt != NULL))
            {
                if (strcmp(pwd_txt, password_input) == 0)
                {
                    if (hint_label != NULL)
                    {
                        lv_label_set_text(hint_label, "Password correct!");
                        lv_obj_del(pwd_main_cont);
                        password_lock_open = true;
                    }
                }
                else if (pwd_txt[0] == '\0')
                {
                    if (hint_label != NULL)
                    {
                        lv_label_set_text(hint_label, "Password can not be empty!");
                    }
                }
                else
                {
                    if (hint_label != NULL)
                    {
                        lv_label_set_text(hint_label, "Password error!");
                    }
                }
            }
        }
	}
}

// 模式取消按键的事件回调
static void cancel_mode_event_callback(lv_event_t* event)
{
	if (event == NULL)
	{
		printf("[%s:%d] event is NULL\n", __FUNCTION__, __LINE__);
		return ;
	}
 
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
	{
		//printf("[%s:%d] cancel button clicked\n", __FUNCTION__, __LINE__);
        if (pwd_text_area != NULL)
        {
            lv_textarea_set_text(pwd_text_area, ""); // 文本框置空
        }
 
        if (hint_label != NULL)
        {
            lv_label_set_text(hint_label, "");
        }

		lv_obj_del(pwd_main_cont);
	}
}
 
// 取消按键的事件回调
static void cancel_btn_event_callback(lv_event_t* event)
{
	if (event == NULL)
	{
		printf("[%s:%d] event is NULL\n", __FUNCTION__, __LINE__);
		return ;
	}
 
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
	{
		//printf("[%s:%d] cancel button clicked\n", __FUNCTION__, __LINE__);
        if (pwd_text_area != NULL)
        {
            lv_textarea_set_text(pwd_text_area, ""); // 文本框置空
        }
 
        if (hint_label != NULL)
        {
            lv_label_set_text(hint_label, "");
        }
	}
}

static void Scram_reset_event_cb(lv_event_t* event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t* obj = lv_event_get_current_target(event);   //获取当前点击对象

    if (code == LV_EVENT_CLICKED)
    {
        //判断是哪个按钮，进行封装数据   Master_scram    Total_reduction
        switch ((int)obj->user_data)
        {
            case 0:
                printf("急停\n");
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[MASTER_SCRAM_FUNCTION_ENABLED]);
                lv_style_reset(&style_indic);
                lv_obj_del(mask);
                break;
            
            case 1:
                printf("复位\n");
                OneNet_Publish(MQTT_PUBLIC_SPORTS_DEVICE_THEME, USRNET_MQTT_MSH[TOTAL_REDUCTION_FUNCTION_ENABLED]);
                lv_style_reset(&style_indic);
                lv_obj_del(mask);
                break;

            default:
                break;
        }
    }
}

/**
 * 创建按钮
 * @param obj          指向父对象的指针    
 * @param text         按钮的名称
 * @param x_ofs        水平偏移 x_ofs
 * @param y_ofs        水平偏移 y_ofs
 * @param index        按到第几个按钮
 * */
static void create_button(lv_obj_t *obj, const char *text, lv_coord_t x_ofs, lv_coord_t y_ofs, uint8_t index)
{
    lv_obj_t *btn = lv_obj_create(obj);
    lv_obj_set_size(btn, 70, 40);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(btn, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, 80, LV_PART_MAIN);

    
    lv_obj_set_user_data(btn, index);   //设置用户数据，表示哪个按钮
    lv_obj_add_event_cb(btn, Scram_reset_event_cb, LV_EVENT_ALL, NULL);  
    
    lv_obj_align(btn, LV_ALIGN_CENTER, x_ofs, y_ofs);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_PART_MAIN);
    lv_obj_center(label);
}

/**
 * 进度条回调，用于设置进度条值
 * @param bar      哪个进度条
 * @param temp     设置的值
*/
static void set_temp(void * bar, int32_t temp)
{

    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

/**
 * 定时器回调，用于延迟删除样式和对象
 * @param timer    创建的定时器
*/
static void lv_example_bar_3_del_back(lv_timer_t * timer)
{
    lv_style_reset(&style_indic);
    lv_obj_del(mask);
    if (timer)
        lv_timer_del(timer);
}

/**
 * 转换成功动画结束时的回调，创建定时器来延时
*/
static void set_del_back(void)
{
    lv_label_set_text(anaim_label, "转换成功");
    lv_timer_t *del_timer = lv_timer_create(lv_example_bar_3_del_back, 3000, NULL);
}

/**
 * 转换模式进度条
 */
static void lv_bar_change(void)
{
    static lv_style_t style_indic;
    lv_obj_t * anaim_bar;

    mask = lv_mode_create_mask_box(lv_scr_act());

    create_button(mask, "急停", -50, -100, 0);
    create_button(mask, "复位", 50, -100, 1);

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    anaim_bar = lv_bar_create(mask);
    lv_obj_add_style(anaim_bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(anaim_bar, 200, 20);
    lv_obj_center(anaim_bar);
    lv_bar_set_range(anaim_bar, -20, 40);

    anaim_label = lv_label_create(mask);
    lv_obj_set_style_text_color(anaim_label, lv_color_hex(0xffffff), LV_PART_MAIN);
	lv_obj_set_style_text_font(anaim_label, &PuHuiTi_Regular_30, LV_PART_MAIN);
    lv_label_set_text(anaim_label, "正在转换");
    lv_obj_align_to(anaim_label, anaim_bar, LV_ALIGN_TOP_MID, 0, 80);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    lv_anim_set_time(&a, 40000);
    //lv_anim_set_playback_time(&a, -30000);
    lv_anim_set_var(&a, anaim_bar);
    lv_anim_set_values(&a, -20, 40);
    //lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    //lv_anim_start(&a);

    lv_anim_t a_label;
    lv_anim_init(&a_label);
    lv_anim_set_var(&a_label, anaim_label);
    // lv_anim_set_exec_cb(&a_label, (lv_anim_exec_xcb_t)lv_obj_set_y); // y轴移动
    lv_anim_set_time(&a_label, 40000);
    // lv_anim_set_values(&a_label, -100, 310);
    // lv_anim_set_path_cb(&a_label, lv_anim_path_bounce); //模拟弹性物体下落动画
    lv_anim_set_ready_cb(&a_label, set_del_back);


    lv_anim_timeline_t* anim_timeline = lv_anim_timeline_create();
    lv_anim_timeline_add(anim_timeline, 0, &a);
    lv_anim_timeline_add(anim_timeline, 0, &a_label);
    lv_anim_timeline_start(anim_timeline);

}
