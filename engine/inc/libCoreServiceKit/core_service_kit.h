#pragma once
#include "libBaseCommon/singleton.h"
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
		@brief: ��ȡĳһ����Ϣ�ĵ�����Ϣ
		*/
		const SMessageProxyGroupInfo*	getMessageProxyGroupInfo(const std::string& szMessageName) const;
		/**
		@brief: ������ϢID��ȡ��Ϣ����
		*/
		const std::string&				getMessageName(uint32_t nMessageID) const;
		/**
		@brief: ��ʼһ���µ�trace
		*/
		void							startNewTrace();
		/**
		@brief: ���trace������Ϣ
		*/
		void							addTraceExtraInfo(const char* szFormat, ...);
	};
}