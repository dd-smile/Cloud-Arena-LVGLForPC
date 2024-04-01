/*
 * @Description:json转换库
 * @Author: mai
 * @LastEditors: da
 * @Date: 2023-06-16 09:55:13
 * @LastEditTime: 2024-04-01 14:23:14
 * @FilePath: lv_node/src/setting/disposition.c
 */
#include "ui_app.h"


// 判断文件是否存在，若不存在则创建文件
void json_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
    }
    else
    {
        file = fopen(filename, "w");
        if (file)
        {
            char *buf = "{\"brightness\":50,\"btnmid\":0,\"screen_save_time\":60000,\"ScreenSaveid\":0}";
            fprintf(file, "%s", buf);
            fclose(file);
        }
        else
        {
            printf("Failed to open file: %s\n", filename);
        }
    }

}

//读取ｊｓｏｎ数据
void readSettingData(lv_setting_data_t *settingData, const char *jsonFilePath)
{
    JSON_Value *rootValue = json_parse_file(jsonFilePath);
    if (rootValue == NULL)
    {
        printf("解析 JSON 文件失败。\n");
        return;
    }

    JSON_Object *rootObject = json_value_get_object(rootValue);
    if (rootObject == NULL)
    {
        printf("无效的 JSON 文件格式。\n");
        json_value_free(rootValue);
        return;
    }

    // 从 JSON 值中读取设置数据并更新到结构体  
    settingData->brightness       = (uint8_t)json_object_get_number(rootObject, "brightness");
    settingData->Btnmid           = (uint32_t)json_object_get_number(rootObject, "btnmid");
    settingData->screen_save_time = (uint32_t)json_object_get_number(rootObject, "screen_save_time");
    settingData->ScreenSaveid     = (uint32_t)json_object_get_number(rootObject, "ScreenSaveid");

    json_value_free(rootValue);
}

//更新ｊｓｏｎ数据
void updateSettingData(lv_setting_data_t *settingData, const char *jsonFilePath)
{
    json_file(jsonFilePath);
    JSON_Value *rootValue = json_parse_file(jsonFilePath);
    if (rootValue == NULL)
    {
        printf("解析 JSON 文件失败。\n");
        return;
    }

    JSON_Object *rootObject = json_value_get_object(rootValue);
    if (rootObject == NULL)
    {
        printf("无效的 JSON 文件格式。\n");
        json_value_free(rootValue);
        return;
    }

    // 从 JSON 值中更新设置数据结构
    settingData->brightness       = (uint8_t)json_object_get_number(rootObject, "brightness");
    settingData->Btnmid           = (uint32_t)json_object_get_number(rootObject, "btnmid");
    settingData->screen_save_time = (uint32_t)json_object_get_number(rootObject, "screen_save_time");
    settingData->ScreenSaveid     = (uint32_t)json_object_get_number(rootObject, "ScreenSaveid");
    // printfscreenModeTime
    


    json_value_free(rootValue);
}

//保存ｊｓｏｎ格式
void saveSettingData(const lv_setting_data_t *settingData, const char *jsonFilePath)
{
    JSON_Value *rootValue = json_value_init_object();
    JSON_Object *rootObject = json_value_get_object(rootValue);

    // 在 JSON 对象中设置值

    json_object_set_number(rootObject, "brightness"      , settingData->brightness);
    json_object_set_number(rootObject, "btnmid"          , settingData->Btnmid);  
    json_object_set_number(rootObject, "screen_save_time", settingData->screen_save_time);
    json_object_set_number(rootObject, "ScreenSaveid"    , settingData->ScreenSaveid);

    // 将 JSON 对象保存到文件
    if (json_serialize_to_file(rootValue, jsonFilePath) != JSONSuccess)
    {
        printf("保存 JSON 文件失败。\n");
    }

    json_value_free(rootValue);
}

//打印ｊｓｏｎ格式
void print_setting(lv_setting_data_t *settingData)
{
    printf("setting: %u, %u, %u, %u, %u,\n", settingData->brightness, settingData->Btnmid, settingData->screen_save_time, settingData->ScreenSaveid);
}

