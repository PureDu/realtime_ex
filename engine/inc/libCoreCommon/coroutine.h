#pragma once
#include "libBaseCommon/base_common.h"

#include <functional>

namespace core
{
	enum ECoroutineState
	{
		eCS_DEAD,	// ����״̬
		eCS_READY,	// �ȴ�ִ�У�������û�б�ִ�й���
		eCS_RUNNING,// ִ��״̬
		eCS_SUSPEND,// ����״̬
	};

	namespace coroutine
	{
		/**
		@brief: ����Э�̣�����Э����ں���
		*/
		uint64_t	start(std::function<void(uint64_t)> fn);
		/**
		@brief: �ָ�ĳһ��Э��ִ�У����Դ�һ��������yield�������غ��ȡ�������Э�̵�һ��ִ�о�����ں����Ĳ���
		*/
		void		resume(uint64_t nID, uint64_t nContext);
		/**
		@brief: ����ǰִ�е�Э�̣���Э���´λ�ִ��ʱ�᷵��resume�����������Ĳ���
		*/
		uint64_t	yield();
		/**
		@brief: ��ȡЭ��״̬
		*/
		uint32_t	getState(uint64_t nID);
		/**
		@brief: ��ָ��Э�̷�����Ϣ
		*/
		void		sendMessage(uint64_t nID, void* pData);
		/**
		@brief: ȡָ��Э�̽�����Ϣ
		*/
		void*		recvMessage(uint64_t nID);
		/**
		@brief: ��ȡ��ǰִ��Э�̵�ID
		*/
		uint64_t	getCurrentID();
	};
}