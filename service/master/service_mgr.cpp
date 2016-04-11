#include "stdafx.h"
#include "service_mgr.h"
#include "master_app.h"
#include "connection_from_service.h"
#include "libCoreCommon\proto_system.h"

#include "libCoreCommon\base_connection_mgr.h"

CServiceMgr::CServiceMgr()
{

}

CServiceMgr::~CServiceMgr()
{

}

bool CServiceMgr::init()
{
	return true;
}

CConnectionFromService* CServiceMgr::getServiceConnection(const std::string& szName) const
{
	auto iter = this->m_mapServiceInfo.find(szName);
	if (iter == this->m_mapServiceInfo.end())
		return nullptr;

	return iter->second.pConnectionFromService;
}

void CServiceMgr::addService(CConnectionFromService* pConnectionFromService, const SServiceBaseInfo& sServiceBaseInfo)
{
	DebugAst(pConnectionFromService != nullptr);

	auto iter = this->m_mapServiceInfo.find(sServiceBaseInfo.szName);
	DebugAst(iter == this->m_mapServiceInfo.end());

	// ��ʼ��������Ϣ
	SServiceInfo sServiceInfo;
	sServiceInfo.pConnectionFromService = pConnectionFromService;
	sServiceInfo.sServiceBaseInfo = sServiceBaseInfo;

	this->m_mapServiceInfo[sServiceBaseInfo.szName] = sServiceInfo;

	// �������������Ϣͬ�����µķ���
	std::vector<core::CBaseConnection*> vecBaseConnection;
	CMasterApp::Inst()->getBaseConnectionMgr()->getBaseConnection(_GET_CLASS_NAME(CConnectionFromService), vecBaseConnection);
	for (size_t i = 0; i < vecBaseConnection.size(); ++i)
	{
		core::CBaseConnection* pBaseConnection = vecBaseConnection[i];
		if (nullptr == pBaseConnection)
			continue;

		CConnectionFromService* pOtherConnectionFromService = dynamic_cast<CConnectionFromService*>(pBaseConnection);
		if (nullptr == pOtherConnectionFromService || pOtherConnectionFromService == pConnectionFromService)
			continue;

		auto iter = this->m_mapServiceInfo.find(pOtherConnectionFromService->getServiceName());
		
		SServiceInfo& sOtherServiceInfo = iter->second;
		base::CWriteBuf& writeBuf = CMasterApp::Inst()->getWriteBuf();

		// ͬ������������Ϣ
		smt_sync_service_base_info netMsg1;
		netMsg1.sServiceBaseInfo = sOtherServiceInfo.sServiceBaseInfo;

		netMsg1.pack(writeBuf);

		pConnectionFromService->send(eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());

		// ͬ����Ϣ������Ϣ
		writeBuf.clear();
		smt_sync_service_message_info netMsg2;
		netMsg2.nAdd = 0;
		for (size_t j = 0; j < sOtherServiceInfo.vecServiceMessageID.size(); ++j)
		{
			SMessageSyncInfo sMessageSyncInfo;
			sMessageSyncInfo.nGate = 0;
			sMessageSyncInfo.nMessageID = sOtherServiceInfo.vecServiceMessageID[i];
			netMsg2.vecMessageSyncInfo.push_back(sMessageSyncInfo);
		}
		for (size_t j = 0; j < sOtherServiceInfo.vecGateServiceMessageID.size(); ++j)
		{
			SMessageSyncInfo sMessageSyncInfo;
			sMessageSyncInfo.nGate = 0;
			sMessageSyncInfo.nMessageID = sOtherServiceInfo.vecGateServiceMessageID[i];
			netMsg2.vecMessageSyncInfo.push_back(sMessageSyncInfo);
		}
		
		netMsg2.pack(writeBuf);

		pConnectionFromService->send(eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());
	}

	// ������¼���ķ���㲥����������
	base::CWriteBuf& writeBuf = CMasterApp::Inst()->getWriteBuf();
	
	smt_sync_service_base_info netMsg;
	netMsg.sServiceBaseInfo = sServiceBaseInfo;
	// ���������������ӷ��Ľ��ջ��棬���ͻ����С�Ե�
	std::swap(netMsg.sServiceBaseInfo.nRecvBufSize, netMsg.sServiceBaseInfo.nSendBufSize);

	netMsg.pack(writeBuf);

	CMasterApp::Inst()->getBaseConnectionMgr()->broadcast(_GET_CLASS_NAME(CConnectionFromService), eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());
}

void CServiceMgr::delService(const std::string& szName)
{
	auto iter = this->m_mapServiceInfo.find(szName);
	if (iter == this->m_mapServiceInfo.end())
		return;

	base::CWriteBuf& writeBuf = CMasterApp::Inst()->getWriteBuf();

	smt_remove_service_base_info netMsg;
	netMsg.szName = szName;

	netMsg.pack(writeBuf);

	CMasterApp::Inst()->getBaseConnectionMgr()->broadcast(_GET_CLASS_NAME(CConnectionFromService), eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());

	this->m_mapServiceInfo.erase(iter);
}

void CServiceMgr::addServiceMessageInfo(const std::string& szName, const std::vector<SMessageSyncInfo>& vecMessageSyncInfo, bool bAdd)
{
	auto iter = this->m_mapServiceInfo.find(szName);
	if (iter == this->m_mapServiceInfo.end())
		return;

	SServiceInfo& sServiceInfo = iter->second;
	if (!bAdd)
	{
		sServiceInfo.vecServiceMessageID.clear();
		sServiceInfo.vecGateServiceMessageID.clear();
	}

	for (size_t i = 0; i < vecMessageSyncInfo.size(); ++i)
	{
		if (vecMessageSyncInfo[i].nGate)
			sServiceInfo.vecGateServiceMessageID.push_back(vecMessageSyncInfo[i].nMessageID);
		else
			sServiceInfo.vecServiceMessageID.push_back(vecMessageSyncInfo[i].nMessageID);
	}

	// ���������Ϣ��Ϣ�㲥����������
	base::CWriteBuf& writeBuf = CMasterApp::Inst()->getWriteBuf();

	smt_sync_service_message_info netMsg;
	netMsg.nAdd = bAdd;
	netMsg.vecMessageSyncInfo = vecMessageSyncInfo;
	netMsg.pack(writeBuf);

	CMasterApp::Inst()->getBaseConnectionMgr()->broadcast(_GET_CLASS_NAME(CConnectionFromService), eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());
}
