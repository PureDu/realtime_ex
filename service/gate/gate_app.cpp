#include "stdafx.h"
#include "gate_app.h"
#include "connection_from_client.h"
#include "gate_message_dispatcher.h"
#include "gate_message_handler.h"
#include "service_connection_factory.h"

#include "libCoreCommon/base_connection_mgr.h"
#include "libCoreServiceKit/message_dispatcher.h"
#include "libCoreServiceKit/message_registry.h"
#include "libCoreServiceKit/core_service_kit.h"

#include <functional>

#include "tinyxml2/tinyxml2.h"

static bool gate_before_filter(const std::string& szFromServiceName, uint32_t nMessageType, const void* pData, uint16_t nSize)
{
	DebugAstEx(pData != nullptr, false);

	CGateMessageDispatcher::Inst()->dispatch(0, nMessageType, pData, nSize);

	return true;
}

CGateApp::CGateApp()
	: m_pGateSessionMgr(nullptr)
	, m_pServiceConnectionFactory(nullptr)
{
}

CGateApp::~CGateApp()
{
}

CGateApp* CGateApp::Inst()
{
	return static_cast<CGateApp*>(CBaseApp::Inst());
}

bool CGateApp::onInit()
{
	this->m_pServiceConnectionFactory = new CServiceConnectionFactory();
	this->getBaseConnectionMgr()->setBaseConnectionFactory(eBCT_ConnectionFromClient, this->m_pServiceConnectionFactory);

	if (!core::CCoreServiceKit::Inst()->init())
	{
		PrintWarning("core::CCoreServiceKit::Inst()->init()");
		return false;
	}

	core::CMessageRegistry::Inst()->addGlobalBeforeFilter(std::bind(gate_before_filter, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	
	if (!CGateMessageDispatcher::Inst()->init())
	{
		PrintWarning("CGateMessageDispatcher::Inst()->init()");
		return false;
	}

	if (!CGateMessageHandler::Inst()->init())
	{
		PrintWarning("CGateMessageHandler::Inst()->init()");
		return false;
	}

	this->m_pGateSessionMgr = new CGateSessionMgr();
	if (!this->m_pGateSessionMgr->init())
	{
		PrintWarning("this->m_pGateSessionMgr->init()");
		return false;
	}
	tinyxml2::XMLDocument* pConfigXML = new tinyxml2::XMLDocument();
	if (pConfigXML->LoadFile(this->getConfigFileName().c_str()) != tinyxml2::XML_SUCCESS)
	{
		PrintWarning("load etc config error");
		return false;
	}
	tinyxml2::XMLElement* pRootXML = pConfigXML->RootElement();
	if (pRootXML == nullptr)
	{
		PrintWarning("pRootXML == nullptr");
		return false;
	}
	// 启动客户端连接
	tinyxml2::XMLElement* pListenClientAddrXML = pRootXML->FirstChildElement("client");
	DebugAstEx(pListenClientAddrXML != nullptr, false);
	this->getBaseConnectionMgr()->listen(pListenClientAddrXML->Attribute("host") != nullptr ? pListenClientAddrXML->Attribute("host") : "0.0.0.0", (uint16_t)pListenClientAddrXML->IntAttribute("port"), eBCT_ConnectionFromClient, "", pListenClientAddrXML->IntAttribute("send_buf_size"), pListenClientAddrXML->IntAttribute("recv_buf_size"), core::default_parser_native_data);

	SAFE_DELETE(pConfigXML);

	return true;
}

void CGateApp::onDestroy()
{
	this->getBaseConnectionMgr()->setBaseConnectionFactory(eBCT_ConnectionFromClient, nullptr);
	
	SAFE_DELETE(this->m_pGateSessionMgr);
	SAFE_DELETE(this->m_pServiceConnectionFactory);

	core::CCoreServiceKit::Inst()->release();
}

void CGateApp::onQuit()
{
	this->doQuit();
}

CGateSessionMgr* CGateApp::getGateSessionMgr() const
{
	return this->m_pGateSessionMgr;
}

int32_t main(int32_t argc, char* argv[])
{
	CGateApp* pGateApp = new CGateApp();
	pGateApp->run(argc, argv, "gate_config.xml");

	return 0;
}