#include "stdafx.h"
#include "core_connection_from_service.h"
#include "proto_system.h"
#include "message_dispatcher.h"
#include "core_service_kit_impl.h"

namespace core
{
	CCoreConnectionFromService::CCoreConnectionFromService()
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
		if (!this->m_szServiceName.empty())
			CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->delConnectionFromService(this->m_szServiceName);
	}

	void CCoreConnectionFromService::onDispatch(uint8_t nMessageType, const void* pData, uint16_t nSize)
	{
		if (nMessageType == eMT_SYSTEM)
		{
			const core::message_header* pHeader = reinterpret_cast<const core::message_header*>(pData);
			DebugAst(nSize > sizeof(core::message_header));

			if (pHeader->nMessageID == eSMT_notify_service_base_info)
			{
				DebugAst(this->m_szServiceName.empty());

				smt_notify_service_base_info netMsg;
				netMsg.unpack(pData, nSize);
				if (netMsg.szServiceName.empty())
				{
					this->shutdown(true, "empty service name");
					return;
				}

				if (netMsg.szKey != CCoreServiceKitImpl::Inst()->getServiceBaseInfo().szName)
				{
					this->shutdown(true, "error key");
					return;
				}

				// �������������ļ�����ַ������Ȥ
				this->m_szServiceName = netMsg.szServiceName;

				if (!CCoreServiceKitImpl::Inst()->getCoreServiceProxy()->addConnectionFromService(this))
				{
					PrintWarning("dup service service_name: %s", this->m_szServiceName.c_str());
					this->m_szServiceName.clear();
					this->shutdown(true, "dup service connection");
					return;
				}
			}
		}
		else
		{
			// ������������ֶ�û���ϱ��ͷ��������������ˣ��϶��Ƿ���ֱ���ߵ�
			if (this->m_szServiceName.empty())
			{
				this->shutdown(true, "invalid connection");
				return;
			}
			CMessageDispatcher::Inst()->dispatch(this->getServiceName(), nMessageType, pData, nSize);
		}
	}

	const std::string& CCoreConnectionFromService::getServiceName() const
	{
		return this->m_szServiceName;
	}

}