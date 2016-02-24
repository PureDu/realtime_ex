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

	// ��ʱ������֧�ֿ�������Ϊ����ʱ����ж�ʱ��Ϊ�������Ϊ�Ǵ���ȥ�ػ��ǿ���һ���յĶ�ʱ���أ���������ֱ�Ӳ�������
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

		int64_t		getIntervalTime() const;
		int64_t		getNextTickTime() const;
		int64_t		getRemainTime() const;
		bool		isRegist() const;
		uint64_t	getContext() const;

		// C�����ص�
		void		setCallback(void(*callback)(uint64_t))
		{
			DebugAst(callback != nullptr);
			DebugAst(sizeof(this->m_pRawCallback) == sizeof(callback));

			memcpy(&this->m_pRawCallback, &callback, sizeof(callback));
			this->setCallback(std::bind(callback, std::placeholders::_1));
		}

		// C++��Ա�����ص�
		template<typename T>
		void		setCallback(void(T::*callback)(uint64_t), T* pObject)
		{
			DebugAst(callback != nullptr && pObject != nullptr);
			
			// ��Щ�����callback��С������void*��С����Ϊ�漰��thisָ����и�
			void* pAddr[2] = { nullptr };
			memcpy(pAddr, &callback, sizeof(callback));
			this->m_pRawCallback = pAddr[0];

			this->setCallback(std::bind(callback, pObject, std::placeholders::_1));
		}

	private:
		void		setCallback(const std::function<void(uint64_t)>& callback);

	private:
		TickerNode_t*					m_pTickerNode;
		int64_t							m_nIntervalTime;	// ��ʱ�����еļ��ʱ��
		int64_t							m_nNextTickTime;	// ��һ�ζ�ʱ������ʱ��
		uint64_t						m_nContext;
		std::function<void(uint64_t)>	m_callback;
		void*							m_pRawCallback;
	};
}