#pragma once
#include "core_common.h"

#include "libBaseCommon/buf_file.h"

#include <vector>

enum ESystemMessageType
{
	eSMT_register_service_base_info		= 1,	// ����������master����ע�����
	eSMT_unregister_service_base_info	= 2,	// ����������master����ע�����
	eSMT_sync_service_base_info			= 3,	// master���������ͬ������������Ϣ
	eSMT_remove_service_base_info		= 4,	// master���������ͬ�����������뿪��Ϣ
	eSMT_notify_service_base_info		= 3,	// ��������ķ����򱻶����յķ���֪ͨ���������Ϣ
	eSMT_register_service_message_info	= 4,	// ����������master��������API��Ϣ
	eSMT_sync_service_message_info		= 5,	// master�������������ͬ����������ĵ�API��Ϣ
};

message_begin(smt_register_service_base_info, eSMT_register_service_base_info)
	SServiceBaseInfo	sServiceBaseInfo;

	void pack(base::CWriteBuf& writeBuf)
	{
		pack_begin(writeBuf);

		writeBuf.write(sServiceBaseInfo.szType);
		writeBuf.write(sServiceBaseInfo.szName);
		writeBuf.write(sServiceBaseInfo.szHost);
		writeBuf.write(sServiceBaseInfo.nPort);
		writeBuf.write(sServiceBaseInfo.nRecvBufSize);
		writeBuf.write(sServiceBaseInfo.nSendBufSize);

		pack_end(writeBuf);
	}

	void unpack(const void* pBuf, uint16_t nSize)
	{
		unpack_begin(pBuf, nSize);

		readBuf.read(sServiceBaseInfo.szType);
		readBuf.read(sServiceBaseInfo.szName);
		readBuf.read(sServiceBaseInfo.szHost);
		readBuf.read(sServiceBaseInfo.nPort);
		readBuf.read(sServiceBaseInfo.nRecvBufSize);
		readBuf.read(sServiceBaseInfo.nSendBufSize);

		unpack_end();
	}

message_end

message_begin(smt_unregister_service_base_info, eSMT_unregister_service_base_info)
	std::string	szName;

	void pack(base::CWriteBuf& writeBuf)
	{
		pack_begin(writeBuf);

		writeBuf.write(szName);

		pack_end(writeBuf);
	}

	void unpack(const void* pBuf, uint16_t nSize)
	{
		unpack_begin(pBuf, nSize);

		readBuf.read(szName);

		unpack_end();
	}
message_end

message_begin(smt_sync_service_base_info, eSMT_sync_service_base_info)
	SServiceBaseInfo	sServiceBaseInfo;

	void pack(base::CWriteBuf& writeBuf)
	{
		pack_begin(writeBuf);

		writeBuf.write(sServiceBaseInfo.szType);
		writeBuf.write(sServiceBaseInfo.szName);
		writeBuf.write(sServiceBaseInfo.szHost);
		writeBuf.write(sServiceBaseInfo.nPort);
		writeBuf.write(sServiceBaseInfo.nRecvBufSize);
		writeBuf.write(sServiceBaseInfo.nSendBufSize);

		pack_end(writeBuf);
	}

	void unpack(const void* pBuf, uint16_t nSize)
	{
		unpack_begin(pBuf, nSize);

		readBuf.read(sServiceBaseInfo.szType);
		readBuf.read(sServiceBaseInfo.szName);
		readBuf.read(sServiceBaseInfo.szHost);
		readBuf.read(sServiceBaseInfo.nPort);
		readBuf.read(sServiceBaseInfo.nRecvBufSize);
		readBuf.read(sServiceBaseInfo.nSendBufSize);

		unpack_end();
	}

message_end

message_begin(smt_remove_service_base_info, eSMT_remove_service_base_info)
	std::string	szName;

	void pack(base::CWriteBuf& writeBuf)
	{
		pack_begin(writeBuf);

		writeBuf.write(szName);

		pack_end(writeBuf);
	}

	void unpack(const void* pBuf, uint16_t nSize)
	{
		unpack_begin(pBuf, nSize);

		readBuf.read(szName);

		unpack_end();
	}
message_end

message_begin(smt_notify_service_base_info, eSMT_notify_service_base_info)
	std::string	szServiceName;

	void pack(base::CWriteBuf& writeBuf)
	{
		pack_begin(writeBuf);

		writeBuf.write(szServiceName);

		pack_end(writeBuf);
	}

	void unpack(const void* pBuf, uint16_t nSize)
	{
		unpack_begin(pBuf, nSize);

		readBuf.read(szServiceName);

		unpack_end();
	}
message_end

message_begin(smt_register_service_message_info, eSMT_register_service_message_info)
	std::vector<SMessageSyncInfo>	vecMessageSyncInfo;

	void pack(base::CWriteBuf& writeBuf)
	{
		pack_begin(writeBuf);

		uint16_t nCount = (uint16_t)vecMessageSyncInfo.size();
		writeBuf.write(nCount);
		for (uint16_t i = 0; i < nCount; ++i)
		{
			writeBuf.write(vecMessageSyncInfo[i].szMessageName);
		}

		pack_end(writeBuf);
	}

	void unpack(const void* pBuf, uint16_t nSize)
	{
		unpack_begin(pBuf, nSize);

		uint16_t nCount = 0;
		readBuf.read(nCount);
		vecMessageSyncInfo.reserve(nCount);
		for (uint16_t i = 0; i < nCount; ++i)
		{
			SMessageSyncInfo sMessageSyncInfo;
			readBuf.read(sMessageSyncInfo.szMessageName);

			vecMessageSyncInfo.push_back(sMessageSyncInfo);
		}

		unpack_end();
	}
message_end

message_begin(smt_sync_service_message_info, eSMT_sync_service_message_info)
	std::string						szServiceName;
	std::vector<SMessageSyncInfo>	vecMessageSyncInfo;

	void pack(base::CWriteBuf& writeBuf)
	{
		pack_begin(writeBuf);

		writeBuf.write(szServiceName);
		uint16_t nCount = (uint16_t)vecMessageSyncInfo.size();
		writeBuf.write(nCount);
		for (uint16_t i = 0; i < nCount; ++i)
		{
			writeBuf.write(vecMessageSyncInfo[i].szMessageName.c_str());
		}

		pack_end(writeBuf);
	}

	void unpack(const void* pBuf, uint16_t nSize)
	{
		unpack_begin(pBuf, nSize);

		readBuf.read(szServiceName);
		uint16_t nCount = 0;
		readBuf.read(nCount);
		vecMessageSyncInfo.reserve(nCount);
		for (uint16_t i = 0; i < nCount; ++i)
		{
			SMessageSyncInfo sMessageSyncInfo;
			readBuf.read(sMessageSyncInfo.szMessageName);

			vecMessageSyncInfo.push_back(sMessageSyncInfo);
		}

		unpack_end();
	}
message_end