#pragma once
#include "libCoreCommon/core_common.h"
#include "libCoreCommon/message.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

#define _SERVICE_WEIGHT_PERCENT_MULT	10000	// ����Ȩ�ر���
#define _MAX_SERVICE_TYPE_LEN			64
#define _MAX_SERVICE_NAME_LEN			64
#define _MAX_SERVICE_API_NAME_LEN		128

#define _GET_MESSAGE_ID(szMessageName) (base::hash(szMessageName.c_str()))

enum EResponseResultType
{
	eRRT_OK,
	eRRT_TIME_OUT,
	eRRT_ERROR,
};

#define _REMOTE_ACTOR_BIT 48

namespace core
{

	struct SServiceBaseInfo
	{
		uint16_t	nID;
		std::string	szType;			// ��������
		std::string	szName;			// ��������
		std::string	szGroup;		// ������������
		std::string	szHost;			// ������IP
		uint16_t	nPort;			// 0��ʾ�÷���û�м�����ַ
		uint32_t	nRecvBufSize;	// ���ջ�������С
		uint32_t	nSendBufSize;	// ���ͻ�������С
	};

	struct SClientSessionInfo
	{
		uint16_t	nServiceID;
		uint64_t	nSessionID;

		SClientSessionInfo(uint16_t	nServiceID, uint64_t nSessionID)
			: nServiceID(nServiceID), nSessionID(nSessionID)
		{}
	};

	struct SServiceSessionInfo
	{
		uint16_t	nServiceID;
		uint64_t	nSessionID;
	};

	struct SActorSessionInfo
	{
		uint64_t	nActorID;
		uint64_t	nSessionID;
	};

	class CResponseFuture;
	
	typedef std::function<void(uint8_t, CMessage)>								InvokeCallback;			// RPC��Ϣ��Ӧ�ص���������
	typedef std::function<CResponseFuture(uint8_t, CMessage)>					InvokeCallbackEx;		// RPC��Ϣ��Ӧ�ص���������
	typedef std::function<void(uint32_t)>										InvokeErrCallback;		// RPC������Ӧ�ص���������
	typedef std::function<void(uint16_t, uint8_t, CMessage)>					ServiceCallback;		// ������Ϣ����������(����������ֱ�����ֵ�����������ã���Ϊ��Э��)
	typedef std::function<void(SClientSessionInfo, uint8_t, CMessage)>			GateForwardCallback;	// �����ط���ת���Ŀͻ�����Ϣ����������
	typedef std::function<void(uint64_t, CMessage)>								ClientCallback;			// �ͻ�����Ϣ����������
	typedef std::function<void(uint16_t, uint8_t, const void*, uint16_t)>		ServiceGlobalFilter;	// ȫ�ֵ���Ϣ����������

#pragma pack(push,1)
	struct gate_forward_cookice
	{
		uint64_t nActorID;
		uint64_t nSessionID;
		uint64_t nTraceID;
	};

	struct gate_send_cookice
	{
		uint64_t nSessionID;
		uint64_t nTraceID;
	};
	
	struct gate_broadcast_cookice :
		public message_header
	{
		uint16_t nCount;
	};

	struct request_cookice
	{
		uint64_t nSessionID;
		uint64_t nFromActorID;
		uint64_t nToActorID;
		uint64_t nTraceID;
	};

	struct response_cookice
	{
		uint64_t	nActorID;
		uint64_t	nSessionID;
		uint64_t	nTraceID;
		uint8_t		nResult;
	};

#pragma pack(pop)
}