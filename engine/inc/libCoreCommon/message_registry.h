#pragma once
#include "libBaseCommon/singleton.h"

#include "core_common.h"

#include <map>

namespace core
{
	/**
	@brief: ��Ϣע����
	*/
	class CMessageRegistry :
		public base::CSingleton<CMessageRegistry>
	{
	public:
		CMessageRegistry();
		~CMessageRegistry();

		bool init();

		/**
		@brief: ע����ͨ������Ϣ
		*/
		void registerCallback(const std::string& szMessageName, ServiceCallback callback);
		/**
		@brief: ע�ᾭ���ط���ת���ͻ��˵���Ϣ
		*/
		void registerCallback(const std::string& szMessageName, GateClientCallback callback);
		/**
		@brief: ��ӷ���֮���ǰ�ù�����
		*/
		void addGlobalBeforeFilter(ServiceGlobalFilter callback);
		/**
		@brief: ��ӷ���֮��ĺ��ù�����
		*/
		void addGlobalAfterFilter(ServiceGlobalFilter callback);
	};
}