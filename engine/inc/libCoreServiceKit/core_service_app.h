#pragma once
#include "libCoreCommon/base_app.h"

#include "libBaseLua/lua_facade.h"

#include "core_service_kit_define.h"

namespace core
{
	class CCoreServiceApp :
		public CBaseApp
	{
	public:
		CCoreServiceApp();
		~CCoreServiceApp();

		static CCoreServiceApp* Inst();

		/**
		@brief: ��ȡ���ڵ������Ϣ
		*/
		const SNodeBaseInfo&			getNodeBaseInfo() const;
		/**
		@brief: ���ݽڵ����ֻ�ȡ�ڵ�id
		*/
		uint16_t						getNodeID(const std::string& szName) const;
		/**
		@brief: ����ȫ�ֵĽڵ����ӳɹ��ص�
		*/
		void							setNodeConnectCallback(const std::function<void(uint16_t)>& callback);
		/**
		@brief: ����ȫ�ֵĽڵ����ӶϿ��ص�
		*/
		void							setNodeDisconnectCallback(const std::function<void(uint16_t)>& callback);
		/**
		@brief: ע����ͨ�ڵ���Ϣ
		*/
		void							registerMessageHandler(uint16_t nMessageID, const std::function<bool(uint16_t, CMessage)>& callback);
		/**
		@brief: ע�ᾭ���ؽڵ�ת���ͻ��˵���Ϣ
		*/
		void							registerForwardHandler(uint16_t nMessageID, const std::function<bool(SClientSessionInfo, CMessage)>& callback);
		/**
		@brief: ��ӽڵ�֮���ǰ�ù�����
		*/
		void							addGlobalBeforeFilter(GlobalBeforeFilter callback);
		/**
		@brief: ��ӽڵ�֮��ĺ��ù�����
		*/
		void							addGlobalAfterFilter(GlobalAfterFilter callback);
		/**
		@brief: ��ʼһ���µ�trace
		*/
		void							startNewTrace();
		/**
		@brief: ���trace������Ϣ
		*/
		void							addTraceExtraInfo(const char* szFormat, ...);
		/**
		@brief: ��ȡlua��װ����
		*/
		base::CLuaFacade*				getLuaFacade() const;

	protected:
		virtual bool					onInit();
		virtual void					onProcess();
		virtual void					onDestroy();
	};

}