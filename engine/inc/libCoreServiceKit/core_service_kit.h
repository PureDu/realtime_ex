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
	};
}