#pragma once
#include "libBaseCommon/singleton.h"
#include "libCoreCommon/core_common.h"

#include "core_service_kit_define.h"

namespace core
{
	class CClusterInvoker :
		public base::CSingleton<CClusterInvoker>
	{
	public:
		CClusterInvoker();
		~CClusterInvoker();

		bool				init();

		/**
		@brief: ����ָ��Զ�̽ڵ����Ϣ��Զ�̽ڵ㲻�÷�����Ϣ
		*/
		bool				invoke(uint16_t nNodeID, const message_header* pData);
		/**
		@brief: ����ָ��Զ�̽ڵ����Ϣ��Զ�̽ڵ㲻�÷�����Ϣ
		*/
		bool				invoke(const std::string& szNodeName, const message_header* pData);

		/**
		@brief: ����ָ��Զ�̽ڵ����Ϣ����Ҫ�ṩԶ�̽ڵ���Ϣ���ص���Ӧ�����ص�,��promise����ʽ�ṩ
		*/
		template<class T>
		inline bool			invoke_r(uint16_t nNodeID, const message_header* pData, CFuture<std::shared_ptr<T>>& sFuture);
		/**
		@brief: ����ָ��Զ�̽ڵ����Ϣ����Ҫ�ṩԶ�̽ڵ���Ϣ���ص���Ӧ�����ص�,��promise����ʽ�ṩ
		*/
		template<class T>
		inline bool			invoke_r(const std::string& szNodeName, const message_header* pData, CFuture<std::shared_ptr<T>>& sFuture);

		/**
		@brief: ����ָ��Զ�̽ڵ����Ϣ����Ҫ�ṩԶ�̽ڵ���Ϣ���ص���Ӧ�����ص�
		*/
		template<class T>
		inline bool			invoke_r(uint16_t nNodeID, const message_header* pData, const std::function<void(std::shared_ptr<T>, uint32_t)>& callback);
		/**
		@brief: ����ָ��Զ�̽ڵ����Ϣ����Ҫ�ṩԶ�̽ڵ���Ϣ���ص���Ӧ�����ص�
		*/
		template<class T>
		inline bool			invoke_r(const std::string& szNodeName, const message_header* pData, const std::function<void(std::shared_ptr<T>, uint32_t)>& callback);
		
		/**
		@brief: ��ӦԶ�̽ڵ�ĵ��ã�������Ӧ��Ϣ
		*/
		void				response(const message_header* pData);
		/**
		@brief: ��ӦԶ�̽ڵ�ĵ��ã�������Ӧ��Ϣ�������Ҫ�û��Լ��ƶ��ڵ�session����Ҫ���ڲ���������Ӧ����Ҫ�ȴ�������Ϣ����Ϣ
		*/
		void				response(const SNodeSessionInfo& sServiceSessionInfo, const message_header* pData);
		/**
		@brief: ��ȡ��ǰ�ڵ�session��Ϣ
		*/
		SNodeSessionInfo	getServiceSessionInfo();
		//=============================�����ǶԼ�Ⱥ�ڲ��ڵ�֮�����Ϣ���ͺ���=========================//

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
		@brief: ���ؽڵ�ת���ͻ�����Ϣ
		*/
		bool				forward(uint16_t nNodeID, uint64_t nSessionID, const message_header* pData);
		/**
		@brief: ���ؽڵ�ת���ͻ�����Ϣ
		*/
		bool				forward_a(uint64_t nActorID, uint64_t nSessionID, const message_header* pData);
		/**
		@brief: ���ؽڵ�ת���ͻ�����Ϣ
		*/
		bool				forward(const std::string& szNodeName, uint64_t nSessionID, const message_header* pData);
		
	private:
		bool				invokeImpl(uint16_t nNodeID, const message_header* pData, const std::function<void(std::shared_ptr<message_header>, uint32_t)>& callback);
	};
}

#include "cluster_invoker.inl"