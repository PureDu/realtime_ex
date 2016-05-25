#include "stdafx.h"
#include "core_connection_to_master.h"
#include "proto_system.h"
#include "core_service_kit_impl.h"

#include "libCoreCommon/base_app.h"
#include "libCoreCommon/base_connection_mgr.h"

namespace core
{
	CCoreConnectionToMaster::CCoreConnectionToMaster()
	{

	}

	CCoreConnectionToMaster::~CCoreConnectionToMaster()
	{

	}

	bool CCoreConnectionToMaster::init(const std::string& szContext)
	{
		return true;
	}

	uint32_t CCoreConnectionToMaster::getType() const
	{
		return eBCT_ConnectionToMaster;
	}

	void CCoreConnectionToMaster::release()
	{
		delete this;
	}

	void CCoreConnectionToMaster::onConnect()
	{
		// 连接master的连接只能有一个
		std::vector<CBaseConnection*> vecBaseConnection;
		CBaseApp::Inst()->getBaseConnectionMgr()->getBaseConnection(this->getType(), vecBaseConnection);
		if (vecBaseConnection.size() > 1)
		{
			this->shutdown(false, "dup master connection");
			return;
		}

		smt_register_service_base_info netMsg;
		netMsg.sServiceBaseInfo = CCoreServiceKitImpl::Inst()->getServiceBaseInfo();

		base::CWriteBuf& writeBuf = CBaseApp::Inst()->getWriteBuf();
		netMsg.pack(writeBuf);
		
		this->send(eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());

		CCoreServiceKitImpl::Inst()->getCoreServiceInvoker()->onConnectToMaster();
	}

	void CCoreConnectionToMaster::onDisconnect()
	{

	}

	void CCoreConnectionToMaster::onDispatch(uint32_t nMsgType, const void* pData, uint16_t nSize)
	{
		DebugAst(nMsgType == eMT_SYSTEM);

		const core::message_header* pHeader = reinterpret_cast<const core::message_header*>(pData);
		DebugAst(nSize > sizeof(core::message_header));

		if (pHeader->nMessageID == eSMT_sync_service_base_info)
		{
			smt_sync_service_base_info netMsg;
			netMsg.unpack(pData, nSize);
			
			CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->addService(netMsg.sServiceBaseInfo);
		}
		else if (pHeader->nMessageID == eSMT_remove_service_base_info)
		{
			smt_remove_service_base_info netMsg;
			netMsg.unpack(pData, nSize);

			CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->delService(netMsg.szName);
		}
		else if (pHeader->nMessageID == eSMT_sync_service_message_info)
		{
			smt_sync_service_message_info netMsg;
			netMsg.unpack(pData, nSize);

			for (size_t i = 0; i < netMsg.vecMessageProxyInfo.size(); ++i)
			{
				CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->addMessageProxyInfo(netMsg.vecMessageProxyInfo[i]);
			}
		}
	}
}