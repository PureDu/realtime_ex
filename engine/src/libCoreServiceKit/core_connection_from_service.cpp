#include "stdafx.h"
#include "core_connection_from_service.h"
#include "proto_system.h"
#include "message_dispatcher.h"
#include "core_service_app_impl.h"

namespace core
{
	CCoreConnectionFromService::CCoreConnectionFromService()
		: m_nServiceID(0)
	{

	}

	CCoreConnectionFromService::~CCoreConnectionFromService()
	{

	}

	bool CCoreConnectionFromService::init(const std::string& szContext)
	{
		return true;
	}

	uint32_t CCoreConnectionFromService::getType() const
	{
		return eBCT_ConnectionFromService;
	}

	void CCoreConnectionFromService::release()
	{
		delete this;
	}

	void CCoreConnectionFromService::onConnect()
	{
		// ����Ǳ������ӣ��Է�������Ϣ���ɶԷ����������ƹ���
	}

	void CCoreConnectionFromService::onDisconnect()
	{
		if (!this->getServiceID() != 0)
			CCoreServiceAppImpl::Inst()->getCoreServiceProxy()->delCoreConnectionFromService(this->getServiceID());
	}

	bool CCoreConnectionFromService::onDispatch(uint8_t nMessageType, const void* pData, uint16_t nSize)
	{
		if (nMessageType == eMT_SYSTEM)
		{
			const core::message_header* pHeader = reinterpret_cast<const core::message_header*>(pData);
			DebugAstEx(nSize > sizeof(core::message_header), true);

			if (pHeader->nMessageID == eSMT_notify_service_base_info)
			{
				DebugAstEx(this->getServiceID() == 0, true);

				smt_notify_service_base_info netMsg;
				netMsg.unpack(pData, nSize);
				if (netMsg.nFromServiceID == 0)
				{
					this->shutdown(true, "empty service name");
					return true;
				}

				if (!CCoreServiceAppImpl::Inst()->getCoreServiceProxy()->addCoreConnectionFromService(netMsg.nFromServiceID, this))
				{
					PrintWarning("dup service service_id %d", netMsg.nFromServiceID);
					this->shutdown(true, "dup service connection");
					return true;
				}

				this->m_nServiceID = netMsg.nFromServiceID;
			}

			return true;
		}
		else
		{
			// ������������ֶ�û���ϱ��ͷ��������������ˣ��϶��Ƿ���ֱ���ߵ�
			if (this->getServiceID() == 0)
			{
				this->shutdown(true, "invalid connection");
				return true;
			}
			return CMessageDispatcher::Inst()->dispatch(this->getServiceID(), nMessageType, pData, nSize);
		}
	}

	uint16_t CCoreConnectionFromService::getServiceID() const
	{
		return this->m_nServiceID;
	}

}