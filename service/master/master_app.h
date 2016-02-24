#pragma once
#include "libCoreCommon/core_app.h"

#include "service_mgr.h"

class CMasterApp
	: public core::CCoreApp
{
public:
	CMasterApp();
	virtual ~CMasterApp();

	static CMasterApp* Inst();

	virtual bool	onInit();
	virtual void	onDestroy();

	virtual void	onQuit();

	CServiceMgr*	getServiceMgr() const;

private:
	CServiceMgr*	m_pServiceMgr;
};