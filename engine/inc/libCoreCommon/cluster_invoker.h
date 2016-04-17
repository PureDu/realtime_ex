#pragma once
#include "libBaseCommon/singleton.h"
#include "libCoreCommon/core_common.h"

#include "load_balance_policy.h"

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
		bool				invok(const std::string& szServiceName, uint16_t nMessageFormat, const message_header* pData);
		/**
		@brief: ����ָ��Զ�̷������Ϣ����Ҫ�ṩԶ�̷�����Ϣ���ص���Ӧ�����ص�
		*/
		bool				invok_r(const std::string& szServiceName, uint16_t nMessageFormat, const message_header* pData, InvokeCallback callback, uint64_t nContext = 0);
		/**
		@brief: ͨ����ϢID����Զ�̷�����Ҫ�ṩ���ؾ�������Զ�̷����÷�����Ϣ
		*/
		bool				invok(uint16_t nMessageFormat, const message_header* pData, ILoadBalancePolicy* pLoadBalancePolicy, uint64_t nLoadBalanceParam);
		/**
		@brief: ͨ����ϢID����Զ�̷�����Ҫ�ṩ���ؾ���������Ҫ�ṩԶ�̷�����Ϣ���ص���Ӧ�����ص�
		*/
		bool				invok_r(uint16_t nMessageFormat, const message_header* pData, ILoadBalancePolicy* pLoadBalancePolicy, uint64_t nLoadBalanceParam, InvokeCallback callback, uint64_t nContext = 0);
		/**
		@brief: ��ӦԶ�̷���ĵ��ã�������Ӧ��Ϣ
		*/
		void				response(uint16_t nMessageFormat, const message_header* pData);
		/**
		@brief: ��ӦԶ�̷���ĵ��ã�������Ӧ��Ϣ�������Ҫ�û��Լ��ƶ�����session����Ҫ���ڲ���������Ӧ����Ҫ�ȴ�������Ϣ����Ϣ
		*/
		void				response(const SServiceSessionInfo& sServiceSessionInfo, uint16_t nMessageFormat, const message_header* pData);
		/**
		@brief: ��ȡ��ǰ����session��Ϣ
		*/
		SServiceSessionInfo	getServiceSessionInfo() const;
		//=============================�����ǶԼ�Ⱥ�ڲ�����֮�����Ϣ���ͺ���=========================//

		//=================================�����ǶԿͻ��˵���Ϣ������==============================//
		/**
		@brief: ��ͻ��˷�����Ϣ���ͻ���ID��SGateSessionInfo������
		*/
		bool				send(const SClientSessionInfo& sGateSessionInfo, uint16_t nMessageFormat, const message_header* pData);
		/**
		@brief: ��һ���ͻ��˹㲥��Ϣ���ͻ���ID��std::vector<SGateSessionInfo>������
		*/
		bool				broadcast(const std::vector<SClientSessionInfo>& vecGateSessionInfo, uint16_t nMessageFormat, const message_header* pData);
		/**
		@brief: ���ط���ת���ͻ�����Ϣ
		*/
		bool				foward(uint64_t nSessionID, uint16_t nMessageFormat, const message_header* pData, ILoadBalancePolicy* pLoadBalancePolicy, uint64_t nLoadBalanceParam);
	};
}