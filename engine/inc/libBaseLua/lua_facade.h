#pragma once

#include "base_lua.h"

namespace base
{
	class __BASE_LUA_API__ CLuaFacade
	{
	public:
		CLuaFacade();
		~CLuaFacade();

		bool		open();
		void		close();
		bool		loadFile(const char* szName);
		bool		runString(const char* szBuf, const char* szBufName);
		void		addSeachPath(const char* szPath);
		bool		startDebug(const char* szIP, uint16_t nPort);
		void		updateDebug();
		
		lua_State*	getState() const;

		void		setActiveLuaState(lua_State* pL);
		lua_State*	getActiveLuaState() const;

		// ������������Ҫ��������������lua���ƵĶ�����C++�������ʱ��Ϊ�˷�ֹlua gc�������������������ü���
		void		addObjectRef(void* pObject);
		void		delObjectRef(void* pObject);

		// �������ֻҪ��������������C++���ƵĶ�����C++�����ͷź�lua�п��ܻ��ж��������ĳ��У���Ҫ���ö����ΪNULL����ֹ����Ұָ��
		void		unbindObject(void* pObject);

	private:
		lua_State*	m_pMainLuaState;
		lua_State*	m_pActiveLuaState;
	};
}