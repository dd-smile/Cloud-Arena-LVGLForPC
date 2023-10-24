/*** 
 * @Description: 
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-06-16 09:55:17
 * @LastEditTime: 2023-06-16 09:58:06
 * @FilePath: lv_node/src/common/lock.h
 */
#ifndef __LOCK_H__
#define __LOCK_H__

extern lv_obj_t *pwd_main_cont;
extern bool password_lock_open;

void lv_gui_password_keyboard_display();

#endif