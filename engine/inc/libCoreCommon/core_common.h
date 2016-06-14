#pragma once

#include <functional>

#include "libBaseNetwork/network.h"

#include "ticker.h"

namespace core
{
#pragma pack(push,1)
	// ��Ϣͷ
	struct inside_message_header
	{
		uint16_t	nMessageSize;	// ������Ϣͷ��
		uint8_t		nMessageType;	// ��Ϣ���ͣ����ͼ�����	

		inside_message_header(uint8_t nMessageType) : nMessageType(nMessageType) { }
		inside_message_header() {}
	};

	struct client_message_header
	{
		uint16_t	nMessageSize;	// ������Ϣͷ��
		uint32_t	nMessageID;
	};

	struct inside_message_cookice
	{
		uint32_t	nMessageID;

		inside_message_cookice(uint32_t nMessageID) : nMessageID(nMessageID) { }
		inside_message_cookice() { nMessageID = 0; }
		~inside_message_cookice() { }
	};
#pragma pack(pop)
}

#define message_begin(MessageName, nMessageID) \
class MessageName : public core::inside_message_cookice\
{\
public:\
	MessageName() : core::inside_message_cookice(nMessageID) { }\
	static  uint16_t	getMessageID() { return nMessageID; }\
	static  const char*	getMessageName() { return #MessageName; }

#define message_end };

#define pack_begin(writeBuf)\
	writeBuf.clear();\
	writeBuf.write(this, sizeof(core::inside_message_cookice));

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
	readBuf.read(this, sizeof(core::inside_message_cookice));

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
	eMT_LUA				= 0x0100,	// lua��Ϣ

	// ������
	eMT_BROADCAST		= 0x200,	// �㲥��Ϣ
};

namespace core
{
	typedef std::function<int32_t(const char*, uint32_t, uint8_t&)>	MessageParser;	// ԭ����Ϣ
}

#define _BASE_CONNECTION_TYPE_BEGIN	100