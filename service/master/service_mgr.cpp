#include "stdafx.h"
#include "service_mgr.h"
#include "master_app.h"
#include "connection_from_service.h"

#include "libCoreCommon/base_connection_mgr.h"
#include "libBaseCommon/base_function.h"
#include "libCoreServiceKit/proto_system.h"
#include "libCoreServiceKit/core_service_kit_define.h"

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

bool CServiceMgr::registerService(CConnectionFromService* pConnectionFromService, const core::SServiceBaseInfo& sServiceBaseInfo)
{
	DebugAstEx(pConnectionFromService != nullptr, false);

	auto iter = this->m_mapServiceInfo.find(sServiceBaseInfo.szName);
	if (iter != this->m_mapServiceInfo.end())
		return false;

	// ��ʼ��������Ϣ
	SServiceInfo sServiceInfo;
	sServiceInfo.pConnectionFromService = pConnectionFromService;
	sServiceInfo.sServiceBaseInfo = sServiceBaseInfo;

	this->m_mapServiceInfo[sServiceBaseInfo.szName] = sServiceInfo;

	std::vector<uint64_t> vecSocketID;
	// �������������Ϣͬ�����µķ���
	std::vector<core::CBaseConnection*> vecBaseConnection = CMasterApp::Inst()->getBaseConnectionMgr()->getBaseConnection(eBCT_ConnectionFromService);
	for (size_t i = 0; i < vecBaseConnection.size(); ++i)
	{
		core::CBaseConnection* pBaseConnection = vecBaseConnection[i];
		if (nullptr == pBaseConnection)
			continue;

		CConnectionFromService* pOtherConnectionFromService = dynamic_cast<CConnectionFromService*>(pBaseConnection);
		if (nullptr == pOtherConnectionFromService || pOtherConnectionFromService == pConnectionFromService)
			continue;

		auto iter = this->m_mapServiceInfo.find(pOtherConnectionFromService->getServiceName());
		if (iter == this->m_mapServiceInfo.end())
			continue;

		SServiceInfo& sOtherServiceInfo = iter->second;
		base::CWriteBuf& writeBuf = CMasterApp::Inst()->getWriteBuf();

		// ͬ������������Ϣ
		core::smt_sync_service_base_info netMsg1;
		netMsg1.sServiceBaseInfo = sOtherServiceInfo.sServiceBaseInfo;

		netMsg1.pack(writeBuf);
		pConnectionFromService->send(eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());

		vecSocketID.push_back(pOtherConnectionFromService->getID());
	}

	// ������¼���ķ���㲥����������
	base::CWriteBuf& writeBuf = CMasterApp::Inst()->getWriteBuf();
	
	core::smt_sync_service_base_info netMsg;
	netMsg.sServiceBaseInfo = sServiceBaseInfo;
	// ���������������ӷ��Ľ��ջ��棬���ͻ����С�Ե�
	std::swap(netMsg.sServiceBaseInfo.nRecvBufSize, netMsg.sServiceBaseInfo.nSendBufSize);

	netMsg.pack(writeBuf);

	CMasterApp::Inst()->getBaseConnectionMgr()->broadcast(vecSocketID, eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());

	PrintInfo("register service service_name: %s local addr: %s %d remote addr: %s %d", sServiceBaseInfo.szName.c_str(), pConnectionFromService->getLocalAddr().szHost, pConnectionFromService->getLocalAddr().nPort, pConnectionFromService->getRemoteAddr().szHost, pConnectionFromService->getRemoteAddr().nPort);

	return true;
}

void CServiceMgr::unregisterService(const std::string& szServiceName)
{
	auto iter = this->m_mapServiceInfo.find(szServiceName);
	if (iter == this->m_mapServiceInfo.end())
		return;

	// ����÷������Ϣ
	SServiceInfo& sServiceInfo = iter->second;

	base::CWriteBuf& writeBuf = CMasterApp::Inst()->getWriteBuf();

	core::smt_remove_service_base_info netMsg;
	netMsg.szName = szServiceName;

	netMsg.pack(writeBuf);

	std::vector<uint64_t> vecExcludeID;
	if (sServiceInfo.pConnectionFromService != nullptr)
		vecExcludeID.push_back(sServiceInfo.pConnectionFromService->getID());
	CMasterApp::Inst()->getBaseConnectionMgr()->broadcast(eBCT_ConnectionFromService, eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize(), &vecExcludeID);

	this->m_mapServiceInfo.erase(iter);

	PrintInfo("unregister service service_name: %s", szServiceName.c_str());
}