#pragma once
#include "libBaseCommon/singleton.h"
#include "libCoreCommon/core_common.h"

#include "core_service_kit_define.h"

namespace core
{
	/**
	@brief: ��Ⱥ���������
	*/
	class CClusterInvoker :
		public base::CSingleton<CClusterInvoker>
	{
	public:
		CClusterInvoker();
		~CClusterInvoker();

		bool				init();

		/**
		@brief: ����ָ��Զ�̷������Ϣ��Զ�̷����÷�����Ϣ
		*/
		bool				invok(const std::string& szServiceName, const message_header* pData);
		/**
		@brief: ����ָ��Զ�̷������Ϣ���߼�������
		*/
		uint32_t			invok(const std::string& szServiceName, const message_header* pData, message_header_ptr& pResultData);
		/**
		@brief: ����ָ��Զ�̷������Ϣ����Ҫ�ṩԶ�̷�����Ϣ���ص���Ӧ�����ص�
		*/
		bool				invok_r(const std::string& szServiceName, const message_header* pData, InvokeCallback callback, uint64_t nContext = 0);
		/**
		@brief: ��ӦԶ�̷���ĵ��ã�������Ӧ��Ϣ
		*/
		void				response(const message_header* pData);
		/**
		@brief: ��ӦԶ�̷���ĵ��ã�������Ӧ��Ϣ�������Ҫ�û��Լ��ƶ�����session����Ҫ���ڲ���������Ӧ����Ҫ�ȴ�������Ϣ����Ϣ
		*/
		void				response(const SServiceSessionInfo& sServiceSessionInfo, const message_header* pData);
		/**
		@brief: ��ȡ��ǰ����session��Ϣ
		*/
		SServiceSessionInfo	getServiceSessionInfo() const;
		//=============================�����ǶԼ�Ⱥ�ڲ�����֮�����Ϣ���ͺ���=========================//

		//=================================�����ǶԿͻ��˵���Ϣ������==============================//
		/**
		@brief: ��ͻ��˷�����Ϣ���ͻ���ID��SGateSessionInfo������
		*/
		bool				send(const SClientSessionInfo& sClientSessionInfo, const message_header* pData);
		/**
		@brief: ��һ���ͻ��˹㲥��Ϣ���ͻ���ID��std::vector<SGateSessionInfo>������
		*/
		bool				broadcast(const std::vector<SClientSessionInfo>& vecClientSessionInfo, const message_header* pData);
		/**
		@brief: ���ط���ת���ͻ�����Ϣ
		*/
		bool				forward(const std::string& szServiceName, uint64_t nSessionID, const message_header* pData);
	};
}