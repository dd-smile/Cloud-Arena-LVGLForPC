/*** 
 * @Description: 打印日志到文件中
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-06-07 09:55:17
 * @LastEditTime: 2024-06-07 17:58:06
 * @FilePath: lv_node/src/common/logprintf.c
 */
#include "ui_app.h"

static FILE *fp_logprintf =NULL;

/**
 * 初始化日志文件
 * @return int 0:成功 -1:失败
*/
int init_logprintf_file(void)
{
    fp_logprintf = fopen("lv_node/src/common/ycglog.txt","at+");
    if(NULL == fp_logprintf)
    {
        printf("open failed\n");
        return -1;
    }
    char* version = "F_10.1";
    fprintf(fp_logprintf,"log output %s\n",version);

    fclose(fp_logprintf);
    
    return 0;
}

/**
 * 打印日志到文件中
 * @param fmt 格式化字符串
 * @param ... 参数列表
*/
void log_printf(const char* fmt, ...)
{
    time_t t_time;
    struct tm* current_time;

    fp_logprintf = fopen("lv_node/src/common/ycglog.txt","at+");
    if(NULL == fp_logprintf)
    {
        printf("open failed\n");
    }

    // 获取当前时间戳
    time(&t_time);
    current_time = localtime(&t_time);
    char buff[COMPO_TEXTBOX_FMT_LEN] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, COMPO_TEXTBOX_FMT_LEN, fmt, args);
    va_end(args);

    printf("%s %04d-%02d-%02d[%02d:%02d:%02d]\n",buff,current_time->tm_year+1900,current_time->tm_mon+1,current_time->tm_mday,current_time->tm_hour,current_time->tm_min,current_time->tm_sec);
    fprintf(fp_logprintf,"%s %04d-%02d-%02d[%02d:%02d:%02d]\n",buff,current_time->tm_year+1900,current_time->tm_mon+1,current_time->tm_mday,current_time->tm_hour,current_time->tm_min,current_time->tm_sec);


    fclose(fp_logprintf);
}

/**
 * 打印错误日志到文件中
 * @param err 错误信息
*/
void log_err_printf(char* err)
{
    time_t t_time;
    struct tm* current_time;

    fp_logprintf = fopen("lv_node/src/common/ycglog.txt","at+");
    if(NULL == fp_logprintf)
    {
        printf("open failed\n");
    }

    // 获取当前时间戳
    time(&t_time);
    current_time = localtime(&t_time);

    fprintf(fp_logprintf,"%s %04d-%02d-%02d[%02d:%02d:%02d]\n",err,current_time->tm_year+1900,current_time->tm_mon+1,current_time->tm_mday,current_time->tm_hour,current_time->tm_min,current_time->tm_sec);


    fclose(fp_logprintf);
}