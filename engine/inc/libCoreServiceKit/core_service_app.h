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
		@brief: ����ȫ�ֵķ������ӳɹ��ص�
		*/
		void							setServiceConnectCallback(std::function<void(uint16_t)> funConnect);
		/**
		@brief: ����ȫ�ֵķ������ӶϿ��ص�
		*/
		void							setServiceDisconnectCallback(std::function<void(uint16_t)> funDisconnect);
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