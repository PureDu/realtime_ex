#pragma once

#include "core_common.h"
#include "base_object.h"

namespace core
{

	class CBaseConnectionMgr;
	class CCoreConnection;

	/**
	@brief: ���ӻ����࣬��ܵ��������Ӷ��̳��������
	*/
	class CBaseConnection :
		public CBaseObject
	{
		friend class CBaseConnectionMgr;
		friend class CCoreConnection;

	public:
		CBaseConnection();
		virtual ~CBaseConnection();
		/**
		@brief: ������Ϣ
		*/
		void					send(uint16_t nMessageType, const void* pData, uint16_t nSize);
		/**
		@brief: ������Ϣ�����Դ��϶�������
		*/
		void					send(uint16_t nMessageType, const void* pData, uint16_t nSize, const void* pExtraBuf, uint16_t nExtraSize);
		/**
		@brief: �ر�����
		*/
		void					shutdown(bool bForce, const std::string& szMsg);
		/**
		@brief: ��ȡ����ID
		*/
		uint64_t				getID() const;
		/**
		@brief: ���ӳɹ��ص�
		*/
		virtual void			onConnect(const std::string& szContext) { }
		/**
		@brief: ���ӶϿ��ص�
		*/
		virtual void			onDisconnect() { }
		/**
		@brief: ��Ϣ�ص�
		*/
		virtual void			onDispatch(uint16_t nMessageType, const void* pData, uint16_t nSize) { }
		/**
		@brief: ��ȡ���ص�ַ
		*/
		const SNetAddr&			getLocalAddr() const;
		/**
		@brief: ��ȡԶ�̵�ַ
		*/
		const SNetAddr&			getRemoteAddr() const;

	private:
		CCoreConnection*	m_pCoreConnection;
	};
}