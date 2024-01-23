/*
 * @Description: 音频控制面板－利亚德
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-26 11:28:17
 * @LastEditTime: 2023-09-28 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/audio_control/leyard.c
 */
#include "ui_app.h"

static int synchronous_fd = 0;  //实时控制客户端的文件描述符
static int synchronous_red = -1;
int multitrack_fd = 0;   //多轨控制客户端的文件描述符
int multitrack_red = -1;   //多轨控制客户端是否连接上服务器
char buf_audio[1024];  //用于存放发送的数据

//                                        00 10 00 80 00 C1 00 03 00 0F 00 00 00 00 00 FF       16
unsigned char packet_mute_fifteen[16] = {0x00, 0x10, 0x00, 0x80, 0x00, 0xC1, 0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0xFF};  //存放静音16进制数据
unsigned char packet_unmute_fifteen[16] = {0x00, 0x10, 0x00, 0x80, 0x00, 0xC1, 0x00, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}; //存放取消静音16进制数据
unsigned char packet_mute_sixteen[16] = {0x00, 0x10, 0x00, 0x80, 0x00, 0xC1, 0x00, 0x03, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0xFF};  //存放静音16进制数据
unsigned char packet_unmute_sixteen[16] = {0x00, 0x10, 0x00, 0x80, 0x00, 0xC1, 0x00, 0x03, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}; //存放取消静音16进制数据



lv_obj_t *connect_label;  //连接提示框


/*--------------------------------通信--------------------------------*/

/**
 * 创建多轨控制客户端
 * @param parent            指向父对象的指针     
 * */
void *create_client_mu(void * parg)
{   
    multitrack_fd = createSocket();  //创建套接字
    synchronous_fd = createSocket();  //创建套接字
    multitrack_red = connectToHost(multitrack_fd, LEYARD_SERVER_MIXCRAFT_IP, LEYARD_SERVER_MIXCRAFT_PORT);  //连接服务器
    synchronous_red = connectToHost(synchronous_fd, LEYARD_SERVER_SYNCHRONOUS_IP, LEYARD_SERVER_SYNCHRONOUS_PORT);  //连接服务器
    while (1)
    {
      if(socketconnected(multitrack_fd) == 0)
      {
        closeSocket(multitrack_fd);
        multitrack_fd = createSocket();  //创建套接字
        multitrack_red = connectToHost(multitrack_fd, LEYARD_SERVER_MIXCRAFT_IP, LEYARD_SERVER_MIXCRAFT_PORT);  //连接服务器
      }else if (socketconnected(synchronous_fd) == 0)
      {
        closeSocket(synchronous_fd);
        synchronous_fd = createSocket();
        synchronous_red = connectToHost(synchronous_fd, LEYARD_SERVER_SYNCHRONOUS_IP, LEYARD_SERVER_SYNCHRONOUS_PORT);
      }
      sleep(3);
    }
    
}


//发送静音的数据
void synchronous_mutework()
{
    if (synchronous_red != -1)
    {
        write(synchronous_fd, packet_mute_fifteen, sizeof(packet_mute_fifteen));
        usleep(50 * 1000);
        write(synchronous_fd, packet_mute_sixteen, sizeof(packet_mute_sixteen));
    }
}

//发送取消静音的数据
void synchronous_unmutework()
{
    if (synchronous_red != -1)
    {
        write(synchronous_fd, packet_unmute_fifteen, sizeof(packet_unmute_fifteen));
        usleep(50 * 1000);
        write(synchronous_fd, packet_unmute_sixteen, sizeof(packet_unmute_sixteen));
    }
}



//检测音频服务器是否断开
void audio_working()
{
    if (socketconnected(multitrack_fd) == 0)
    {   
        closeSocket(multitrack_fd);
        lv_label_set_text(connect_label, "请检查服务器是否开启");
    }
    else
    {
        lv_label_set_text(connect_label, "");
        write(multitrack_fd, buf_audio, strlen(buf_audio)+1);
    }
}



/**多轨控制按钮点击事件**/
void multitrack_Controls_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_current_target(e);   //获取当前点击对象


    if (code == LV_EVENT_CLICKED)
    {
        //判断是哪个按钮，进行封装数据
        switch ((int)obj->user_data)
        {
        case 0:  //音乐1
                sprintf(buf_audio, "music1@5F");  //播放
                /* 向线程池中添加任务 */
                //threadpool_add(thp, (void *)&multitrack_working, 0);
                audio_working();
            break;
        case 1:   //音乐2
                sprintf(buf_audio, "music2@5F");  //播放
                audio_working();
            break;
        case 2:   //音乐3
                sprintf(buf_audio, "music3@5F");  //播放
                audio_working();
            break;
        case 3:   //音乐4
                sprintf(buf_audio, "music4@5F");  //播放
                audio_working();
            break;
        case 4:   //大自然音乐
                sprintf(buf_audio, "daziran@5F");  //播放
                audio_working();
            break;
        case 5:   //音量增加5db
                sprintf(buf_audio, "V+@5F");  
                audio_working();
            break;
        case 6:   //音量减少5db
                sprintf(buf_audio, "V-@5F");
                audio_working();
            break;
        // case 7:   //初始音量-15db
        //     sprintf(buf_audio, "-15db@5F");
        //     write(multitrack_fd, buf_audio, strlen(buf_audio)+1);
        //     break;
        case 8:   //切换下一首
                sprintf(buf_audio, "nest@5F");
                audio_working();
            break;
        case 9:   //暂停播放
                sprintf(buf_audio, "stop@5F");
                audio_working();
            break;

        default:
            break;
        }
    }
}




