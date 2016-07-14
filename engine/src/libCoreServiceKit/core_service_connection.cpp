#include "stdafx.h"
#include "core_service_connection.h"
#include "proto_system.h"
#include "message_dispatcher.h"
#include "core_service_kit_impl.h"
#include "core_service_proxy.h"

#include "libCoreCommon/base_connection_mgr.h"
#include "libCoreCommon/base_app.h"

/*
����֮���������ֹ���
step1 connection1 ---eSMT_notify_service_base_info	---> connection2
step2 connection2 ---eSMT_notify_ack_service_base_info---> connection1

�����step2������ͻ������step1����ͬʱ��������ô�͹ر�����һ������

*/
namespace core
{
	CCoreServiceConnection::CCoreServiceConnection()
	{

	}

	CCoreServiceConnection::~CCoreServiceConnection()
	{

	}

	bool CCoreServiceConnection::init(const std::string& szContext)
	{
		this->m_szServiceName = szContext;

		return true;
	}

	uint32_t CCoreServiceConnection::getType() const
	{
		return eBCT_ConnectionService;
	}

	void CCoreServiceConnection::release()
	{
		delete this;
	}

	void CCoreServiceConnection::onConnect()
	{
		// szContext�д���Ƿ�������
		if (CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->getServiceConnection(this->getServiceName()) != nullptr)
		{
			PrintWarning("dup service service_name: %s", this->getServiceName().c_str());
			this->shutdown(true, "dup service connection");
			return;
		}

		const SServiceBaseInfo* pServiceBaseInfo = CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->getServiceBaseInfo(this->m_szServiceName);
		if (pServiceBaseInfo != nullptr)
		{
			// ͬ��������Ϣ
			smt_notify_service_base_info netMsg;
			netMsg.szFromServiceName = CCoreServiceKitImpl::Inst()->getServiceBaseInfo().szName;
			base::CWriteBuf& writeBuf = CBaseApp::Inst()->getWriteBuf();
			netMsg.pack(writeBuf);

			this->send(eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());
		}
	}

	void CCoreServiceConnection::onDisconnect()
	{
		if (!this->m_szServiceName.empty())
			CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->delServiceConnection(this->m_szServiceName);
	}

	bool CCoreServiceConnection::onDispatch(uint8_t nMessageType, const void* pData, uint16_t nSize)
	{
		if (nMessageType == eMT_SYSTEM)
		{
			const core::message_header* pHeader = reinterpret_cast<const core::message_header*>(pData);
			DebugAstEx(nSize > sizeof(core::message_header), true);

			if (pHeader->nMessageID == eSMT_notify_service_base_info)
			{
				DebugAstEx(this->m_szServiceName.empty(), true);

				smt_notify_service_base_info netMsg;
				netMsg.unpack(pData, nSize);

				// �Ѿ������ֳɹ��������ˣ��Ͽ�ǰ�����ֵ�����
				if (CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->getServiceConnection(netMsg.szFromServiceName) != nullptr)
				{
					PrintWarning("dup service service_name: %s", netMsg.szFromServiceName.c_str());
					this->shutdown(true, "dup service connection");
					return true;
				}

				std::vector<CBaseConnection*> vecBaseConnection = CBaseApp::Inst()->getBaseConnectionMgr()->getBaseConnection(eBCT_ConnectionService);
				for (size_t i = 0; i < vecBaseConnection.size(); ++i)
				{
					CCoreServiceConnection* pCoreServiceConnection = dynamic_cast<CCoreServiceConnection*>(vecBaseConnection[i]);
					if (nullptr == pCoreServiceConnection)
						continue;

					if (pCoreServiceConnection->getServiceName() == netMsg.szFromServiceName)
					{
						// ��������г�ͻ���ͶϿ��������ֹ�ϣֵС��
						uint32_t nOtherHashID = base::hash(netMsg.szFromServiceName.c_str());
						uint32_t nOwnerHashID = base::hash(CCoreServiceKitImpl::Inst()->getServiceBaseInfo().szName.c_str());
						if (nOwnerHashID > nOtherHashID)
						{
							PrintWarning("conflict service connection service_name: %s", netMsg.szFromServiceName.c_str());
							this->shutdown(true, "conflict service connection");
							return true;
						}
						else
						{
							PrintWarning("conflict service connection service_name: %s", netMsg.szFromServiceName.c_str());
							pCoreServiceConnection->shutdown(true, "conflict service connection");
						}
					}
				}
				
				this->m_szServiceName = netMsg.szFromServiceName;

				if (!CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->addServiceConnection(this))
				{
					PrintWarning("add service connection error service_name: %s", this->m_szServiceName.c_str());
					this->m_szServiceName.clear();
					this->shutdown(true, "dup service connection");
					return true;
				}

				smt_notify_ack_service_base_info netMsg2;
				netMsg2.szServiceName = CCoreServiceKitImpl::Inst()->getServiceBaseInfo().szName;
				base::CWriteBuf& writeBuf = CBaseApp::Inst()->getWriteBuf();
				netMsg2.pack(writeBuf);

				this->send(eMT_SYSTEM, writeBuf.getBuf(), (uint16_t)writeBuf.getCurSize());

				PrintInfo("service shake hands half service_name: %s", this->m_szServiceName.c_str());
			}
			else if (pHeader->nMessageID == eSMT_notify_ack_service_base_info)
			{
				smt_notify_ack_service_base_info netMsg;
				netMsg.unpack(pData, nSize);
				if (netMsg.szServiceName.empty())
				{
					this->shutdown(true, "empty service name");
					return true;
				}

				if (!CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->addServiceConnection(this))
				{
					PrintWarning("add service connection error service_name: %s", this->m_szServiceName.c_str());
					this->m_szServiceName.clear();
					this->shutdown(true, "dup service connection");
					return true;
				}

				PrintInfo("service shake hands full service_name: %s", this->m_szServiceName.c_str());
			}
		}
		else
		{
			// ������������ֶ�û���ϱ��ͷ��������������ˣ��϶��Ƿ���ֱ���ߵ�
			if (this->m_szServiceName.empty())
			{
				this->shutdown(true, "invalid connection");
				return true;
			}
			return CMessageDispatcher::Inst()->dispatch(this->getServiceName(), nMessageType, pData, nSize);
		}

		return true;
	}

	const std::string& CCoreServiceConnection::getServiceName() const
	{
		return this->m_szServiceName;
	}

}