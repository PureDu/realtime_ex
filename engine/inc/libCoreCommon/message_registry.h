#pragma once
#include "libBaseCommon/singleton.h"

#include "core_common.h"

#include <map>

namespace core
{
	/**
	@brief: ��Ϣ��Ӧע����
	*/
	class CMessageRegistry :
		public base::CSingleton<CMessageRegistry>
	{
	public:
		CMessageRegistry();
		~CMessageRegistry();

		bool init();

		/**
		@brief: ע����ͨ�ķ�����Ӧ����
		*/
		void registCallback(uint32_t nMessageID, ServiceCallback callback);
		/**
		@brief: ע�ᾭ���ط���ת���ͻ�����Ϣ��Ӧ����
		*/
		void registCallback(uint32_t nMessageID, GateClientCallback callback);
		/**
		@brief: ע�����֮���ǰ�ù�����
		*/
		void registGlobalBeforeFilter(ServiceGlobalFilter callback);
		/**
		@brief: ע�����֮��ĺ��ù�����
		*/
		void registGlobalAfterFilter(ServiceGlobalFilter callback);
	};
}