/*
 * @Description: MQTT通用控制,调用MqttKit库
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-10-25 11:28:17
 * @LastEditTime: 2024-04-01 15:59:05
 * @FilePath: lv_node/src/mqtt/onenet.c
 */
#include "ui_app.h"

#define PROID		"admin"

#define AUTH_INFO	"public"

#define DEVID		"avant_test"

#define MQTT_SERVER_IP 	"112.74.105.251"


static int mqtt_fd;    //连接ｍｑｔｔ套接字
static int reconnect_mqtt = 2; 
static int heart_count = 0;
static char heart_buf[256];

unsigned char dataPtr[1024];
char *sub_topics[] = {"/mytopic/sub","/mytopic/task"};   //订阅多个主题

static void *mqttConnection(void *parg)
{
	// 1. 创建通信的套接字
  	mqtt_fd = createSocket();

  	// 2. 连接服务器
  	connectToHost(mqtt_fd, MQTT_SERVER_IP, 1883);
	if (socketconnected(mqtt_fd) != 0)
	{
		OneNet_DevLink();   //连接ＭＱＴＴ服务器

		OneNet_Subscribe(sub_topics, 2);
	}

	//接收MQTT消息
	while (1)
	{
		dataPtr[0] = '\0';
		int len = recv(mqtt_fd, dataPtr, 1024, 0);
		dataPtr[len] = '\0';

		if (len > 0)
		{
			// printf("收到订阅消息: %s\n", dataPtr);
			OneNet_RevPro(dataPtr);
			usleep(50 * 1000);
		}
	}
}

void OneNet_ParseJsonNow(char *msg, Soundlight_Data *data)
{
	JSON_Value  *root_value;
    JSON_Array  *results;

	root_value = json_parse_string(msg);
	if (root_value == NULL) {
  		printf("JSON 文件为空\n");
  	}
	JSON_Object *root_object = json_value_get_object(root_value);

	// int code = json_object_get_number(root_object, "code");
  	// printf("code is %d\n", code);

}

/**
 * 创建MQTT连接
 * */
void connect_mqtt()
{
	pthread_t mqtt_thread;
	pthread_create(&mqtt_thread, NULL, mqttConnection, NULL);
}

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包
	
	_Bool status = 1;
	
	
	if (MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 60, 1, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		write(mqtt_fd, mqttPacket._data, mqttPacket._len);			//上传平台
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
		printf("连接MQTT服务器成功\n");
	}
	else
		//UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

//==========================================================
//	函数名称：	OneNet_HeartBeat
//
//	函数功能：	心跳检测
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_HeartBeat(void)
{

	unsigned char sCount = 3;
	int mqtt_red = -1;
	
	while(sCount--)
	{
		if (socketconnected(mqtt_fd) == 0)
		{
			reconnect_mqtt--;
			if (reconnect_mqtt == 0)
			{
				printf("进行重连尝试\n");      //要正常检测重连机制，需要修改linux内核参数tcp_retries2  /proc/sys/net/ipv4/tcp_retries2
				reconnect_mqtt = 2;
				closeSocket(mqtt_fd);
				mqtt_fd = createSocket();
				mqtt_red = connectToHost(mqtt_fd, MQTT_SERVER_IP, 1883);
				if(mqtt_red != -1)
				{
					OneNet_DevLink();
					break;
				}

			}
			
		}
		else
		{
			printf("正常发送心跳\n");
			heart_count++;
			if (heart_count >= 1000)
			{
				heart_count = 0;
			}
			snprintf(heart_buf, sizeof(heart_buf), "{\"msgId\":%d,\"devName\":%s}",heart_count, DEVID);
			OneNet_Publish("/myherat/ycg", heart_buf);
			break;
		}

	}

}

//==========================================================
//	函数名称：	OneNet_Subscribe
//
//	函数功能：	订阅
//
//	入口参数：	topics：订阅的topic
//				topic_cnt：topic个数
//
//	返回参数：	SEND_TYPE_OK-成功	SEND_TYPE_SUBSCRIBE-需要重发
//
//	说明：		
//==========================================================
void OneNet_Subscribe(const char *topics[], unsigned char topic_cnt)
{
	
	unsigned char i = 0;
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
	for (; i < topic_cnt; i++)
		printf("Subscribe Topic: %s\r\n", topics[i]);
	
	if (MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, topics, topic_cnt, &mqttPacket) == 0)
	{
		write(mqtt_fd, mqttPacket._data, mqttPacket._len);				//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}

}

//==========================================================
//	函数名称：	OneNet_Publish
//
//	函数功能：	发布消息
//
//	入口参数：	topic：发布的主题
//				msg：消息内容
//
//	返回参数：	SEND_TYPE_OK-成功	SEND_TYPE_PUBLISH-需要重送
//
//	说明：		
//==========================================================
void OneNet_Publish(const char *topic, const char *msg)
{

	ssize_t test_flag = 0;
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
	//printf("Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if (MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) == 0  && socketconnected(mqtt_fd) != 0)
	{
		lv_label_set_text(g_mqtt_detection, "");
		test_flag = write(mqtt_fd, mqttPacket._data, mqttPacket._len);					//向平台发送订阅请求
		// printf("Publish testFlag: %d\r\n", test_flag);
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}
	else
	{
		lv_label_set_text(g_mqtt_detection, "MQTT连接失败");
	}

}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	// cJSON *json, *json_value;
	// cJSON *json_task, *task_value;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//命令下发
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
			if(result == 0)
			{
				printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
				{
					printf("Tips:	Send CmdResp\r\n");
					
					//ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
					MQTT_DeleteBuffer(&mqttPacket);									//删包
				}
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0)
			{
				printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
																	cmdid_topic, topic_len, req_payload, req_len);
				 
				// if((strcmp(cmdid_topic,led_topic) == 0))    //判断是否是led灯开关主题
				// {
				// 	//解析数据包
				// 	json = cJSON_Parse(req_payload);
				// 	if(!json)printf("Error before: [%s]\n",cJSON_GetErrorPtr());
				// 	else
				// 	{
				// 		json_value = cJSON_GetObjectItem(json,"LED_SW");
				// 		printf("json_value = %d\r\n",json_value);
				// 		if(json_value->valueint)//json_value > 0 且为整型
				// 		{
				// 			//打开led
				// 			LED2_ON();
				// 			led_switch = 1;  //用于开关喂食系统的标志位
				// 		}
				// 		else
				// 		{
				// 			//关闭led
				// 			LED2_OFF();
				// 			led_switch = 0;
				// 		}
				// 	}
				// 	//删json包, 防止内存炸
				// 	cJSON_Delete(json);
				// 	delay_ms(500);
				// }else if((strcmp(cmdid_topic,task_topic) == 0))    //判断是否是task开关主题
				// {
				// 	//解析数据包
				// 	json_task = cJSON_Parse(req_payload);
				// 	if(!json_task)printf("Error before: [%s]\n",cJSON_GetErrorPtr());
				// 	else
				// 	{
				// 		task_value = cJSON_GetObjectItem(json_task,"TASK_SW");
				// 		printf("task_value = %d\r\n",task_value);
				// 		if(json_value->valueint)//json_value > 0 且为整型
				// 		{
				// 			//UsartPrintf(USART_DEBUG,"kkkkkkkkkk json_value = %d\r\n",json_value);
				// 			task_switch = 1;   //如果有收到TASK_SW,且值为1就把task开关置为1
				// 		}
				// 	//删json包, 防止内存炸
				// 	cJSON_Delete(json_task);
				// 	delay_ms(500);
				// 	}
				// }

			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				printf("Tips:	MQTT Publish Send OK\r\n");
			
		break;
			
		case MQTT_PKT_PUBREC:														//发送Publish消息，平台回复的Rec，设备需回复Rel消息
		
			if(MQTT_UnPacketPublishRec(cmd) == 0)
			{
				printf("Tips:	Rev PublishRec\r\n");
				if(MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishRel\r\n");
					write(mqtt_fd, mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
			
		case MQTT_PKT_PUBREL:														//收到Publish消息，设备回复Rec后，平台回复的Rel，设备需再回复Comp
			
			if(MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
			{
				printf("Tips:	Rev PublishRel\r\n");
				if(MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishComp\r\n");
					write(mqtt_fd, mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
		
		case MQTT_PKT_PUBCOMP:														//发送Publish消息，平台返回Rec，设备回复Rel，平台再返回的Comp
		
			if(MQTT_UnPacketPublishComp(cmd) == 0)
			{
				printf("Tips:	Rev PublishComp\r\n");
			}
		
		break;
			
		case MQTT_PKT_SUBACK:														//发送Subscribe消息的Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
				printf("Tips:	MQTT Subscribe OK\r\n");
			else
				printf("Tips:	MQTT Subscribe Err\r\n");
		
		break;
			
		case MQTT_PKT_UNSUBACK:														//发送UnSubscribe消息的Ack
		
			if(MQTT_UnPacketUnSubscribe(cmd) == 0)
				printf("Tips:	MQTT UnSubscribe OK\r\n");
			else
				printf("Tips:	MQTT UnSubscribe Err\r\n");
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	// ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
