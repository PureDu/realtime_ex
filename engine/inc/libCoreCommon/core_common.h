#pragma once

#include <functional>

#include "libBaseNetwork/network.h"

#include "ticker.h"

namespace core
{
#pragma pack(push,1)
	// ��Ϣͷ
	struct message_header
	{
		uint16_t	nMessageSize;	// ������Ϣͷ��
		uint32_t	nMessageID;	

		message_header(uint16_t nMessageID) : nMessageID(nMessageID) { }
		message_header() {}
	};
#pragma pack(pop)

	inline int32_t default_parser_native_data(const void* pData, uint32_t nDataSize)
	{
		// ��������Ϣͷ
		if (nDataSize < sizeof(message_header))
			return 0;

		const message_header* pHeader = reinterpret_cast<const message_header*>(pData);
		if (pHeader->nMessageSize < sizeof(message_header))
			return -1;

		// ������������Ϣ
		if (nDataSize < pHeader->nMessageSize)
			return 0;

		return pHeader->nMessageSize;
	}
}

#define message_begin(MessageName, nMessageID) \
class MessageName : public core::message_header\
{\
public:\
	MessageName() : message_header(nMessageID) { nMessageSize = sizeof(MessageName); }\
	static  uint16_t	getMessageID() { return nMessageID; }\
	static  const char*	getMessageName() { return #MessageName; }

#define message_end };

#define pack_begin(writeBuf)\
	writeBuf.clear();\
	writeBuf.write(this, sizeof(core::message_header));

#define pack_end(writeBuf)\
	do\
			{\
		uint16_t nPos = (uint16_t)writeBuf.getCurSize(); \
		writeBuf.seek(base::eBST_Begin, 0); \
		writeBuf.write(nPos); \
		writeBuf.seek(base::eBST_Begin, nPos);\
			} while(0)

#define unpack_begin(buf, size)\
	base::CReadBuf readBuf;\
	readBuf.init(buf, size);\
	readBuf.read(this, sizeof(core::message_header));

#define unpack_end()



#define _INVALID_SOCKET_ID			-1

enum EMessageType
{
	eMT_HEARTBEAT		= 1,		// ����֮��������Ϣ
	eMT_REQUEST			= 2,		// ����֮���������Ϣ
	eMT_RESPONSE		= 3,		// ����֮�����Ӧ��Ϣ
	eMT_SYSTEM			= 4,		// ����֮���ϵͳ��Ϣ
	eMT_GATE_FORWARD	= 5,		// �ͻ���ͨ�����ط���ת��������������Ϣ
	eMT_TO_GATE			= 6,		// ��������ͨ�����ط���ת���ͻ�����Ϣ
	eMT_CLIENT			= 7,		// �ͻ�����Ϣ
	
	eMT_TYPE_MASK		= 0x00ff,	// ��������

	// ������
	eMT_BROADCAST		= 0x200,	// �㲥��Ϣ
};

namespace core
{
	typedef std::function<int32_t(const char*, uint32_t)>	ClientDataCallback;
}