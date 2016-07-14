#pragma once
#include "libBaseCommon/singleton.h"
#include "libBaseLua/lua_facade.h"
#include "core_service_kit_define.h"

namespace core
{
	class CCoreServiceKit :
		public base::CSingleton<CCoreServiceKit>
	{
	public:
		CCoreServiceKit();
		~CCoreServiceKit();

		bool							init();
		virtual void					release();

		/**
		@brief: ��ȡ�����������Ϣ
		*/
		const SServiceBaseInfo&			getServiceBaseInfo() const;
		/**
		@brief: ����ȫ�ֵķ������ӳɹ��ص�
		*/
		void							setServiceConnectCallback(std::function<void(const std::string)> funConnect);
		/**
		@brief: ����ȫ�ֵķ������ӶϿ��ص�
		*/
		void							setServiceDisconnectCallback(std::function<void(const std::string)> funDisconnect);
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
	};
}