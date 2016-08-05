#pragma once

#include "libCoreCommon/base_connection.h"

namespace core
{
	class CCoreConnectionToService
		: public core::CBaseConnection
	{
	public:
		CCoreConnectionToService();
		virtual ~CCoreConnectionToService();

		virtual bool		init(const std::string& szContext);
		virtual uint32_t	getType() const;
		virtual void		release();

		virtual void		onConnect();
		virtual void		onDisconnect();
		virtual bool		onDispatch(uint8_t nMessageType, const void* pData, uint16_t nSize);

		uint16_t			getServiceID() const;

	private:
		uint16_t m_nServiceID;
	};
}