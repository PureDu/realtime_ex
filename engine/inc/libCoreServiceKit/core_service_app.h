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
		@brief: ��ȡ�����������Ϣ
		*/
		const SServiceBaseInfo&			getServiceBaseInfo() const;
		/**
		@brief: ���ݷ������ֻ�ȡ����id
		*/
		uint16_t						getServiceID(const std::string& szServiceName) const;
		/**
		@brief: ����ȫ�ֵķ������ӳɹ��ص�
		*/
		void							setServiceConnectCallback(std::function<void(uint16_t)> funConnect);
		/**
		@brief: ����ȫ�ֵķ������ӶϿ��ص�
		*/
		void							setServiceDisconnectCallback(std::function<void(uint16_t)> funDisconnect);
		/**
		@brief: ע����ͨ������Ϣ
		*/
		void							registerServiceCallback(uint16_t nMessageID, ServiceCallback callback);
		/**
		@brief: ע�ᾭ���ط���ת���ͻ��˵���Ϣ
		*/
		void							registerGateForwardCallback(uint16_t nMessageID, GateForwardCallback callback);
		/**
		@brief: ��ӷ���֮���ǰ�ù�����
		*/
		void							addGlobalBeforeFilter(ServiceGlobalFilter callback);
		/**
		@brief: ��ӷ���֮��ĺ��ù�����
		*/
		void							addGlobalAfterFilter(ServiceGlobalFilter callback);
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