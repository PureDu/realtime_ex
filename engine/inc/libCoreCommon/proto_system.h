#pragma once
#include "core_common.h"

enum ESystemMessageType
{
	eSMT_SyncServiceInfo	= 1,	// ��������֮��ͬ��������Ϣ
};

Msg_Begin(CSMT_SyncServiceInfo, eSMT_SyncServiceInfo)
	enum{ eAdd, eDel, eSync };
	uint8_t		nType;
	char		szName[_MAX_SERVICE_TYPE_LEN];
	char		szHost[INET_ADDRSTRLEN];
	uint16_t	nPort;	// 0��ʾ�÷���û�м�����ַ
	uint32_t	nRecvBufSize;
	uint32_t	nSendBufSize;
Msg_End