#include "stdafx.h"
#include "connection_from_service.h"
#include "connection_from_client.h"
#include "gate_app.h"

#include "libCoreCommon\base_connection_mgr.h"
#include "libCoreCommon\proto_system.h"

DEFINE_OBJECT(CConnectionFromService, 100)

CConnectionFromService::CConnectionFromService()
{

}

CConnectionFromService::~CConnectionFromService()
{

}

void CConnectionFromService::onConnect(const std::string& szContext)
{
	// ���ﲻ��Ҫ���������������ط������Ϣ����Ϊ���������������ط���ģ�������������֪�����ط�����Ϣ
}

void CConnectionFromService::onDisconnect()
{
	CGateApp::Inst()->getServiceMgr()->delServiceConnection(this->m_szServiceName);
}

void CConnectionFromService::onDispatch(uint16_t nMsgType, const void* pData, uint16_t nSize)
{
	// ��������ͨ�����ط���ת����Ϣ���ͻ���
	if (nMsgType == eMT_GATE)
	{
		DebugAst(nSize > sizeof(gate_header));
		const gate_header* pHeader = reinterpret_cast<const gate_header*>(pData);
		CBaseConnection*  pBaseConnection = CGateApp::Inst()->getBaseConnectionMgr()->getBaseConnection(pHeader->nID);
		if (nullptr == pBaseConnection)
			return;

		CConnectionFromClient* pConnectionFromClient = dynamic_cast<CConnectionFromClient*>(pBaseConnection);
		DebugAst(nullptr != pConnectionFromClient);
		
		pConnectionFromClient->send(eMT_CLIENT, pHeader + 1, nSize - sizeof(gate_header));
	}
	else if (nMsgType == eMT_SYSTEM)
	{
		const core::message_header* pHeader = reinterpret_cast<const core::message_header*>(pData);
		DebugAst(nSize > sizeof(core::message_header));

		if (pHeader->nMsgID == eSMT_SyncServiceInfo)
		{
			const CSMT_SyncServiceInfo* pInfo = reinterpret_cast<const CSMT_SyncServiceInfo*>(pData);
			DebugAst(pInfo->nType == CSMT_SyncServiceInfo::eSync);

			this->m_szServiceName = pInfo->szName;
			// �������������ļ�����ַ������Ȥ����Ϊ���ط��񲻿���ȥ����������������

			CGateApp::Inst()->getServiceMgr()->addServiceConnection(this);
		}
	}
}

const std::string& CConnectionFromService::getServiceName() const
{
	return this->m_szServiceName;
}