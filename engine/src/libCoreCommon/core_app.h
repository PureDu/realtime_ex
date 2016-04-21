#pragma once

#include "libBaseCommon/singleton.h"

#include "base_app.h"
#include "ticker_mgr.h"
#include "core_connection_mgr.h"
#include "service_mgr.h"
#include "transport.h"
#include "message_directory.h"
#include "load_balance_policy_mgr.h"

namespace core
{
	/**
	@brief: ���Ŀ���࣬��������Ҫ������Ϸ�и���������
	*/
	class CCoreApp :
		public base::CSingleton<CCoreApp>
	{
	public:
		CCoreApp();
		~CCoreApp();

		/**
		@brief: �������
		*/
		bool	run(bool bNormalService, int32_t argc, char** argv, const char* szConfig);
		/**
		@brief: ��ȡ���������Ϣ
		*/
		const SServiceBaseInfo&	
				getServiceBaseInfo() const;
		/**
		@brief: ע�ᶨʱ��
		nStartTime ��һ�δ�����ʱ����ʱ��
		nIntervalTime ��һ�δ�����ʱ�����������ʱ�������ļ��ʱ�䣬�����ֵ��0�ͱ�ʾ�����ʱ��ֻ����һ��
		*/
		void	registerTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext);
		/**
		@brief: ��ע�ᶨʱ��
		*/
		void	unregisterTicker(CTicker* pTicker);
		/**
		@brief: ��ȡ��ǰ�߼�ʱ��
		*/
		int64_t	getLogicTime() const;
		/*
		@brief: ��ȡ���ӹ�����
		*/
		CCoreConnectionMgr*		
				getCoreConnectionMgr() const;
		/*
		@brief: ��ȡ���������
		*/
		CServiceMgr*			
				getServiceMgr() const;
		/*
		@brief: ��ȡ��Ϣ������
		*/
		CTransport*			
				getTransport() const;
		/*
		@brief: ��ȡ��Ϣ�ֵ�
		*/
		CMessageDirectory*		
				getMessageDirectory() const;
		/*
		@brief: ��ȡ�����ļ���
		*/
		const std::string&		
				getConfigFileName() const;
		/*
		@brief: ��ȡдbuf���󣬵���Ҫ������Ϣ���
		*/
		base::CWriteBuf&		
				getWriteBuf() const;
		/*
		@brief: ע�Ḻ�ؾ�����
		*/
		void	registLoadBalancePolicy(ILoadBalancePolicy* pLoadBalance);
		/*
		@brief: ��ȡ���ؾ�����
		*/
		ILoadBalancePolicy*		
				getLoadBalancePolicy(uint32_t nID) const;
		/*
		@brief: ������ϢID��ȡ����֧�ָ���Ϣ�ķ�������
		*/
		const std::vector<std::string>&
				getServiceName(const std::string& szMessageName) const;
		/*
		@brief: �˳����
		*/
		void					doQuit();

	private:
		bool					onInit();
		bool					onProcess();
		void					onDestroy();
		void					onAnalyze();

	protected:
		std::string				m_szConfig;
		std::string				m_szPID;
		CTickerMgr*				m_pTickerMgr;
		CCoreConnectionMgr*		m_pCoreConnectionMgr;
		CServiceMgr*			m_pServiceMgr;
		CTransport*				m_pTransport;
		CMessageDirectory*		m_pMessageDirectory;
		CLoadBalancePolicyMgr*	m_pLoadBalancePolicyMgr;
		SServiceBaseInfo		m_sServiceBaseInfo;
		base::CWriteBuf			m_writeBuf;
		uint32_t				m_nCycleCount;
		uint32_t				m_nTotalTime;
		volatile uint32_t		m_nRunState;
		bool					m_bNormalService;
		bool					m_bMarkQuit;	// �������������ֻ����һ��onQuit
	};
}