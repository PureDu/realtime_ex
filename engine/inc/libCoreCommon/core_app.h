#pragma once

#include "tinyxml2/tinyxml2.h"

#include "core_common.h"
#include "ticker.h"
#include "base_connection_mgr.h"

namespace core
{
	// ������״̬ eARS_Start->eARS_Normal->eARS_Quitting->eARS_Quit
	// ����eARS_Quitting״̬�л���eARS_Quit�����߼�����������doQuit����ɵģ�
	// ��������֤�˹ط�ǰ��һЩ��Ҫ�Ƚϳ���ʱ����ȷ��һЩ�µ���Ϊ���������ݴ洢
	enum EAppRunState
	{
		eARS_Start		= 0,	// ����״̬
		eARS_Normal		= 1,	// ����״̬
		eARS_Quitting	= 2,	// �˳���
		eARS_Quit		= 3,	// �����˳�
	};

	class CCoreConnectionMgr;
	class CTickerMgr;
	class CMessagePort;
	/**
	@brief: ��������࣬��������Ҫ������Ϸ�и���������
	*/
	class CCoreApp
	{
	public:
		CCoreApp();
		virtual ~CCoreApp();

		static CCoreApp*& Inst();

		/**
		@brief: �������
		*/
		bool					run(int32_t argc, char** argv, const char* szConfig);
		/**
		@brief: ��ȡ���������Ϣ
		*/
		const SServiceBaseInfo&	getServiceBaseInfo() const;
		/**
		@brief: ע�ᶨʱ��
		nStartTime ��һ�δ�����ʱ����ʱ��
		nIntervalTime ��һ�δ�����ʱ�����������ʱ�������ļ��ʱ�䣬�����ֵ��0�ͱ�ʾ�����ʱ��ֻ����һ��
		*/
		void					registTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext);
		/**
		@brief: ��ע�ᶨʱ��
		*/
		void					unregistTicker(CTicker* pTicker);
		/**
		@brief: ��ȡ��ǰ�߼�ʱ��
		*/
		int64_t					getLogicTime() const;
		/*
		@brief: ��ȡ���ӹ�����
		*/
		CBaseConnectionMgr*		getBaseConnectionMgr() const;
		/*
		@brief: ��ȡ��Ϣ�ʾ�
		*/
		CMessagePort*			getMessagePort() const;
		
	protected:
		virtual bool			onInit();
		virtual bool			onProcess();
		virtual void			onDestroy();

		virtual void			onBeforeFrame() { }
		virtual void			onAfterFrame() { }

		virtual	void			onQuit() = 0;
		void					doQuit();

	private:
		void					onAnalyze();

	protected:
		tinyxml2::XMLElement*		m_pRootXML;
		CTickerMgr*					m_pTickerMgr;
		CCoreConnectionMgr*			m_pCoreConnectionMgr;
		CMessagePort*				m_pMessagePort;
		SServiceBaseInfo			m_sServiceBaseInfo;
		uint32_t					m_nCycleCount;
		uint32_t					m_nTotalTime;
		volatile uint32_t			m_nRunState;
		bool						m_bMarkQuit;	// �������������ֻ����һ��onQuit
	};
}