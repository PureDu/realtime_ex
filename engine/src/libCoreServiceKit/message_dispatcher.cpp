#include "stdafx.h"
#include "message_dispatcher.h"
#include "protobuf_helper.h"
#include "core_service_kit_impl.h"
#include "core_service_kit_define.h"

#include "libBaseCommon/debug_helper.h"
#include "libBaseCommon/defer.h"
#include "libCoreCommon/coroutine.h"

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

	bool CMessageDispatcher::dispatch(const std::string& szFromServiceName, uint8_t nMessageType, const void* pData, uint16_t nSize)
	{
		DebugAstEx(pData != nullptr, true);

		// ��ǰ�ù�������һ�飬�����һ������false��ֱ�����������Ϣ
		const std::vector<ServiceGlobalFilter>& vecServiceGlobalBeforeFilter = CCoreServiceKitImpl::Inst()->getGlobalBeforeFilter();
		for (size_t i = 0; i < vecServiceGlobalBeforeFilter.size(); ++i)
		{
			if (vecServiceGlobalBeforeFilter[i] != nullptr && !vecServiceGlobalBeforeFilter[i](szFromServiceName, nMessageType, pData, nSize))
				return true;
		}

		bool bRet = true;
		if ((nMessageType&eMT_TYPE_MASK) == eMT_REQUEST)
		{
			DebugAstEx(nSize > sizeof(request_cookice), true);

			const request_cookice* pCookice = reinterpret_cast<const request_cookice*>(pData);

			SServiceSessionInfo& sServiceSessionInfo = CCoreServiceKitImpl::Inst()->getTransporter()->getServiceSessionInfo();
			sServiceSessionInfo.szServiceName = szFromServiceName;
			sServiceSessionInfo.nSessionID = pCookice->nSessionID;

			// ����cookice
			const message_header* pHeader = reinterpret_cast<const message_header*>(pCookice + 1);

			CCoreServiceKitImpl::Inst()->getInvokerTrace()->beginRecv(pCookice->nTraceID, pHeader->nMessageID, szFromServiceName);

			ServiceCallback& callback = CCoreServiceKitImpl::Inst()->getCoreServiceInvoker()->getCallback(pHeader->nMessageID);
			if (callback != nullptr)
			{
				message_header_ptr pMessage = message_header_ptr(pHeader, [pData](const void*){ delete[] reinterpret_cast<const char*>(pData); });
				uint64_t nCoroutineID = coroutine::start([&](uint64_t){ callback(szFromServiceName, nMessageType, pMessage); });
				coroutine::resume(nCoroutineID, 0);

				bRet = false;
			}
			sServiceSessionInfo.szServiceName.clear();
			sServiceSessionInfo.nSessionID = 0;

			CCoreServiceKitImpl::Inst()->getInvokerTrace()->endRecv();
		}
		else if ((nMessageType&eMT_TYPE_MASK) == eMT_RESPONSE)
		{
			const response_cookice* pCookice = reinterpret_cast<const response_cookice*>(pData);
			// ����cookice
			const message_header* pHeader = reinterpret_cast<const message_header*>(pCookice + 1);

			CCoreServiceKitImpl::Inst()->getInvokerTrace()->beginRecv(pCookice->nTraceID, pHeader->nMessageID, szFromServiceName);

			SResponseWaitInfo* pResponseWaitInfo = CCoreServiceKitImpl::Inst()->getTransporter()->getResponseWaitInfo(pCookice->nSessionID, true);
			if (nullptr == pResponseWaitInfo)
			{
				CCoreServiceKitImpl::Inst()->getInvokerTrace()->addTraceExtraInfo("invalid session id by message dispatcher"UINT64FMT, pCookice->nSessionID);
				return true;
			}

			Defer(delete pResponseWaitInfo);

			if (pResponseWaitInfo->callback != nullptr && pCookice->nResult == eRRT_OK)
			{
				message_header_ptr pMessage = message_header_ptr(pHeader, [pData](const void*){ delete[] reinterpret_cast<const char*>(pData); });
				uint64_t nCoroutineID = coroutine::start([&](uint64_t){ pResponseWaitInfo->callback(pResponseWaitInfo, nMessageType, pMessage); });
				coroutine::resume(nCoroutineID, 0);

				bRet = false;
			}
			else if (pResponseWaitInfo->err != nullptr && pCookice->nResult != eRRT_OK)
			{
				message_header_ptr pMessage = message_header_ptr(pHeader, [pData](const void*){ delete[] reinterpret_cast<const char*>(pData); });
				uint64_t nCoroutineID = coroutine::start([&](uint64_t){ pResponseWaitInfo->err((EResponseResultType)pCookice->nResult); });
				coroutine::resume(nCoroutineID, 0);

				bRet = false;
			}
			else
			{
				if (pResponseWaitInfo->nCoroutineID != 0)
				{
					coroutine::sendMessage(pResponseWaitInfo->nCoroutineID, reinterpret_cast<void*>(pCookice->nResult));
					coroutine::sendMessage(pResponseWaitInfo->nCoroutineID, const_cast<message_header*>(pHeader));
					coroutine::sendMessage(pResponseWaitInfo->nCoroutineID, const_cast<void*>(pData));
					coroutine::resume(pResponseWaitInfo->nCoroutineID, 0);

					bRet = false;
				}
			}
			CCoreServiceKitImpl::Inst()->getInvokerTrace()->endRecv();
		}
		else if ((nMessageType&eMT_TYPE_MASK) == eMT_GATE_FORWARD)
		{
			const gate_cookice* pCookice = reinterpret_cast<const gate_cookice*>(pData);
			// ����cookice
			const message_header* pHeader = reinterpret_cast<const message_header*>(pCookice + 1);
			
			CCoreServiceKitImpl::Inst()->getInvokerTrace()->beginRecv(pCookice->nTraceID, pHeader->nMessageID, szFromServiceName);

			SClientSessionInfo session(szFromServiceName, pCookice->nSessionID);

			GateForwardCallback& callback = CCoreServiceKitImpl::Inst()->getCoreServiceInvoker()->getGateClientCallback(pHeader->nMessageID);
			if (callback != nullptr)
			{
				message_header_ptr pMessage = message_header_ptr(pHeader, [pData](const void*){ delete[] reinterpret_cast<const char*>(pData); });
				uint64_t nCoroutineID = coroutine::start([&](uint64_t){ callback(session, nMessageType, pMessage); });
				coroutine::resume(nCoroutineID, 0);
				
				bRet = false;
			}
			CCoreServiceKitImpl::Inst()->getInvokerTrace()->endRecv();
		}

		const std::vector<ServiceGlobalFilter>& vecServiceGlobalAfterFilter = CCoreServiceKitImpl::Inst()->getGlobalAfterFilter();
		for (size_t i = 0; i < vecServiceGlobalAfterFilter.size(); ++i)
		{
			if (vecServiceGlobalAfterFilter[i] != nullptr)
				vecServiceGlobalAfterFilter[i](szFromServiceName, nMessageType, pData, nSize);
		}

		return bRet;
	}
}