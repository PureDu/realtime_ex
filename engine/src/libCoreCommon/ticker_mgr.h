#pragma once

#include "ticker.h"

namespace core
{
	class CTickerMgr :
		public base::noncopyable
	{
	private:
		enum
		{
			__TIME_WHEEL_SIZE		= 0x00010000,	///< �̶ȳߵĳ��� �󲿷�������������Ӧ���㹻�ˣ����ٻ��ж�ʱ���ȳ���2^16��65536��ms�ģ���ʹ����Ҳû��
			__TIME_WHEEL_MASK		= 0x0000ffff,	///< ��ʱʱ������
			__TIME_WHEEL_PRECISION	= 10,			///< ����10ms
		};

	private:
		base::CTinyList<TickerNode_t>	m_listTicker[__TIME_WHEEL_SIZE];		///< ������ʱ�����ڶ�Ӧ�̶ȵ��б��ϣ� ������������ʱ�����2^16ms�Ķ�ʱ���������ͨ�����ϣ���Ҫ������
		std::vector<TickerNode_t*>		m_vecTempTickerNode;
		int64_t							m_nLogicTime;							///< ��ǰ�̶�ʱ��

	public:
		CTickerMgr();
		~CTickerMgr();

		void				insertTicker(TickerNode_t* pTickerNode);
		void				registTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext);
		void				unregistTicker(CTicker* pTicker);
		void				update();
		inline uint64_t		getLogicTime() const { return this->m_nLogicTime; }
		uint32_t			getNearestTime() const;
	};
}