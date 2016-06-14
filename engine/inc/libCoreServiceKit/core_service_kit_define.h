#pragma once
#include "libCoreCommon/core_common.h"

#include "google/protobuf/message.h"

#include <string>
#include <vector>
#include <map>

#define _SERVICE_WEIGHT_PERCENT_MULT	10000	// ����Ȩ�ر���
#define _MAX_SERVICE_TYPE_LEN			64
#define _MAX_SERVICE_NAME_LEN			64
#define _MAX_SERVICE_API_NAME_LEN		128

#define _GET_MESSAGE_ID(szMessageName) (base::hash(szMessageName.c_str()))

namespace core
{
	struct SMetaMessageProxyInfo
	{
		std::string	szMessageName;		// ��Ϣ����
		std::string	szLoadbalanceName;	// ���ؾ���������
		std::string	szFail;				// ʧ�ܲ�������
		uint32_t	nRetries;			// ʧ�����Դ���
		uint32_t	nTimeout;			// Զ�̵��ó�ʱʱ��
	};

	struct SMessageProxyInfo
	{
		std::string	szServiceName;		// ��������
		std::string	szServiceGroup;		// ������
		std::string	szMessageName;		// ��Ϣ����
		int32_t		nWeight;			// ���ؾ���Ȩ��
	};

	struct SMessageProxyGroupInfo
	{
		std::map<std::string, SMessageProxyInfo>	mapMessageProxyInfo;
		std::map<std::string, int32_t>				mapGroupWeight;
		int32_t										nTotalWeight;
	};

	struct SServiceBaseInfo
	{
		std::string	szType;			// ��������
		std::string	szName;			// ��������
		std::string	szGroup;		// ������������
		std::string	szHost;
		uint16_t	nPort;			// 0��ʾ�÷���û�м�����ַ
		uint32_t	nRecvBufSize;
		uint32_t	nSendBufSize;
	};

	struct SClientSessionInfo
	{
		const std::string&	szServiceName;
		uint64_t			nSessionID;

		SClientSessionInfo(const std::string&	szServiceName, uint64_t nSessionID)
			: szServiceName(szServiceName), nSessionID(nSessionID)
		{}
	};

	struct SServiceSessionInfo
	{
		std::string	szServiceName;
		uint64_t	nSessionID;
	};

	enum EResponseResultType
	{
		eRRT_OK,
		eRRT_TIME_OUT,
		eRRT_ERROR,
	};

	typedef std::function<void(uint32_t, const google::protobuf::Message*, EResponseResultType)>		InvokeCallback;			// RPC��Ϣ��Ӧ�ص���������
	typedef std::function<void(const std::string&, uint32_t, const google::protobuf::Message*)>			ServiceCallback;		// ������Ϣ����������
	typedef std::function<void(const SClientSessionInfo&, uint32_t, const google::protobuf::Message*)>	GateForwardCallback;	// �����ط���ת���Ŀͻ�����Ϣ����������
	typedef std::function<void(uint64_t, const client_message_header*)>									ClientCallback;			// �ͻ�����Ϣ����������
	typedef std::function<bool(const std::string&, uint8_t, const void*, uint16_t)>						ServiceGlobalFilter;	// ȫ�ֵ���Ϣ����������

#pragma pack(push,1)
	struct gate_cookice :
		public inside_message_cookice
	{
		uint64_t nSessionID;
		uint64_t nTraceID;
	};
	
	struct gate_broadcast_cookice :
		public inside_message_cookice
	{
		uint16_t nCount;
	};

	struct request_cookice :
		public inside_message_cookice
	{
		uint64_t nSessionID;
		uint64_t nTraceID;
	};

	struct response_cookice :
		public inside_message_cookice
	{
		uint64_t	nSessionID;
		uint64_t	nTraceID;
		uint8_t		nResult;
		uint16_t	nMessageNameLen;
		char		szMessageName[1];
	};

#pragma pack(pop)
}