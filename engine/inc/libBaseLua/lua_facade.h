#pragma once

#include "libBaseCommon/debug_helper.h"
#include "libBaseCommon/profiling.h"
#include "libBaseCommon/base_function.h"

#include "lua-5.3.3/lua.hpp"

namespace base
{

#ifdef _WIN32
#	ifdef __BUILD_BASE_LUA_DLL__
#		define __BASE_LUA_API__ __declspec(dllexport)
#	else
#		define __BASE_LUA_API__ __declspec(dllimport)
#	endif
#else
#	define __BASE_LUA_API__
#endif

	namespace lua_helper
	{
		__BASE_LUA_API__ int32_t	onError(lua_State* pL);
		
		template<class T>
		inline static int32_t		deleteObject(lua_State* pL);

		template<typename T, typename ...Args>
		inline static int32_t		createObject(lua_State* pL);
	}

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
		
		void		setActiveLuaState(lua_State* pL);

		// ������������Ҫ��������������lua���ƵĶ�����C++�������ʱ��Ϊ�˷�ֹlua gc�������������������ü���
		void		addObjectRef(void* pObject);
		void		delObjectRef(void* pObject);

		// �������ֻҪ��������������C++���ƵĶ�����C++�����ͷź�lua�п��ܻ��ж��������ĳ��У���Ҫ���ö����ΪNULL����ֹ����Ұָ��
		void		unbindObject(void* pObject);

		template<typename RT, typename ...Args>
		inline void	registerFunction(const char* szName, RT(*pfFun)(Args...));

		template<class T, class F>
		inline void	registerClass(const char* szClassName, F pfFun);

		template<class T, class M>
		inline void	registerClassMember(const char* szName, const M T::* pMember);
		template<class M>
		void		registerClassStaticMember(const char* szClassName, const char* szName, M* pMember);

		template<class T, typename RT, typename ...Args>
		inline void	registerClassFunction(const char* szName, RT(T::*pfFun)(Args...) const);

		template<class T, typename RT, typename ...Args>
		inline void	registerClassFunction(const char* szName, RT(T::*pfFun)(Args...));

		template<typename ...Args>
		inline bool	call(const char* szFunName, Args... args);

		template<typename RT, typename ...Args>
		inline bool	callR(const char* szFunName, RT& ret, Args... args);

		template<class T>
		inline void	registerConstData(const char* szName, const T& val);

	private:
		lua_State*	getActiveLuaState() const;

	private:
		lua_State*	m_pMainLuaState;
		lua_State*	m_pActiveLuaState;
	};
}

#include "lua_facade.inl"