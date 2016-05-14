#pragma once
#include "libBaseCommon/singleton.h"

#include "libCoreCommon/core_common.h"

#include "core_service_kit_define.h"

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
		void registerServiceCallback(const std::string& szMessageName, ServiceCallback callback);
		/**
		@brief: ע�ᾭ���ط���ת���ͻ��˵���Ϣ
		*/
		void registerGateForwardCallback(const std::string& szMessageName, GateForwardCallback callback);
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