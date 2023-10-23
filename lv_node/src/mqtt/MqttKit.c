/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	MqttKit.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2018-04-27
	*
	*	版本： 		V1.6
	*
	*	说明： 		MQTT协议
	*
	*	修改记录：	V1.1：解决MQTT_PacketSubscribe订阅不为2个topic
	*						个数时协议错误的bug
	*				V1.2：修复MQTT_PacketCmdResp的bug
	*				V1.3：将strncpy替换为memcpy，解决潜在bug
	*				V1.4：修复	MQTT_PacketPublishAck
	*							MQTT_PacketPublishRel
	*							函数封包错误的bug
	*				V1.5：增加	MQTT_UnPacketCmd
	*							MQTT_UnPacketPublish
	*							接口对消息内容长度的提取参数
	*				V1.6：增加二进制文件上传接口
	************************************************************
	************************************************************
	************************************************************
**/

#include "ui_app.h"


#define CMD_TOPIC_PREFIX		"$creq"


//==========================================================
//	函数名称：	EDP_NewBuffer
//
//	函数功能：	申请内存
//
//	入口参数：	edpPacket：包结构体
//				size：大小
//
//	返回参数：	无
//
//	说明：		1.可使用动态分配来分配内存
//				2.可使用局部或全局数组来指定内存
//==========================================================
void MQTT_NewBuffer(MQTT_PACKET_STRUCTURE *mqttPacket, uint32 size)
{
	
	uint32 i = 0;

	if(mqttPacket->_data == NULL)
	{
		mqttPacket->_memFlag = MEM_FLAG_ALLOC;
		
		mqttPacket->_data = (uint8 *)MQTT_MallocBuffer(size);
		if(mqttPacket->_data != NULL)
		{
			mqttPacket->_len = 0;
			
			mqttPacket->_size = size;
			
			for(; i < mqttPacket->_size; i++)
				mqttPacket->_data[i] = 0;
		}
	}
	else
	{
		mqttPacket->_memFlag = MEM_FLAG_STATIC;
		
		for(; i < mqttPacket->_size; i++)
			mqttPacket->_data[i] = 0;
		
		mqttPacket->_len = 0;
		
		if(mqttPacket->_size < size)
			mqttPacket->_data = NULL;
	}

}

//==========================================================
//	�������ƣ�	MQTT_DeleteBuffer
//
//	�������ܣ�	�ͷ������ڴ�
//
//	��ڲ�����	edpPacket�����ṹ��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void MQTT_DeleteBuffer(MQTT_PACKET_STRUCTURE *mqttPacket)
{

	if(mqttPacket->_memFlag == MEM_FLAG_ALLOC)
		MQTT_FreeBuffer(mqttPacket->_data);
	
	mqttPacket->_data = NULL;
	mqttPacket->_len = 0;
	mqttPacket->_size = 0;
	mqttPacket->_memFlag = MEM_FLAG_NULL;

}

int32 MQTT_DumpLength(size_t len, uint8 *buf)
{
	
	int32 i = 0;
	
	for(i = 1; i <= 4; ++i)
	{
		*buf = len % 128;
		len >>= 7;
		if(len > 0)
		{
			*buf |= 128;
			++buf;
		}
		else
		{
			return i;
		}
	}

	return -1;
}

int32 MQTT_ReadLength(const uint8 *stream, int32 size, uint32 *len)
{
	
	int32 i;
	const uint8 *in = stream;
	uint32 multiplier = 1;

	*len = 0;
	for(i = 0; i < size; ++i)
	{
		*len += (in[i] & 0x7f) * multiplier;

		if(!(in[i] & 0x80))
		{
			return i + 1;
		}

		multiplier <<= 7;
		if(multiplier >= 2097152)		//128 * *128 * *128
		{
			return -2;					// error, out of range
		}
	}

	return -1;							// not complete

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketRecv
//
//	�������ܣ�	MQTT���ݽ��������ж�
//
//	��ڲ�����	dataPtr�����յ�����ָ��
//
//	���ز�����	0-�ɹ�		����-ʧ��ԭ��
//
//	˵����		
//==========================================================
uint8 MQTT_UnPacketRecv(uint8 *dataPtr)
{
	
	uint8 status = 255;
	uint8 type = dataPtr[0] >> 4;				//���ͼ��
	
	if(type < 1 || type > 14)
		return status;
	
	if(type == MQTT_PKT_PUBLISH)
	{
		uint8 *msgPtr;
		uint32 remain_len = 0;
		
		msgPtr = dataPtr + MQTT_ReadLength(dataPtr + 1, 4, &remain_len) + 1;
		
		if(remain_len < 2 || dataPtr[0] & 0x01)					//retain
			return 255;
		
		if(remain_len < ((uint16)msgPtr[0] << 8 | msgPtr[1]) + 2)
			return 255;
		
		if(strstr((int8 *)msgPtr + 2, CMD_TOPIC_PREFIX) != NULL)	//����������·�
			status = MQTT_PKT_CMD;
		else
			status = MQTT_PKT_PUBLISH;
	}
	else
		status = type;
	
	return status;

}

//==========================================================
//	函数名称：	MQTT_PacketConnect
//
//	函数功能：	连接消息组包
//
//	入口参数：	user：用户名：产品ID
//				password：密码：鉴权信息或apikey
//				devid：设备ID
//				cTime：连接保持时间
//				clean_session：离线消息清除标志
//				qos：重发标志
//				will_topic：异常离线topic
//				will_msg：异常离线消息
//				will_retain：消息推送标志
//				mqttPacket：包指针
//
//	返回参数：	0-成功		其他-失败
//
//	说明：		
//==========================================================
uint8 MQTT_PacketConnect(const int8 *user, const int8 *password, const int8 *devid,
						uint16 cTime, uint1 clean_session, uint1 qos,
						const int8 *will_topic, const int8 *will_msg, int32 will_retain,
						MQTT_PACKET_STRUCTURE *mqttPacket)
{
	
	uint8 flags = 0;
	uint8 will_topic_len = 0;
	uint16 total_len = 15;
	int16 len = 0, devid_len = strlen(devid);
	
	if(!devid)
		return 1;
	
	total_len += devid_len + 2;
	
	//断线后，是否清理离线消息：1-清理	0-不清理--------------------------------------------
	if(clean_session)
	{
		flags |= MQTT_CONNECT_CLEAN_SESSION;
	}
	
	//异常掉线情况下，服务器发布的topic------------------------------------------------------
	if(will_topic)
	{
		flags |= MQTT_CONNECT_WILL_FLAG;
		will_topic_len = strlen(will_topic);
		total_len += 4 + will_topic_len + strlen(will_msg);
	}
	
	//qos级别--主要用于PUBLISH（发布态）消息的，保证消息传递的次数-----------------------------
	switch((unsigned char)qos)
	{
		case MQTT_QOS_LEVEL0:
			flags |= MQTT_CONNECT_WILL_QOS0;							//最多一次
		break;
		
		case MQTT_QOS_LEVEL1:
			flags |= (MQTT_CONNECT_WILL_FLAG | MQTT_CONNECT_WILL_QOS1);	//最少一次
		break;
		
		case MQTT_QOS_LEVEL2:
			flags |= (MQTT_CONNECT_WILL_FLAG | MQTT_CONNECT_WILL_QOS2);	//只有一次
		break;
		
		default:
		return 2;
	}
	
	//主要用于PUBLISH(发布态)的消息，表示服务器要保留这次推送的信息，如果有新的订阅者出现，就把这消息推送给它。如果不设那么推送至当前订阅的就释放了
	if(will_retain)
	{
		flags |= (MQTT_CONNECT_WILL_FLAG | MQTT_CONNECT_WILL_RETAIN);
	}
	
	//账号为空 密码为空---------------------------------------------------------------------
	if(!user || !password)
	{
		return 3;
	}
	flags |= MQTT_CONNECT_USER_NAME | MQTT_CONNECT_PASSORD;
	
	total_len += strlen(user) + strlen(password) + 4;
	
	//分配内存-----------------------------------------------------------------------------
	MQTT_NewBuffer(mqttPacket, total_len);
	if(mqttPacket->_data == NULL)
		return 4;
	
	memset(mqttPacket->_data, 0, total_len);
	
/*************************************固定头部***********************************************/
	
	//固定头部----------------------连接请求类型---------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_CONNECT << 4;
	
	//固定头部----------------------剩余长度值-----------------------------------------------
	len = MQTT_DumpLength(total_len - 5, mqttPacket->_data + mqttPacket->_len);
	if(len < 0)
	{
		MQTT_DeleteBuffer(mqttPacket);
		return 5;
	}
	else
		mqttPacket->_len += len;
	
/*************************************可变头部***********************************************/
	
	//可变头部----------------------协议名长度 和 协议名--------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 0;
	mqttPacket->_data[mqttPacket->_len++] = 4;
	mqttPacket->_data[mqttPacket->_len++] = 'M';
	mqttPacket->_data[mqttPacket->_len++] = 'Q';
	mqttPacket->_data[mqttPacket->_len++] = 'T';
	mqttPacket->_data[mqttPacket->_len++] = 'T';
	
	//可变头部----------------------protocol level 4-----------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 4;
	
	//可变头部----------------------连接标志(该函数开头处理的数据)-----------------------------
    mqttPacket->_data[mqttPacket->_len++] = flags;
	
	//可变头部----------------------保持连接的时间(秒)----------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(cTime);
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(cTime);
	 
/*************************************消息体************************************************/

	//消息体----------------------------devid长度、devid-------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(devid_len);
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(devid_len);
	
	strncat((int8 *)mqttPacket->_data + mqttPacket->_len, devid, devid_len);
	mqttPacket->_len += devid_len;
	
	//消息体----------------------------will_flag 和 will_msg---------------------------------
	if(flags & MQTT_CONNECT_WILL_FLAG)
	{
		unsigned short mLen = 0;
		
		if(!will_msg)
			will_msg = "";
		
		mLen = strlen(will_topic);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(mLen);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(mLen);
		strncat((int8 *)mqttPacket->_data + mqttPacket->_len, will_topic, mLen);
		mqttPacket->_len += mLen;
		
		mLen = strlen(will_msg);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(mLen);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(mLen);
		strncat((int8 *)mqttPacket->_data + mqttPacket->_len, will_msg, mLen);
		mqttPacket->_len += mLen;
	}
	
	//消息体----------------------------use---------------------------------------------------
	if(flags & MQTT_CONNECT_USER_NAME)
	{
		unsigned short user_len = strlen(user);
		
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(user_len);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(user_len);
		strncat((int8 *)mqttPacket->_data + mqttPacket->_len, user, user_len);
		mqttPacket->_len += user_len;
	}

	//消息体----------------------------password----------------------------------------------
	if(flags & MQTT_CONNECT_PASSORD)
	{
		unsigned short psw_len = strlen(password);
		
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(psw_len);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(psw_len);
		strncat((int8 *)mqttPacket->_data + mqttPacket->_len, password, psw_len);
		mqttPacket->_len += psw_len;
	}

	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_PacketDisConnect
//
//	�������ܣ�	�Ͽ�������Ϣ���
//
//	��ڲ�����	mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_PacketDisConnect(MQTT_PACKET_STRUCTURE *mqttPacket)
{

	MQTT_NewBuffer(mqttPacket, 2);
	if(mqttPacket->_data == NULL)
		return 1;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_DISCONNECT << 4;
	
	//�̶�ͷ��----------------------ʣ�೤��ֵ-----------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 0;
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketConnectAck
//
//	�������ܣ�	������Ϣ���
//
//	��ڲ�����	rev_data�����յ�����
//
//	���ز�����	1��255-ʧ��		����-ƽ̨�ķ�����
//
//	˵����		
//==========================================================
uint8 MQTT_UnPacketConnectAck(uint8 *rev_data)
{

	if(rev_data[1] != 2)
		return 1;
	
	if(rev_data[2] == 0 || rev_data[2] == 1)
		return rev_data[3];
	else
		return 255;

}

//==========================================================
//	�������ƣ�	MQTT_PacketSaveData
//
//	�������ܣ�	���ݵ��ϴ����
//
//	��ڲ�����	devid���豸ID(��Ϊ��)
//				send_buf��json����buf
//				send_len��json�ܳ�
//				type_bin_head��bin�ļ�����Ϣͷ
//				type������
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_PacketSaveData(const int8 *devid, int16 send_len, int8 *type_bin_head, uint8 type, MQTT_PACKET_STRUCTURE *mqttPacket)
{

	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, "$dp", NULL, send_len + 3, MQTT_QOS_LEVEL1, 0, 1, mqttPacket) == 0)
	{
		mqttPacket->_data[mqttPacket->_len++] = type;					//����
		
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(send_len);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(send_len);
	}
	else
		return 1;
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_PacketSaveBinData
//
//	�������ܣ�	Ϊ��ֹ�ļ��ϴ����
//
//	��ڲ�����	name������������
//				file_len���ļ�����
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_PacketSaveBinData(const int8 *name, int16 file_len, MQTT_PACKET_STRUCTURE *mqttPacket)
{

	uint1 result = 1;
	int8 *bin_head = NULL;
	uint8 bin_head_len = 0;
	int8 *payload = NULL;
	int32 payload_size = 0;
	
	bin_head = (int8 *)MQTT_MallocBuffer(13 + strlen(name));
	if(bin_head == NULL)
		return result;
	
	sprintf(bin_head, "{\"ds_id\":\"%s\"}", name);
	
	bin_head_len = strlen(bin_head);
	payload_size = 7 + bin_head_len + file_len;
	
	payload = (int8 *)MQTT_MallocBuffer(payload_size - file_len);
	if(payload == NULL)
	{
		MQTT_FreeBuffer(bin_head);
		
		return result;
	}
	
	payload[0] = 2;						//����
		
	payload[1] = MOSQ_MSB(bin_head_len);
	payload[2] = MOSQ_LSB(bin_head_len);
	
	memcpy(payload + 3, bin_head, bin_head_len);
	
	payload[bin_head_len + 3] = (file_len >> 24) & 0xFF;
	payload[bin_head_len + 4] = (file_len >> 16) & 0xFF;
	payload[bin_head_len + 5] = (file_len >> 8) & 0xFF;
	payload[bin_head_len + 6] = file_len & 0xFF;
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, "$dp", payload, payload_size, MQTT_QOS_LEVEL1, 0, 1, mqttPacket) == 0)
		result = 0;
	
	MQTT_FreeBuffer(bin_head);
	MQTT_FreeBuffer(payload);
	
	return result;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketCmd
//
//	�������ܣ�	�����·����
//
//	��ڲ�����	rev_data�����յ�����ָ��
//				cmdid��cmdid-uuid
//				req������
//
//	���ز�����	0-�ɹ�		����-ʧ��ԭ��
//
//	˵����		
//==========================================================
uint8 MQTT_UnPacketCmd(uint8 *rev_data, int8 **cmdid, int8 **req, uint16 *req_len)
{

	int8 *dataPtr = strchr((int8 *)rev_data + 6, '/');	//��6������ͷ��Ϣ
	
	uint32 remain_len = 0;
	
	if(dataPtr == NULL)									//δ�ҵ�'/'
		return 1;
	dataPtr++;											//����'/'
	
	MQTT_ReadLength(rev_data + 1, 4, &remain_len);		//��ȡʣ���ֽ�
	
	*cmdid = (int8 *)MQTT_MallocBuffer(37);				//cmdid�̶�36�ֽڣ������һ����������λ��
	if(*cmdid == NULL)
		return 2;
	
	memset(*cmdid, 0, 37);								//ȫ������
	memcpy(*cmdid, (const int8 *)dataPtr, 36);			//����cmdid
	dataPtr += 36;
	
	*req_len = remain_len - 44;							//����� = ʣ�೤��(remain_len) - 2 - 5($creq) - 1(\) - cmdid����
	*req = (int8 *)MQTT_MallocBuffer(*req_len + 1);		//���������+1
	if(*req == NULL)
	{
		MQTT_FreeBuffer(*cmdid);
		return 3;
	}
	
	memset(*req, 0, *req_len + 1);						//����
	memcpy(*req, (const int8 *)dataPtr, *req_len);		//��������
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_PacketCmdResp
//
//	�������ܣ�	����ظ����
//
//	��ڲ�����	cmdid��cmdid
//				req������
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_PacketCmdResp(const int8 *cmdid, const int8 *req, MQTT_PACKET_STRUCTURE *mqttPacket)
{
	
	uint16 cmdid_len = strlen(cmdid);
	uint16 req_len = strlen(req);
	_Bool status = 0;
	
	int8 *payload = MQTT_MallocBuffer(cmdid_len + 7);
	if(payload == NULL)
		return 1;
	
	memset(payload, 0, cmdid_len + 7);
	memcpy(payload, "$crsp/", 6);
	strncat(payload, cmdid, cmdid_len);

	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, payload, req, strlen(req), MQTT_QOS_LEVEL0, 0, 1, mqttPacket) == 0)
		status = 0;
	else
		status = 1;
	
	MQTT_FreeBuffer(payload);
	
	return status;

}

//==========================================================
//	�������ƣ�	MQTT_PacketSubscribe
//
//	�������ܣ�	Subscribe��Ϣ���
//
//	��ڲ�����	pkt_id��pkt_id
//				qos����Ϣ�ط�����
//				topics�����ĵ���Ϣ
//				topics_cnt�����ĵ���Ϣ����
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		����-ʧ��
//
//	˵����		
//==========================================================
uint8 MQTT_PacketSubscribe(uint16 pkt_id, enum MqttQosLevel qos, const int8 *topics[], uint8 topics_cnt, MQTT_PACKET_STRUCTURE *mqttPacket)
{
	
	uint32 topic_len = 0, remain_len = 0;
	int16 len = 0;
	uint8 i = 0;
	
	if(pkt_id == 0)
		return 1;
	
	//����topic����-------------------------------------------------------------------------
	for(; i < topics_cnt; i++)
	{
		if(topics[i] == NULL)
			return 2;
		
		topic_len += strlen(topics[i]);
	}
	
	//2 bytes packet id + topic filter(2 bytes topic + topic length + 1 byte reserve)------
	remain_len = 2 + 3 * topics_cnt + topic_len;
	
	//�����ڴ�------------------------------------------------------------------------------
	MQTT_NewBuffer(mqttPacket, remain_len + 5);
	if(mqttPacket->_data == NULL)
		return 3;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_SUBSCRIBE << 4 | 0x02;
	
	//�̶�ͷ��----------------------ʣ�೤��ֵ-----------------------------------------------
	len = MQTT_DumpLength(remain_len, mqttPacket->_data + mqttPacket->_len);
	if(len < 0)
	{
		MQTT_DeleteBuffer(mqttPacket);
		return 4;
	}
	else
		mqttPacket->_len += len;
	
/*************************************payload***********************************************/
	
	//payload----------------------pkt_id---------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(pkt_id);
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(pkt_id);
	
	//payload----------------------topic_name-----------------------------------------------
	for(i = 0; i < topics_cnt; i++)
	{
		topic_len = strlen(topics[i]);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(topic_len);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(topic_len);
		
		strncat((int8 *)mqttPacket->_data + mqttPacket->_len, topics[i], topic_len);
		mqttPacket->_len += topic_len;
		
		mqttPacket->_data[mqttPacket->_len++] = qos & 0xFF;
	}

	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketSubscrebe
//
//	�������ܣ�	Subscribe�Ļظ���Ϣ���
//
//	��ڲ�����	rev_data�����յ�����Ϣ
//
//	���ز�����	0-�ɹ�		����-ʧ��
//
//	˵����		
//==========================================================
uint8 MQTT_UnPacketSubscribe(uint8 *rev_data)
{
	
	uint8 result = 255;

	if(rev_data[2] == MOSQ_MSB(MQTT_SUBSCRIBE_ID) && rev_data[3] == MOSQ_LSB(MQTT_SUBSCRIBE_ID))
	{
		switch(rev_data[4])
		{
			case 0x00:
			case 0x01:
			case 0x02:
				//MQTT Subscribe OK
				result = 0;
			break;
			
			case 0x80:
				//MQTT Subscribe Failed
				result = 1;
			break;
			
			default:
				//MQTT Subscribe UnKnown Err
				result = 2;
			break;
		}
	}
	
	return result;

}

//==========================================================
//	�������ƣ�	MQTT_PacketUnSubscribe
//
//	�������ܣ�	UnSubscribe��Ϣ���
//
//	��ڲ�����	pkt_id��pkt_id
//				qos����Ϣ�ط�����
//				topics�����ĵ���Ϣ
//				topics_cnt�����ĵ���Ϣ����
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		����-ʧ��
//
//	˵����		
//==========================================================
uint8 MQTT_PacketUnSubscribe(uint16 pkt_id, const int8 *topics[], uint8 topics_cnt, MQTT_PACKET_STRUCTURE *mqttPacket)
{
	
	uint32 topic_len = 0, remain_len = 0;
	int16 len = 0;
	uint8 i = 0;
	
	if(pkt_id == 0)
		return 1;
	
	//����topic����-------------------------------------------------------------------------
	for(; i < topics_cnt; i++)
	{
		if(topics[i] == NULL)
			return 2;
		
		topic_len += strlen(topics[i]);
	}
	
	//2 bytes packet id, 2 bytes topic length + topic + 1 byte reserve---------------------
	remain_len = 2 + (topics_cnt << 1) + topic_len;
	
	//�����ڴ�------------------------------------------------------------------------------
	MQTT_NewBuffer(mqttPacket, remain_len + 5);
	if(mqttPacket->_data == NULL)
		return 3;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_UNSUBSCRIBE << 4 | 0x02;
	
	//�̶�ͷ��----------------------ʣ�೤��ֵ-----------------------------------------------
	len = MQTT_DumpLength(remain_len, mqttPacket->_data + mqttPacket->_len);
	if(len < 0)
	{
		MQTT_DeleteBuffer(mqttPacket);
		return 4;
	}
	else
		mqttPacket->_len += len;
	
/*************************************payload***********************************************/
	
	//payload----------------------pkt_id---------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(pkt_id);
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(pkt_id);
	
	//payload----------------------topic_name-----------------------------------------------
	for(i = 0; i < topics_cnt; i++)
	{
		topic_len = strlen(topics[i]);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(topic_len);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(topic_len);
		
		strncat((int8 *)mqttPacket->_data + mqttPacket->_len, topics[i], topic_len);
		mqttPacket->_len += topic_len;
	}

	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketUnSubscribe
//
//	�������ܣ�	UnSubscribe�Ļظ���Ϣ���
//
//	��ڲ�����	rev_data�����յ�����Ϣ
//
//	���ز�����	0-�ɹ�		����-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_UnPacketUnSubscribe(uint8 *rev_data)
{
	
	uint1 result = 1;

	if(rev_data[2] == MOSQ_MSB(MQTT_UNSUBSCRIBE_ID) && rev_data[3] == MOSQ_LSB(MQTT_UNSUBSCRIBE_ID))
	{
		result = 0;
	}
	
	return result;

}

//==========================================================
//	�������ƣ�	MQTT_PacketPublish
//
//	�������ܣ�	Pulish��Ϣ���
//
//	��ڲ�����	pkt_id��pkt_id
//				topic��������topic
//				payload����Ϣ��
//				payload_len����Ϣ�峤��
//				qos���ط�����
//				retain��������Ϣ����
//				own��
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		����-ʧ��
//
//	˵����		
//==========================================================
uint8 MQTT_PacketPublish(uint16 pkt_id, const int8 *topic,
						const int8 *payload, uint32 payload_len,
						enum MqttQosLevel qos, int32 retain, int32 own,
						MQTT_PACKET_STRUCTURE *mqttPacket)
{

	uint32 total_len = 0, topic_len = 0;
	uint32 data_len = 0;
	int32 len = 0;
	uint8 flags = 0;
	
	//pkt_id���----------------------------------------------------------------------------
	if(pkt_id == 0)
		return 1;
	
	//$dpΪϵͳ�ϴ����ݵ��ָ��--------------------------------------------------------------
	for(topic_len = 0; topic[topic_len] != '\0'; ++topic_len)
	{
		if((topic[topic_len] == '#') || (topic[topic_len] == '+'))
			return 2;
	}
	
	//Publish��Ϣ---------------------------------------------------------------------------
	flags |= MQTT_PKT_PUBLISH << 4;
	
	//retain��־----------------------------------------------------------------------------
	if(retain)
		flags |= 0x01;
	
	//�ܳ���--------------------------------------------------------------------------------
	total_len = topic_len + payload_len + 2;
	
	//qos����--��Ҫ����PUBLISH������̬����Ϣ�ģ���֤��Ϣ���ݵĴ���-----------------------------
	switch(qos)
	{
		case MQTT_QOS_LEVEL0:
			flags |= MQTT_CONNECT_WILL_QOS0;	//���һ��
		break;
		
		case MQTT_QOS_LEVEL1:
			flags |= 0x02;						//����һ��
			total_len += 2;
		break;
		
		case MQTT_QOS_LEVEL2:
			flags |= 0x04;						//ֻ��һ��
			total_len += 2;
		break;
		
		default:
		return 3;
	}
	
	//�����ڴ�------------------------------------------------------------------------------
	if(payload != NULL)
	{
		if(payload[0] == 2)
		{
			uint32 data_len_t = 0;
			
			while(payload[data_len_t++] != '}');
			data_len_t -= 3;
			data_len = data_len_t + 7;
			data_len_t = payload_len - data_len;
			
			MQTT_NewBuffer(mqttPacket, total_len + 3 - data_len_t);
			
			if(mqttPacket->_data == NULL)
				return 4;
			
			memset(mqttPacket->_data, 0, total_len + 3 - data_len_t);
		}
		else
		{
			MQTT_NewBuffer(mqttPacket, total_len + 5);
			
			if(mqttPacket->_data == NULL)
				return 4;
			
			memset(mqttPacket->_data, 0, total_len + 5);
		}
	}
	else
	{
		MQTT_NewBuffer(mqttPacket, total_len + 5);
		
		if(mqttPacket->_data == NULL)
			return 4;
		
		memset(mqttPacket->_data, 0, total_len + 5);
	}
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = flags;
	
	//�̶�ͷ��----------------------ʣ�೤��ֵ-----------------------------------------------
	len = MQTT_DumpLength(total_len, mqttPacket->_data + mqttPacket->_len);
	if(len < 0)
	{
		MQTT_DeleteBuffer(mqttPacket);
		return 5;
	}
	else
		mqttPacket->_len += len;
	
/*************************************�ɱ�ͷ��***********************************************/
	
	//�ɱ�ͷ��----------------------д��topic���ȡ�topic-------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(topic_len);
	mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(topic_len);
	
	strncat((int8 *)mqttPacket->_data + mqttPacket->_len, topic, topic_len);
	mqttPacket->_len += topic_len;
	if(qos != MQTT_QOS_LEVEL0)
	{
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_MSB(pkt_id);
		mqttPacket->_data[mqttPacket->_len++] = MOSQ_LSB(pkt_id);
	}
	
	//�ɱ�ͷ��----------------------д��payload----------------------------------------------
	if(payload != NULL)
	{
		if(payload[0] == 2)
		{
			memcpy((int8 *)mqttPacket->_data + mqttPacket->_len, payload, data_len);
			mqttPacket->_len += data_len;
		}
		else
		{
			memcpy((int8 *)mqttPacket->_data + mqttPacket->_len, payload, payload_len);
			mqttPacket->_len += payload_len;
		}
	}
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketPublish
//
//	�������ܣ�	Publish��Ϣ���
//
//	��ڲ�����	flags��MQTT��ر�־��Ϣ
//				pkt��ָ��ɱ�ͷ��
//				size���̶�ͷ���е�ʣ�೤����Ϣ
//
//	���ز�����	0-�ɹ�		����-ʧ��ԭ��
//
//	˵����		
//==========================================================
uint8 MQTT_UnPacketPublish(uint8 *rev_data, int8 **topic, uint16 *topic_len, int8 **payload, uint16 *payload_len, uint8 *qos, uint16 *pkt_id)
{
	
	const int8 flags = rev_data[0] & 0x0F;
	uint8 *msgPtr;
	uint32 remain_len = 0;

	const int8 dup = flags & 0x08;

	*qos = (flags & 0x06) >> 1;
	
	msgPtr = rev_data + MQTT_ReadLength(rev_data + 1, 4, &remain_len) + 1;
	
	if(remain_len < 2 || flags & 0x01)							//retain
		return 255;
	
	*topic_len = (uint16)msgPtr[0] << 8 | msgPtr[1];
	if(remain_len < *topic_len + 2)
		return 255;
	
	if(strstr((int8 *)msgPtr + 2, CMD_TOPIC_PREFIX) != NULL)	//����������·�
		return MQTT_PKT_CMD;
	
	switch(*qos)
	{
		case MQTT_QOS_LEVEL0:									// qos0 have no packet identifier
			
			if(0 != dup)
				return 255;

			*topic = MQTT_MallocBuffer(*topic_len + 1);			//Ϊtopic�����ڴ�
			if(*topic == NULL)
				return 255;
			
			memset(*topic, 0, *topic_len + 1);
			memcpy(*topic, (int8 *)msgPtr + 2, *topic_len);		//��������
			
			*payload_len = remain_len - 2 - *topic_len;			//Ϊpayload�����ڴ�
			*payload = MQTT_MallocBuffer(*payload_len + 1);
			if(*payload == NULL)								//���ʧ��
			{
				MQTT_FreeBuffer(*topic);						//����Ҫ��topic���ڴ��ͷŵ�
				return 255;
			}
			
			memset(*payload, 0, *payload_len + 1);
			memcpy(*payload, (int8 *)msgPtr + 2 + *topic_len, *payload_len);
			
		break;

		case MQTT_QOS_LEVEL1:
		case MQTT_QOS_LEVEL2:
			
			if(*topic_len + 2 > remain_len)
				return 255;
			
			*pkt_id = (uint16)msgPtr[*topic_len + 2] << 8 | msgPtr[*topic_len + 3];
			if(pkt_id == 0)
				return 255;
			
			*topic = MQTT_MallocBuffer(*topic_len + 1);			//Ϊtopic�����ڴ�
			if(*topic == NULL)
				return 255;
			
			memset(*topic, 0, *topic_len + 1);
			memcpy(*topic, (int8 *)msgPtr + 2, *topic_len);		//��������
			
			*payload_len = remain_len - 4 - *topic_len;
			*payload = MQTT_MallocBuffer(*payload_len + 1);		//Ϊpayload�����ڴ�
			if(*payload == NULL)								//���ʧ��
			{
				MQTT_FreeBuffer(*topic);						//����Ҫ��topic���ڴ��ͷŵ�
				return 255;
			}
			
			memset(*payload, 0, *payload_len + 1);
			memcpy(*payload, (int8 *)msgPtr + 4 + *topic_len, *payload_len);
			
		break;

		default:
			return 255;
	}
	
	if(strchr((int8 *)topic, '+') || strchr((int8 *)topic, '#'))
		return 255;

	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_PacketPublishAck
//
//	�������ܣ�	Publish Ack��Ϣ���
//
//	��ڲ�����	pkt_id��packet id
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��ԭ��
//
//	˵����		���յ���Publish��Ϣ��QoS�ȼ�Ϊ1ʱ����ҪAck�ظ�
//==========================================================
uint1 MQTT_PacketPublishAck(uint16 pkt_id, MQTT_PACKET_STRUCTURE *mqttPacket)
{

	MQTT_NewBuffer(mqttPacket, 4);
	if(mqttPacket->_data == NULL)
		return 1;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_PUBACK << 4;
	
	//�̶�ͷ��----------------------ʣ�೤��-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 2;
	
/*************************************�ɱ�ͷ��***********************************************/
	
	//�ɱ�ͷ��----------------------pkt_id����-----------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = pkt_id >> 8;
	mqttPacket->_data[mqttPacket->_len++] = pkt_id & 0xff;
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketPublishAck
//
//	�������ܣ�	Publish Ack��Ϣ���
//
//	��ڲ�����	rev_data���յ�������
//
//	���ز�����	0-�ɹ�		1-ʧ��ԭ��
//
//	˵����		
//==========================================================
uint1 MQTT_UnPacketPublishAck(uint8 *rev_data)
{

	if(rev_data[1] != 2)
		return 1;

	if(rev_data[2] == MOSQ_MSB(MQTT_PUBLISH_ID) && rev_data[3] == MOSQ_LSB(MQTT_PUBLISH_ID))
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	MQTT_PacketPublishRec
//
//	�������ܣ�	Publish Rec��Ϣ���
//
//	��ڲ�����	pkt_id��packet id
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��ԭ��
//
//	˵����		���յ���Publish��Ϣ��QoS�ȼ�Ϊ2ʱ�����յ�rec
//==========================================================
uint1 MQTT_PacketPublishRec(uint16 pkt_id, MQTT_PACKET_STRUCTURE *mqttPacket)
{

	MQTT_NewBuffer(mqttPacket, 4);
	if(mqttPacket->_data == NULL)
		return 1;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_PUBREC << 4;
	
	//�̶�ͷ��----------------------ʣ�೤��-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 2;
	
/*************************************�ɱ�ͷ��***********************************************/
	
	//�ɱ�ͷ��----------------------pkt_id����-----------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = pkt_id >> 8;
	mqttPacket->_data[mqttPacket->_len++] = pkt_id & 0xff;
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketPublishRec
//
//	�������ܣ�	Publish Rec��Ϣ���
//
//	��ڲ�����	rev_data�����յ�������
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_UnPacketPublishRec(uint8 *rev_data)
{

	if(rev_data[1] != 2)
		return 1;

	if(rev_data[2] == MOSQ_MSB(MQTT_PUBLISH_ID) && rev_data[3] == MOSQ_LSB(MQTT_PUBLISH_ID))
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	MQTT_PacketPublishRel
//
//	�������ܣ�	Publish Rel��Ϣ���
//
//	��ڲ�����	pkt_id��packet id
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��ԭ��
//
//	˵����		���յ���Publish��Ϣ��QoS�ȼ�Ϊ2ʱ�����յ�rec���ٻظ�rel
//==========================================================
uint1 MQTT_PacketPublishRel(uint16 pkt_id, MQTT_PACKET_STRUCTURE *mqttPacket)
{

	MQTT_NewBuffer(mqttPacket, 4);
	if(mqttPacket->_data == NULL)
		return 1;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_PUBREL << 4 | 0x02;
	
	//�̶�ͷ��----------------------ʣ�೤��-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 2;
	
/*************************************�ɱ�ͷ��***********************************************/
	
	//�ɱ�ͷ��----------------------pkt_id����-----------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = pkt_id >> 8;
	mqttPacket->_data[mqttPacket->_len++] = pkt_id & 0xff;
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketPublishRel
//
//	�������ܣ�	Publish Rel��Ϣ���
//
//	��ڲ�����	rev_data�����յ�������
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_UnPacketPublishRel(uint8 *rev_data, uint16 pkt_id)
{

	if(rev_data[1] != 2)
		return 1;

	if(rev_data[2] == MOSQ_MSB(pkt_id) && rev_data[3] == MOSQ_LSB(pkt_id))
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	MQTT_PacketPublishComp
//
//	�������ܣ�	Publish Comp��Ϣ���
//
//	��ڲ�����	pkt_id��packet id
//				mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��ԭ��
//
//	˵����		���յ���Publish��Ϣ��QoS�ȼ�Ϊ2ʱ�����յ�rec���ٻظ�rel
//==========================================================
uint1 MQTT_PacketPublishComp(uint16 pkt_id, MQTT_PACKET_STRUCTURE *mqttPacket)
{

	MQTT_NewBuffer(mqttPacket, 4);
	if(mqttPacket->_data == NULL)
		return 1;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_PUBCOMP << 4;
	
	//�̶�ͷ��----------------------ʣ�೤��-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 2;
	
/*************************************�ɱ�ͷ��***********************************************/
	
	//�ɱ�ͷ��----------------------pkt_id����-----------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = pkt_id >> 8;
	mqttPacket->_data[mqttPacket->_len++] = pkt_id & 0xff;
	
	return 0;

}

//==========================================================
//	�������ƣ�	MQTT_UnPacketPublishComp
//
//	�������ܣ�	Publish Comp��Ϣ���
//
//	��ڲ�����	rev_data�����յ�������
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_UnPacketPublishComp(uint8 *rev_data)
{

	if(rev_data[1] != 2)
		return 1;

	if(rev_data[2] == MOSQ_MSB(MQTT_PUBLISH_ID) && rev_data[3] == MOSQ_LSB(MQTT_PUBLISH_ID))
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	MQTT_PacketPing
//
//	�������ܣ�	�����������
//
//	��ڲ�����	mqttPacket����ָ��
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
uint1 MQTT_PacketPing(MQTT_PACKET_STRUCTURE *mqttPacket)
{

	MQTT_NewBuffer(mqttPacket, 2);
	if(mqttPacket->_data == NULL)
		return 1;
	
/*************************************�̶�ͷ��***********************************************/
	
	//�̶�ͷ��----------------------ͷ����Ϣ-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = MQTT_PKT_PINGREQ << 4;
	
	//�̶�ͷ��----------------------ʣ�೤��-------------------------------------------------
	mqttPacket->_data[mqttPacket->_len++] = 0;
	
	return 0;

}
