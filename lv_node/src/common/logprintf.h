/*** 
 * @Description: 打印日志到文件中
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-06-07 09:55:17
 * @LastEditTime: 2024-06-07 17:58:06
 * @FilePath: lv_node/src/common/logprintf.h
 */
#ifndef __LOGPRINTF_H__
#define __LOGPRINTF_H__


#define COMPO_TEXTBOX_FMT_LEN   256

int init_logprintf_file(void);
void log_printf(const char* fmt, ...);
void log_err_printf(char* err);


#endif // LOGPRINTF_H
