/*
 * @Description: 中央控制系统，用于接收平台端下发的指令，并执行相应的操作
 * @Author: da
 * @LastEditors: da
 * @Date: 2024-05-16 15:32:17
 * @LastEditTime: 2024-05-23 15:59:05
 * @FilePath: lv_node/src/mqtt/center_console.c
 */
#include "ui_app.h"

// 函数声明
static void handle_video(int video_number);
static void handle_ppt(int ppt_number);
static void handle_auto(int auto_number);


/**
 * 解析MQTT的JSON数据(数组单个元素)
 * @param msg         指向JSON数据的指针
 * @param info        指向声光电结构体的指针
*/
void OneNet_ParseJsonNow(char *msg, Soundlight_Data *info)
{
	JSON_Value *root_value;
    JSON_Object *root_object;
    JSON_Array *devices;
    JSON_Object *first_device;

	root_value = json_parse_string(msg);
    if (root_value == NULL) {
        printf("JSON 解析失败\n");
        return;
    }
    root_object = json_value_get_object(root_value);
    if (root_object == NULL) {
        printf("获取 JSON 对象失败\n");
        json_value_free(root_value);
        return;
    }

	strncpy(info->news_type, json_object_get_string(root_object, "f"), sizeof(info->news_type) - 1);

    devices = json_object_get_array(root_object, "d");
    if (devices == NULL) {
        printf("获取 JSON 数组失败\n");
        json_value_free(root_value);
        return;
    }
    first_device = json_array_get_object(devices, 0);
    if (first_device == NULL) {
        printf("获取 JSON 数组中的第一个对象失败\n");
        json_value_free(root_value);
        return;
    }

    strncpy(info->dev_type, json_object_get_string(first_device, "dev"), sizeof(info->dev_type) - 1);
    strncpy(info->eid, json_object_get_string(first_device, "eid"), sizeof(info->eid) - 1);
    strncpy(info->vid, json_object_get_string(first_device, "v"), sizeof(info->vid) - 1);

	printf("消息类型: %s\n", info->news_type);
    printf("设备类型: %s\n", info->dev_type);
    printf("设备号: %s\n", info->eid);
	printf("数值: %s\n", info->vid);

    json_value_free(root_value);

}


/**
 * 解析MQTT的JSON数据(数组多个元素)
 * @param json_string  指向JSON数据的指针
 * @param devices      指向声光电结构体数组的指针
 * @param device_count 指向声光电结构体数组大小的指针
*/
void OneNet_ParseJsonMulti(const char *json_string, Soundlight_Data **devices, size_t *device_count) 
{

    JSON_Value *root_value;
    JSON_Object *root_object;
    JSON_Array *d_array;
    size_t i, count;

    root_value = json_parse_string(json_string);
    if (root_value == NULL) {
        printf("JSON 解析失败\n");
        *devices = NULL;
        *device_count = 0;
        return;
    }
    root_object = json_value_get_object(root_value);
    if (root_object == NULL) {
        printf("获取 JSON 对象失败\n");
        json_value_free(root_value);
        *devices = NULL;
        *device_count = 0;
        return;
    }

    // 获取 "f" 字段
    const char *f_value = json_object_get_string(root_object, "f");
    if (f_value == NULL) {
        printf("获取 \"f\" 字段失败\n");
        json_value_free(root_value);
        *devices = NULL;
        *device_count = 0;
        return;
    }


    // 获取 "d" 数组
    d_array = json_object_get_array(root_object, "d");
    if (d_array == NULL) {
        printf("获取 \"d\" 数组失败\n");
        json_value_free(root_value);
        *devices = NULL;
        *device_count = 0;
        return;
    }
    count = json_array_get_count(d_array);
    *device_count = count;

    // 动态分配结构体数组
    *devices = (Soundlight_Data *)malloc(count * sizeof(Soundlight_Data));
    if (*devices == NULL) {
        printf("内存分配失败\n");
        json_value_free(root_value);
        *device_count = 0;
        return;
    }

    for (i = 0; i < count; ++i) {
        JSON_Object *device_object = json_array_get_object(d_array, i);
        if (device_object == NULL) {
            printf("获取设备对象失败\n");
            free(*devices); // 释放已经分配的内存
            json_value_free(root_value);
            *devices = NULL;
            *device_count = 0;
            return;
        }

        // 将 "f" 字段的值赋给每个设备
        strncpy((*devices)[i].news_type, f_value, sizeof((*devices)[i].news_type) - 1);

        // 获取设备数组中的字段
        const char *dev = json_object_get_string(device_object, "dev");
        const char *eid = json_object_get_string(device_object, "eid");
        const char *vid = json_object_get_string(device_object, "v");

        if (dev != NULL) {
            strncpy((*devices)[i].dev_type, dev, sizeof((*devices)[i].dev_type) - 1);
            (*devices)[i].dev_type[sizeof((*devices)[i].dev_type) - 1] = '\0';
        } else {
            (*devices)[i].dev_type[0] = '\0'; // 确保字符串以空字符结尾
        }

        if (eid != NULL) {
            strncpy((*devices)[i].eid, eid, sizeof((*devices)[i].eid) - 1);
            (*devices)[i].eid[sizeof((*devices)[i].eid) - 1] = '\0';
        } else {
            (*devices)[i].eid[0] = '\0';
        }

        if (vid != NULL) {
            strncpy((*devices)[i].vid, vid, sizeof((*devices)[i].vid) - 1);
            (*devices)[i].vid[sizeof((*devices)[i].vid) - 1] = '\0';
        } else {
            (*devices)[i].vid[0] = '\0';
        }
    }

    json_value_free(root_value);
}


/**
 * 构建JSON数据(单个元素)
 * @param info        指向声光电结构体的指针
 * @return            返回json字符串     
*/
char* construct_json_string(const Soundlight_Data *info) 
{
    // 创建 JSON 结构
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    // 设置根对象的 "f" 字段
    json_object_set_string(root_object, "f", info->news_type);

    // 创建一个数组 "d"
    JSON_Value *d_value = json_value_init_array();
    JSON_Array *d_array = json_value_get_array(d_value);

    // 创建数组中的第一个对象
    JSON_Value *device_value = json_value_init_object();
    JSON_Object *device_object = json_value_get_object(device_value);

    json_object_set_string(device_object, "dev", info->dev_type);
    json_object_set_string(device_object, "eid", info->eid);
    json_object_set_string(device_object, "v", info->vid);

    // 将对象添加到数组中
    json_array_append_value(d_array, device_value);

    // 将数组添加到根对象中
    json_object_set_value(root_object, "d", d_value);

    // 序列化 JSON 对象为字符串
    char *serialized_string = json_serialize_to_string_pretty(root_value);

    // 释放内存（根值将在 后续 中被释放）         json_free_serialized_string();
    json_value_free(root_value);

    return serialized_string;
}

/**
 * 构建JSON数据(多个元素)
 * @param devices          指向声光电结构体数组的指针
 * @param device_count     声光电结构体数组的大小
 * @return                 返回json字符串
*/
char* construct_json_stringMulti(const Soundlight_Data *devices, size_t device_count) 
{
    // 创建 JSON 结构
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    // 所有设备的 "f" 字段值相同，使用第一个设备的 "f" 值
    // json_object_set_string(root_object, "f", devices[0].news_type);
    json_object_set_string(root_object, "f", "sa");

    // 创建一个数组 "d"
    JSON_Value *d_value = json_value_init_array();
    JSON_Array *d_array = json_value_get_array(d_value);

    // 遍历设备数组并将每个设备对象添加到数组中
    for (size_t i = 0; i < device_count; ++i) {
        JSON_Value *device_value = json_value_init_object();
        JSON_Object *device_object = json_value_get_object(device_value);

        json_object_set_string(device_object, "dev", devices[i].dev_type);
        json_object_set_string(device_object, "eid", devices[i].eid);
        json_object_set_string(device_object, "v", devices[i].vid);

        json_array_append_value(d_array, device_value);
    }

    // 将数组添加到根对象中
    json_object_set_value(root_object, "d", d_value);

    // 序列化 JSON 对象为字符串
    char *serialized_string = json_serialize_to_string_pretty(root_value);

    // 释放内存（根值将在 后续 中被释放）            json_free_serialized_string();
    json_value_free(root_value);

    return serialized_string;
}

/**
 * 查询设备类型
 * @param device_type  设备类型
 * @return  返回设备类型对应的数字
*/
int Query_Device_type(const char *device_type)
{

    if(strcmp(device_type, "LightsVsu") == 0)
    {
        return 1;
    }
    else if(strcmp(device_type, "LightsRunsound") == 0)
    {
        return 2;
    }
    else if(strcmp(device_type, "LightsUlaide") == 0)
    {
        return 3;
    }
    else if(strcmp(device_type, "SoundLeyard") == 0)
    {
        return 4;
    }
    else if(strcmp(device_type, "SoundEzpro") == 0)
    {
        return 5;
    }
    else if(strcmp(device_type, "LedAoto") == 0)
    {
        return 6;
    }
    else if(strcmp(device_type, "LedNova") == 0)
    {
        return 7;
    }
    else if(strcmp(device_type, "LedUnilumin") == 0)
    {
        return 8;
    }
    else if(strcmp(device_type, "MultimediaKls") == 0)
    {
        return 9;
    }
    else if(strcmp(device_type, "mode") == 0)
    {
        return 10;
    }
    else 
    {
        return -1;//未知设备类型
    }
}

/**
 * 转换字符串为uint8_t类型
 * @param str  输入的字符串
 * @return  返回转换后的uint8_t类型
*/
static uint8_t transition_Char_array(const char *str)
{
    uint8_t result;

    // 将字符串转换为整数，然后再转换为uint8_t类型
    int value = atoi(str);
    result = (uint8_t)value;

    return result;
}

/**
 * 处理输入字符串,字符串为ＭＱＴＴ消息中的vid
 * @param input  输入的字符串
*/
void handle_input(const char *input) {
    // 提取字符串前缀和数字部分
    char type[7] = {0};  // 初始化类型数组
    int number = 0;
    
    // 遍历字符串，找到类型和数字的分界点
    for (int i = 0; input[i] != '\0'; ++i) {
        if (input[i] >= '0' && input[i] <= '9') {
            strncpy(type, input, i);
            number = atoi(&input[i]);
            break;
        }
    }
    
    // 根据类型和数字部分执行不同的操作
    if (strcmp(type, "video") == 0) {
        handle_video(number);
    } else if (strcmp(type, "ppt") == 0) {
        handle_ppt(number);
    } else if (strcmp(type, "auto") == 0) {
        handle_auto(number);
    } else {
        printf("其他类型: %s\n", input);
        set_Central_control_system_command(input);
    }
}

/**
 * 处理视频类操作
 * @param video_number  视频类操作的数字部分,具体播放哪个视频
*/
static void handle_video(int video_number) {
    // 根据数字部分执行不同的操作
    switch(video_number) {
        case 1:
            set_Central_control_system_command(KLS_PLAY_INSTRUCTION_MOVIE("1"));
            printf("执行video操作1\n");
            break;

        case 2:
            set_Central_control_system_command(KLS_PLAY_INSTRUCTION_MOVIE("2"));
            printf("执行video操作2\n");
            break;

        case 3:
            set_Central_control_system_command(KLS_PLAY_INSTRUCTION_MOVIE("3"));
            printf("执行video操作3\n");
            break;

        default:
            printf("执行video默认操作\n");
            break;
    }
}

/**
 * 处理PPT类操作
 * @param ppt_number  PPT类操作的数字部分,具体播放哪个PPT
*/
static void handle_ppt(int ppt_number) {
    // 根据数字部分执行不同的操作
    switch(ppt_number) {
        case 1:
            set_Central_control_system_command(KLS_PPT_PLAYING("1"));
            printf("执行ppt操作1\n");
            break;

        case 2:
            set_Central_control_system_command(KLS_PPT_PLAYING("2"));
            printf("执行ppt操作2\n");
            break;

        case 3:
            set_Central_control_system_command(KLS_PPT_PLAYING("3"));
            printf("执行ppt操作3\n");
            break;

        default:
            printf("执行ppt默认操作\n");
            break;
    }
}

/**
 * 处理自动播放PPT类操作
 * @param auto_number  自动播放PPT类操作的数字部分,具体播放哪个PPT
*/
static void handle_auto(int auto_number) {
    // 根据数字部分执行不同的操作
    switch(auto_number) {
        case 1:
            set_Central_control_system_command(KLS_PPT_AUTOPLAY("3", "1"));
            printf("执行auto操作1\n");
            break;

        case 2:
            set_Central_control_system_command(KLS_PPT_AUTOPLAY("3", "2"));
            printf("执行auto操作2\n");
            break;

        case 3:
            set_Central_control_system_command(KLS_PPT_AUTOPLAY("3", "3"));
            printf("执行auto操作3\n");
            break;
            
        default:
            printf("执行auto默认操作\n");
            break;
    }
}


/**
 * 根据不同的命令执行对应设备操作
 * @param dev         设备类型
 * @param eid         设备号
 * @param vid         设备控制值
*/
void TICS_Issue_instruction(int dev, const char *eid, const char *vid)
{
    uint8_t eeid;  //16进制，用于发送第几路
    uint8_t vvid;  //16进制，用于发送控制数值
    switch (dev)
    {
    case 1:  //Vsu
        if (strcmp(vid, "100") == 0)
        {
            SetLightingscene_vsu(0x01); 
        }
        else
        {
            SetLightingscene_vsu(0x02);
        }
        break;

    case 2:  //Runsound
        eeid = transition_Char_array(eid); 
        vvid = transition_Char_array(vid);
        // printf("The vvid value is: 0x%x\n", vvid); 
        Setdimming_runshen(0x01, eeid, vvid);
        break;

    case 3:  //Ulaide
        /* code */
        break;

    case 4:  //Leyard
        if (strcmp(vid, "daziran") == 0)
        {
            snprintf(buf_audio, sizeof(buf_audio), "daziran@5F");  //播放
            audio_working();
        }
        else if (strcmp(vid, "stop") == 0)
        {
            snprintf(buf_audio, sizeof(buf_audio), "stop@5F");  //停止
            audio_working();
        }
        else if (strcmp(vid, "turnup") == 0)
        {
            snprintf(buf_audio, sizeof(buf_audio), "V+@5F");  //音量增大
            audio_working();
        }
        else if (strcmp(vid, "turndown") == 0)
        {
            snprintf(buf_audio, sizeof(buf_audio), "V-@5F");  //音量减小
            audio_working();
        }
        break;

    case 5:  //Ezpro
        /* code */
        break;

    case 6:  //Aoto
        if (strcmp(vid, "HDMI1") == 0)
        {
            SetLedinputsource_aoto(0x01, 0x01, 0x00);   //图层２，卡槽２，第１路输入   HDMI1
        }
        else if (strcmp(vid, "HDMI2") == 0)
        {
            SetLedinputsource_aoto(0x01, 0x01, 0x01);
        }
        else if (strcmp(vid, "HDMI3") == 0)
        {
            SetLedinputsource_aoto(0x01, 0x01, 0x02);
        }
        else if (strcmp(vid, "HDMI4") == 0)
        {
            SetLedinputsource_aoto(0x01, 0x01, 0x03);
        }
        break;

    case 7:  //Nova
        /* code */
        break;

    case 8:  //Unilumin
        /* code */
        break;

    case 9:  //Kls
        handle_input(vid);
        break;

    case 10: //模式转换
        if (strcmp(vid, "halfcourtmode") == 0)  //3vs3
        {
            mode_halfcourt_Controls();
        }
        else if (strcmp(vid, "competitionmode") == 0)  //比赛模式
        {
            mode_competition_Controls();
        }
        else if (strcmp(vid, "performancemode") == 0)  //演出模式
        {
            mode_performance_Controls();
        }
        else if (strcmp(vid, "trainmode") == 0)   //训练模式
        {
            mode_train_Controls();
        }
        break;
    
    default:
        break;
    }
}
