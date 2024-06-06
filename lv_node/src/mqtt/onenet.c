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

#define DEVID		"avant_ycg"

#define MQTT_SERVER_IP 	"112.74.105.251"


static int mqtt_fd;    //连接ｍｑｔｔ套接字

Soundlight_Data soundtest;
unsigned char dataPtr[1024];
char *sub_topics[] = {"/mytopic/sub","/mytopic/task"};   //订阅多个主题

static void *mqttConnection(void *parg)
{

	int mqtt_red = -1;
	fd_set readfds;
    struct timeval tv;
	int retval;

	// 1. 创建通信的套接字
  	mqtt_fd = createSocket();

  	// 2. 连接服务器
  	connectToHost(mqtt_fd, MQTT_SERVER_IP, 1883);
	if (socketconnected(mqtt_fd) != 0)
	{
		OneNet_DevLink();   //连接ＭＱＴＴ服务器

		OneNet_Subscribe(sub_topics, 2);
	}

	while (1)
	{
		// 初始化文件描述符集合
        FD_ZERO(&readfds);
        FD_SET(mqtt_fd, &readfds);

        // 设置超时时间
        tv.tv_sec = 5;  // 设置秒
    	tv.tv_usec = 0; // 设置微秒

        retval = select(mqtt_fd + 1, &readfds, NULL, NULL, &tv);

		if (retval > 0)
		{
			if (FD_ISSET(mqtt_fd, &readfds)) 
			{
				dataPtr[0] = '\0';
				int len = recv(mqtt_fd, dataPtr, 1024, 0);

				if (len < 0)
				{
					perror("recv failed");
					break;
				}else if (len == 0)
				{
					printf("connect close\n");
					break;
				}

				dataPtr[len] = '\0';

				if (len > 0)
				{
					OneNet_RevPro(dataPtr);
					usleep(50 * 1000);
				}
			}
		}
		else if (retval == 0)
		{
			printf("select timeout\n");
		}
		else
		{
			perror("select failed");
			break;
		} 


		if (socketconnected(mqtt_fd) == 0)
		{
			printf("进行重连尝试\n");      //要正常检测重连机制，需要修改linux内核参数tcp_retries2  /proc/sys/net/ipv4/tcp_retries2
			closeSocket(mqtt_fd);
			mqtt_fd = createSocket();
			mqtt_red = connectToHost(mqtt_fd, MQTT_SERVER_IP, 1883);
			if (mqtt_red != -1)
			{
				OneNet_DevLink();

				OneNet_Subscribe(sub_topics, 2);
			}
		}

		usleep(1000 * 1000);
	}
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

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};

	unsigned char sCount = 3;

	if (MQTT_PacketPing(&mqttPacket))    //组包
		return;
	
	while (sCount--)
	{
		if (socketconnected(mqtt_fd) != 0)
		{
			write(mqtt_fd, mqttPacket._data, mqttPacket._len);	
		}

		if (MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_PINGRESP)
		{
			printf("Tips:	HeartBeat OK\r\n");
			
			break;
		}
		else
		{
			// printf("Check Device\r\n");
		}

		usleep(50 * 2000);

	}

	// printf("删包\r\n");
	MQTT_DeleteBuffer(&mqttPacket);

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
	
	short result = -1;
	
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
					
					write(mqtt_fd, mqttPacket._data, mqttPacket._len);			//回复命令
					MQTT_DeleteBuffer(&mqttPacket);									//删包
				}
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0)
			{
				// printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
				// 													cmdid_topic, topic_len, req_payload, req_len);

				Soundlight_Data *devices = NULL;  //创建 Soundlight_Data 数组
    			size_t device_count = 0;
				int dev = 0;
				//解析数据包		
				OneNet_ParseJsonMulti(req_payload, &devices, &device_count);

				// 输出解析结果  进行命令下发执行,同时执行回复命令
				for (size_t i = 0; i < device_count; ++i) {
					printf("Device %zu:\n", i + 1);
					printf("  Device: %s\n", devices[i].dev_type);
					dev = Query_Device_type(devices[i].dev_type);
					TICS_Issue_instruction(dev, devices[i].eid, devices[i].vid);
					printf("  Dev: %d\n", dev);
					printf("  EID: %s\n", devices[i].eid);
					printf("  Version: %s\n", devices[i].vid);
				}			
				char *json_public = construct_json_stringMulti(devices, device_count);	
				OneNet_Publish("/mytest/ycg", json_public);						
				
				//释放内存
				json_free_serialized_string(json_public);
				free(devices);
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
			// result = -1;
		break;
	}
	
	// ESP8266_Clear();									//清空缓存
	
	// if(result == -1)
	// 	return;

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
