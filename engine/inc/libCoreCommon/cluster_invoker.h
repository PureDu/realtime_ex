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
		bool				invok(const std::string& szServiceName, const google::protobuf::Message* pMessage);
		/**
		@brief: ����ָ��Զ�̷������Ϣ����Ҫ�ṩԶ�̷�����Ϣ���ص���Ӧ�����ص�
		*/
		bool				invok_r(const std::string& szServiceName, const google::protobuf::Message* pMessage, InvokeCallback callback, uint64_t nContext = 0);
		/**
		@brief: ͨ����ϢID����Զ�̷�����Ҫ�ṩ���ؾ�������Զ�̷����÷�����Ϣ
		*/
		bool				invok(const google::protobuf::Message* pMessage, ILoadBalancePolicy* pLoadBalancePolicy, uint64_t nLoadBalanceParam);
		/**
		@brief: ͨ����ϢID����Զ�̷�����Ҫ�ṩ���ؾ���������Ҫ�ṩԶ�̷�����Ϣ���ص���Ӧ�����ص�
		*/
		bool				invok_r(const google::protobuf::Message* pMessage, ILoadBalancePolicy* pLoadBalancePolicy, uint64_t nLoadBalanceParam, InvokeCallback callback, uint64_t nContext = 0);
		/**
		@brief: ��ӦԶ�̷���ĵ��ã�������Ӧ��Ϣ
		*/
		void				response(const google::protobuf::Message* pMessage);
		/**
		@brief: ��ӦԶ�̷���ĵ��ã�������Ӧ��Ϣ�������Ҫ�û��Լ��ƶ�����session����Ҫ���ڲ���������Ӧ����Ҫ�ȴ�������Ϣ����Ϣ
		*/
		void				response(const SServiceSessionInfo& sServiceSessionInfo, const google::protobuf::Message* pMessage);
		/**
		@brief: ��ȡ��ǰ����session��Ϣ
		*/
		SServiceSessionInfo	getServiceSessionInfo() const;
		//=============================�����ǶԼ�Ⱥ�ڲ�����֮�����Ϣ���ͺ���=========================//

		//=================================�����ǶԿͻ��˵���Ϣ������==============================//
		/**
		@brief: ��ͻ��˷�����Ϣ���ͻ���ID��SGateSessionInfo������
		*/
		bool				send(const SClientSessionInfo& sGateSessionInfo, const google::protobuf::Message* pMessage);
		/**
		@brief: ��һ���ͻ��˹㲥��Ϣ���ͻ���ID��std::vector<SGateSessionInfo>������
		*/
		bool				broadcast(const std::vector<SClientSessionInfo>& vecGateSessionInfo, const google::protobuf::Message* pMessage);
		/**
		@brief: ���ط���ת���ͻ�����Ϣ
		*/
		bool				forward(uint64_t nSessionID, const message_header* pHeader, ILoadBalancePolicy* pLoadBalancePolicy, uint64_t nLoadBalanceParam);
	};
}