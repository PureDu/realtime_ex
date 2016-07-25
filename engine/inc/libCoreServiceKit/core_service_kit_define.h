#pragma once
#include "libCoreCommon/core_common.h"

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

namespace core
{

	struct SServiceBaseInfo
	{
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
		const std::string	szServiceName;
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

	typedef std::shared_ptr<const message_header>	message_header_ptr;

	class CResponsePromise;
	typedef std::function<void(uint8_t, message_header_ptr)>								InvokeCallback;			// RPC��Ϣ��Ӧ�ص���������
	typedef std::function<CResponsePromise(uint8_t, message_header_ptr)>					InvokeCallbackEx;		// RPC��Ϣ��Ӧ�ص���������
	typedef std::function<void(uint32_t)>													InvokeErrCallback;		// RPC������Ӧ�ص���������
	typedef std::function<void(const std::string, uint8_t, message_header_ptr)>				ServiceCallback;		// ������Ϣ����������(����������ֱ�����ֵ�����������ã���Ϊ��Э��)
	typedef std::function<void(const SClientSessionInfo, uint8_t, message_header_ptr)>		GateForwardCallback;	// �����ط���ת���Ŀͻ�����Ϣ����������
	typedef std::function<void(uint64_t, message_header_ptr)>								ClientCallback;			// �ͻ�����Ϣ����������
	typedef std::function<bool(const std::string&, uint8_t, const void*, uint16_t)>			ServiceGlobalFilter;	// ȫ�ֵ���Ϣ����������

#pragma pack(push,1)
	struct gate_cookice
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
		uint64_t nTraceID;
	};

	struct response_cookice
	{
		uint64_t	nSessionID;
		uint64_t	nTraceID;
		uint8_t		nResult;
	};

#pragma pack(pop)
}