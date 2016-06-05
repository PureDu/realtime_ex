#pragma once

#include <vector>

#include "libBaseCommon/debug_helper.h"
#include "libBaseCommon/tiny_list.h"
#include "libBaseCommon/noncopyable.h"
#include "libBaseCommon/noninheritable.h"

namespace core
{
	class CTicker;
	struct STickerNodeInfo
	{
		CTicker*	pTicker;
	};
	typedef base::STinyListNode<STickerNodeInfo> TickerNode_t;

	// ��ʱ������֧�ֿ�������Ϊ����ʱ����ж�ʱ��Ϊ�������Ϊ�Ǵ���ȥ�ػ��ǿ���һ���յĶ�ʱ���أ�����ֱ����move�ķ�ʽ
	// ���������п���������ô����Ҫ��ָ�����ʽ����CTicker���󣬵�������һ����Ҫ���Լ���������ڴ棬�������ɾ������ʱ���������ܵģ��ܿ��ܾͱ�����
	// ���ｨ����ָ�����ʽ����CTicker������std::unique_ptr����
	class CTicker :
		public virtual base::noninheritable<CTicker>,
		public base::noncopyable
	{
		friend class CTickerMgr;
		
	public:
		CTicker();
		~CTicker();

		CTicker(CTicker&& rhs);

		int64_t		getIntervalTime() const;
		int64_t		getNextTickTime() const;
		int64_t		getRemainTime() const;
		bool		isRegister() const;
		uint64_t	getContext() const;
		void		setCallback(const std::function<void(uint64_t)>& callback);

	private:
		TickerNode_t*					m_pTickerNode;
		int64_t							m_nIntervalTime;	// ��ʱ�����еļ��ʱ��
		int64_t							m_nNextTickTime;	// ��һ�ζ�ʱ������ʱ��
		uint64_t						m_nContext;
		std::function<void(uint64_t)>	m_callback;
	};
}