#include "stdafx.h"
#include "message_dispatcher.h"
#include "protobuf_helper.h"
#include "core_service_kit_impl.h"
#include "core_service_kit_define.h"

#include "libBaseCommon/debug_helper.h"
#include "libBaseCommon/defer.h"

namespace core
{
	CMessageDispatcher::CMessageDispatcher()
	{

	}

	CMessageDispatcher::~CMessageDispatcher()
	{

	}

	bool CMessageDispatcher::init()
	{
		return true;
	}

	void CMessageDispatcher::dispatch(const std::string& szFromServiceName, uint32_t nMessageType, const void* pData, uint16_t nSize)
	{
		DebugAst(pData != nullptr);

		// ��ǰ�ù�������һ�飬�����һ������false��ֱ�����������Ϣ
		const std::vector<ServiceGlobalFilter>& vecServiceGlobalBeforeFilter = CCoreServiceKitImpl::Inst()->getGlobalBeforeFilter();
		for (size_t i = 0; i < vecServiceGlobalBeforeFilter.size(); ++i)
		{
			if (vecServiceGlobalBeforeFilter[i] != nullptr && !vecServiceGlobalBeforeFilter[i](szFromServiceName, nMessageType, pData, nSize))
				return;
		}

		if ((nMessageType&eMT_TYPE_MASK) == eMT_REQUEST)
		{
			const request_cookice* pCookice = reinterpret_cast<const request_cookice*>(pData);

			SServiceSessionInfo& sServiceSessionInfo = CCoreServiceKitImpl::Inst()->getTransporter()->getServiceSessionInfo();
			sServiceSessionInfo.szServiceName = szFromServiceName;
			sServiceSessionInfo.nSessionID = pCookice->nSessionID;

			// ����cookice
			const message_header* pHeader = reinterpret_cast<const message_header*>(pCookice + 1);
			const std::string& szMessageName = CCoreServiceKitImpl::Inst()->getCoreServiceInvoker()->getMessageName(pHeader->nMessageID);
			ServiceCallback& callback = CCoreServiceKitImpl::Inst()->getCoreServiceInvoker()->getCallback(pHeader->nMessageID);
			if (callback != nullptr)
			{
				google::protobuf::Message* pMessage = unserialize_protobuf_message_from_buf(szMessageName, pHeader);
				if (nullptr == pMessage)
					PrintWarning("create message error from_service_name: %s message_name: %s message_id: %d", szFromServiceName.c_str(), szMessageName.c_str(), pHeader->nMessageID);
				else
					callback(szFromServiceName, nMessageType, pMessage);

				SAFE_DELETE(pMessage);
			}
			sServiceSessionInfo.szServiceName.clear();
			sServiceSessionInfo.nSessionID = 0;
		}
		else if ((nMessageType&eMT_TYPE_MASK) == eMT_RESPONSE)
		{
			const response_cookice* pCookice = reinterpret_cast<const response_cookice*>(pData);

			SResponseWaitInfo* pResponseWaitInfo = CCoreServiceKitImpl::Inst()->getTransporter()->getResponseWaitInfo(pCookice->nSessionID, true);
			if (nullptr == pResponseWaitInfo)
			{
				PrintWarning("invalid session id from_service_name: %s session_id: "UINT64FMT, szFromServiceName.c_str(), pCookice->nSessionID);
				return;
			}

			Defer(delete pResponseWaitInfo);

			if (pResponseWaitInfo->callback != nullptr)
			{
				const std::string& szMessageName = pCookice->szMessageName;
				google::protobuf::Message* pMessage = create_protobuf_message(szMessageName);
				if (nullptr == pMessage)
				{
					PrintWarning("create message error from_service_name: %s message_name: %s", szFromServiceName.c_str(), szMessageName.c_str());
					return;
				}

				Defer(delete pMessage);

				const void* pMessageData = reinterpret_cast<const char*>(pCookice + 1) + pCookice->nMessageNameLen;
				DebugAst(nSize > sizeof(response_cookice) + pCookice->nMessageNameLen);
				if (!pMessage->ParseFromArray(pMessageData, nSize - sizeof(response_cookice) - pCookice->nMessageNameLen))
				{
					PrintWarning("parse message error from_service_name: %s message_name: %s", szFromServiceName.c_str(), szMessageName.c_str());
					return;
				}

				pResponseWaitInfo->callback(nMessageType, pMessage, (EResponseResultType)pCookice->nResult);
			}
		}
		else if ((nMessageType&eMT_TYPE_MASK) == eMT_GATE_FORWARD)
		{
			const gate_cookice* pCookice = reinterpret_cast<const gate_cookice*>(pData);

			// ����cookice
			const message_header* pHeader = reinterpret_cast<const message_header*>(pCookice + 1);
			const std::string& szMessageName = CCoreServiceKitImpl::Inst()->getCoreServiceInvoker()->getMessageName(pHeader->nMessageID);

			SClientSessionInfo session(szFromServiceName, pCookice->nSessionID);

			GateForwardCallback& callback = CCoreServiceKitImpl::Inst()->getCoreServiceInvoker()->getGateClientCallback(pHeader->nMessageID);
			if (callback != nullptr)
			{
				google::protobuf::Message* pMessage = unserialize_protobuf_message_from_buf(szMessageName, pHeader);
				if (nullptr == pMessage)
					PrintWarning("create message error from_service_name: %s message_name: %s message_id: %d", szFromServiceName.c_str(), szMessageName.c_str(), pHeader->nMessageID);
				else
					callback(session, nMessageType, pMessage);

				SAFE_DELETE(pMessage);
			}
		}

		const std::vector<ServiceGlobalFilter>& vecServiceGlobalAfterFilter = CCoreServiceKitImpl::Inst()->getGlobalAfterFilter();
		for (size_t i = 0; i < vecServiceGlobalAfterFilter.size(); ++i)
		{
			if (vecServiceGlobalAfterFilter[i] != nullptr)
				vecServiceGlobalAfterFilter[i](szFromServiceName, nMessageType, pData, nSize);
		}
	}
}