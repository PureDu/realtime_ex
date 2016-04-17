#pragma once
#include "libBaseCommon/singleton.h"

#include "libCoreCommon/core_common.h"

namespace core
{
	/**
	@brief: ��Ϣ�ɷ���
	*/
	class CMessageDispatcher :
		public base::CSingleton<CMessageDispatcher>
	{
	public:
		CMessageDispatcher();
		~CMessageDispatcher();

		bool init();

		/**
		@brief: ��Ϣ�ɷ��������ɸ�����ϢԴ�������ɷ���Ϣ
		*/
		void dispatch(const std::string& szFromServiceName, uint16_t nMessageType, const void* pData, uint16_t nSize);
	};
}