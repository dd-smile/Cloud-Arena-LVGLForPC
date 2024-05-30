/*
 * @Description: 数据看板通用代码
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-04-09 11:28:17
 * @LastEditTime: 2023-04-10 15:59:05
 * @FilePath: lv_node/src/display/display_common.c
 */
#include "ui_app.h"

/**
 * 设置设备数据卡片的图片样式
 * @param parent                    指向父对象的指针
 * @param src                       要设置的图片
 * @param align                     对齐方式
 * @param x_ofs                     x轴偏移量
 * @param y_ofs                     y轴偏移量
 * @return                          返回一个图片对象
*/
lv_obj_t *equipment_data_style(lv_obj_t *parent, const void *src, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *temp_image = lv_img_create(parent);
    lv_img_set_src(temp_image, src);
    lv_obj_align_to(temp_image, parent, align, x_ofs, y_ofs);
    return temp_image;
}

/**
 * 设置温湿度数据文本样式
 * @param destlabel                  指向要设置的文本对象的指针
 * @param alignlabel                 指向要对齐的文本对象的指针
 * @param align                      对齐方式
 * @param x_ofs                      x轴偏移量
 * @param y_ofs                      y轴偏移量
*/
void tempehum_label_style(lv_obj_t *destlabel, lv_obj_t *alignlabel, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_set_style_text_font(destlabel, &PuHuiTi_Regular_16, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(destlabel, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_align_to(destlabel, alignlabel, align, x_ofs, y_ofs);
}

/**
 * 创建一个温湿度数据卡片
 * @param parent                     指向父对象的指针
*/
lv_obj_t *CreateTemHumCard(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 295, 235);
    lv_obj_set_style_bg_color(card, lv_color_make(31, 38, 51), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(card, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 0, LV_STATE_DEFAULT);
    return card;
}


/**
 * 创建一个用电趋势数据卡片
 * @param parent                     指向父对象的指针
*/
lv_obj_t *CreateElectricityCard(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 789, 235);
    lv_obj_set_style_bg_color(card, lv_color_make(31, 38, 51), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(card, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 0, LV_STATE_DEFAULT);
    return card;
}

/**
 * 创建一个数据看板标题
 * @param parent                       指向父对象的指针
 * @param text                         要设置的文本
 * @param align                        对齐方式
 * @param x_ofs                        x轴偏移量
 * @param y_ofs                        y轴偏移量
 * @return                             返回一个文本对象
*/
lv_obj_t *CreateDataBoardtitle(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *title = lv_label_create(parent);
    lv_label_set_text(title, text);

    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(title, 2, LV_STATE_DEFAULT);

    lv_obj_align_to(title, parent, align, x_ofs, y_ofs);

    return title;
}


/**
 * 创建一个数据看板文本
 * @param parent                       指向父对象的指针
 * @param text                         要设置的文本
 * @param align                        对齐方式
 * @param x_ofs                        x轴偏移量
 * @param y_ofs                        y轴偏移量
 * @return                             返回一个文本对象
*/
lv_obj_t *CreateDataBoardtxt(lv_obj_t *parent, const char *text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *txt = lv_label_create(parent);
    lv_label_set_text(txt, text);

    lv_obj_set_style_text_color(txt, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(txt, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);

    lv_obj_align_to(txt, parent, align, x_ofs, y_ofs);

    return txt;
}


/**
 * 获取温度数据
 * @param Buf                         要解析的文本
 * @param Res                         解析后的温度数据
 * @return                            执行成功返回0
*/
int searchTemp(char *Buf, char *Res)
{
	char *Begin = NULL;    //定义开始指针
	char *End = NULL;      //定义结束指针

    /*寻找第一次出现<temp>的位置  <temp>温度</temp>...
    再偏移strlen("<temp>")个地址可以得到温度</temp>...*/
	Begin = strstr(Buf, "<temp>");//寻找第一次出现<temp>的位置
    End = strstr(Buf, "</temp>");//寻找第一次出现</temp>的位置

	if(Begin == NULL || End == NULL || Begin > End)
    {
        printf("寻找错误!\n");
    }else{
		Begin = Begin + strlen("<temp>");//温度</temp>...
		memcpy(Res, Begin, End-Begin);//获得Begin和End之间的值
	}
	return 0;
}


/**
 * 获取湿度数据
 * @param Buf                         要解析的文本
 * @param Res                         解析后的湿度数据
 * @return                            执行成功返回0
*/
int searchHum(char *Buf, char *Res)
{
	char *Begin = NULL;
	char *End = NULL;

	Begin = strstr(Buf, "<hum>");
	End = strstr(Buf, "</hum>");

	if(Begin == NULL || End == NULL || Begin > End)
    {
    printf("寻找错误!\n"); 
    }else{
		Begin = Begin + strlen("<hum>");
		memcpy(Res, Begin, End-Begin);
	}
	return 0;
}


void *listening_temphum(void * parg)
{
    int len;
    char buf[1024];
    // 阻塞等待并接受客户端连接
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    g_cfd = accept(g_lfd, (struct sockaddr*)&cliaddr, &clilen);
    if(g_cfd == -1)
    {
        perror("accept");
        exit(0);
    }
    // 打印客户端的地址信息
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
            ntohs(cliaddr.sin_port));
    while (1)
    {
        // 和客户端通信
        // 接收数据
        memset(buf, 0, sizeof(buf));
        len = read(g_cfd, buf, sizeof(buf));
        if(len > 0)
        {
        g_cBuf = buf;
        searchTemp(g_cBuf, g_temp_data);
        searchHum(g_cBuf, g_hum_data);

        snprintf(PUB_BUF, sizeof(PUB_BUF), "{\"Temp\":%s,\"Hum\":%s}",g_temp_data, g_hum_data);
        OneNet_Publish(MQTT_HUMITURE_DATA_THEME, PUB_BUF);

        //   printf("温度: %s, 湿度: %s\n", g_temp_data, g_hum_data);
        }
    }

}

