/*** 
 * @Description: 锁屏界面
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-06-16 09:55:17
 * @LastEditTime: 2023-06-16 09:58:06
 * @FilePath: lv_node/src/common/lock.h
 */
#ifndef __LOCK_H__
#define __LOCK_H__

extern lv_obj_t *pwd_main_cont;
extern bool password_lock_open;
extern bool password_mode_lock;

void lv_gui_password_keyboard_display(void);
void lv_mode_password_keyboard_display(void);
void password_read();
int password_write(char *password_input);

#endif