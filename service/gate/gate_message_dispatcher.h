#pragma once
#include "libBaseCommon/singleton.h"

#include "libCoreCommon/core_common.h"

#include <map>

class CGateMessageDispatcher
	: public base::CSingleton<CGateMessageDispatcher>
{
public:
	CGateMessageDispatcher();
	~CGateMessageDispatcher();

	bool	init();

	/**
	@brief: ��Ϣ�ɷ��������ɸ�����ϢԴ�������ɷ���Ϣ
	*/
	void	dispatch(uint16_t nMessageType, const void* pData, uint16_t nSize);
	/**
	@brief: ע�ᾭ�ͻ�����Ϣ��Ӧ����
	*/
	void	registCallback(uint32_t nMessageID, core::ClientCallback callback);
	/**
	@brief: ��ȡ��Ϣ��Ӧ����
	*/
	core::ClientCallback&
			getCallback(uint32_t nMessageID);

private:
	std::map<uint32_t, core::ClientCallback>	m_mapClientCallback;
};