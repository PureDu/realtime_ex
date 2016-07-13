#pragma once

#include "core_common.h"

namespace core
{

	class CBaseConnectionMgr;
	class CCoreConnection;

	/**
	@brief: ���ӻ����࣬��ܵ��������Ӷ��̳��������
	*/
	class CBaseConnection
	{
		friend class CBaseConnectionMgr;
		friend class CCoreConnection;

	public:
		CBaseConnection();
		virtual ~CBaseConnection();

		virtual bool			init(const std::string& szContext) { return true; }
		/**
		@brief: ��ȡ��������
		*/
		virtual uint32_t		getType() const = 0;
		/**
		@brief: �ͷŶ���
		*/
		virtual void			release() = 0;
		/**
		@brief: ������Ϣ
		*/
		void					send(uint8_t nMessageType, const void* pData, uint16_t nSize);
		/**
		@brief: ������Ϣ�����Դ��϶�������
		*/
		void					send(uint8_t nMessageType, const void* pData, uint16_t nSize, const void* pExtraBuf, uint16_t nExtraSize);
		/**
		@brief: �ر�����
		*/
		void					shutdown(bool bForce, const std::string& szMsg);
		/**
		@brief: �򿪹ر�����
		*/
		void					enableHeartbeat(bool bEnable);
		/**
		@brief: ��ȡ����ID
		*/
		uint64_t				getID() const;
		/**
		@brief: ��ȡ���ص�ַ
		*/
		const SNetAddr&			getLocalAddr() const;
		/**
		@brief: ��ȡԶ�̵�ַ
		*/
		const SNetAddr&			getRemoteAddr() const;
		/**
		@brief: ���ӳɹ��ص�
		*/
		virtual void			onConnect() = 0;
		/**
		@brief: ���ӶϿ��ص�
		*/
		virtual void			onDisconnect() = 0;
		/**
		@brief: ��Ϣ�ص�
		*/
		virtual bool			onDispatch(uint8_t nMessageType, const void* pData, uint16_t nSize) = 0;

	private:
		void					onHeartbeat(uint64_t nContext);

	private:
		bool		m_bHeartbeat;
		CTicker		m_heartbeat;
		uint32_t	m_nSendHeartbeatCount;

		uint64_t	m_nID;
		SNetAddr	m_sLocalAddr;
		SNetAddr	m_sRemoteAddr;
	};
}