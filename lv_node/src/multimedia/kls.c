/*
 * @Description: 克拉斯中控系统指令
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-01-18 11:28:17
 * @LastEditTime: 2023-01-28 15:59:05
 * @FilePath: lv_node/src/multimedia/kls.c
 */
#include "ui_app.h"

int kls_fd = 0;
struct sockaddr_in kls_seraddr;


/*
关于PPT播放，电脑上必须安装了Microsoft Office 2016及以上的版本
*/

/**
 * 发送KLS中控指令
 * @param command            要发送的指令
*/
void set_Central_control_system_command(const char *command)
{
    sendto(kls_fd, command, strlen(command), 0, (struct sockaddr *)&kls_seraddr, sizeof(kls_seraddr));
}

/**
 * 创建KLS客户端  UDP
 * */
void *create_client_kls(void)
{
    if (kls_fd == 0)
    {
        kls_fd= socket(AF_INET, SOCK_DGRAM, 0);
        if (kls_fd == -1)
        {
            perror("socket");
            exit(0);
        }
        
        kls_seraddr.sin_family = AF_INET;
        kls_seraddr.sin_port = htons(KLS_SERVER_PORT);    
        inet_pton(AF_INET, KLS_SERVER_IP, &kls_seraddr.sin_addr.s_addr);
    }
}