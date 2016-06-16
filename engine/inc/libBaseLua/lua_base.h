#pragma once

#include "libBaseCommon/profiling.h"
#include "libBaseCommon/base_function.h"
#include "libBaseCommon/base_exception.h"
#include "libBaseCommon/debug_helper.h"

#include "lua-5.3.3/lua.hpp"

namespace base
{
#define _WEAK_OBJECT_TBL_NAME	"weak_object_table"
#define _REF_OBJECT_TBL_NAME	"ref_object_table"
#define _FACADE_NAME			"facade"

	struct SObjectWraper
	{
		void*	pObject;	// �����Ķ���
		bool	bGc;		// true ��ʾ������������������ȫ����lua���Ƶģ�lua������lua���٣���false ��ʾ���������������ڲ�����lua���Ƶģ�һ���Ƿ���ֵ������ʲô�ģ����ֶ���Ӧ����lua�б��棬����ö����ɾ���ͻ���ֻ���
		int32_t	nRefCount;	// ��������������lua���ƵĶ��󣬸ñ�����ʾ��C++�������ü������������ڲ�����lua�����������ڵĶ��󣬸ñ���û������
	};

	template<class T>
	struct SClassName
	{
		static inline void setName(const char* szClassName)
		{
			s_szClassName = szClassName;
		}
		static inline const char* getName()
		{
			return s_szClassName;
		}

		static const char* s_szClassName;
	};
	template<class T>
	const char* SClassName<T>::s_szClassName;

	struct SStackCheck
	{
		int32_t		m_nTop;
		lua_State*	m_pL;

		SStackCheck(lua_State* pL)
		{
			this->m_pL = pL;
			this->m_nTop = lua_gettop(this->m_pL);
		}

		~SStackCheck()
		{
			int32_t nTop = lua_gettop(this->m_pL);
			if (nTop != this->m_nTop)
			{
				PrintWarning("lua stack error");
			}
		}
	};

	inline void printStack(lua_State* pL, int32_t n)
	{
		SStackCheck sStackCheck(pL);

		lua_Debug ar;
		if (lua_getstack(pL, n, &ar))
		{
			lua_getinfo(pL, "Sln", &ar);

			if (ar.name != NULL)
				PrintInfo("\tstack[%d] -> line %d : %s()[%s : line %d]", n, ar.currentline, ar.name, ar.short_src, ar.linedefined);
			else
				PrintInfo("\tstack[%d] -> line %d : unknown[%s : line %d]", n, ar.currentline, ar.short_src, ar.linedefined);

			printStack(pL, n + 1);
		}
	}

	// ��֧�ַ�ָ����󴫵�
	template<class T>
	struct SLua2CppType
	{
		inline static T convert(lua_State* pL, int32_t nIndex)
		{
#ifdef _WIN32
			// �����������﷨����ģ���Ҫȥ��
			err;
			// �����������﷨����ģ���Ҫȥ��
#else
			return T();
#endif
		}
	};

	template<class T>
	struct SLua2CppType<T*>
	{
		inline static T* convert(lua_State* pL, int32_t nIndex)
		{ 
			SObjectWraper* pObjectWraper = static_cast<SObjectWraper*>(lua_touserdata(pL, nIndex));
			DebugAstEx( pObjectWraper != nullptr, nullptr );
			return static_cast<T*>(pObjectWraper->pObject);
		}
	};

	template<class T> 
	inline T read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isuserdata(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(userdata)");
			return nullptr;
		}

		return SLua2CppType<T>::convert(pL, nIndex);
	};

	template<> inline void read2Cpp(lua_State* pL, int32_t nIndex) {}

	template<> inline bool read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isboolean(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(bool)");
			return false;
		}
		return lua_toboolean(pL, nIndex) ? true : false;
	}
	template<> inline char* read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isstring(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(char*)");
			return nullptr;
		}
		return (char*)lua_tostring(pL, nIndex);
	}
	template<> inline const char* read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isstring(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(const char*)");
			return nullptr;
		}
		return (const char*)lua_tostring(pL, nIndex);
	}
	template<> inline int8_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(int8)");
			return 0;
		}
		return (int8_t)lua_tointeger(pL, nIndex);
	}
	template<> inline uint8_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(uint8)");
			return 0;
		}
		return (uint8_t)lua_tointeger(pL, nIndex);
	}
	template<> inline int16_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(int16)");
			return 0;
		}
		return (int16_t)lua_tointeger(pL, nIndex);
	}
	template<> inline uint16_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(uint16)");
			return 0;
		}
		return (uint16_t)lua_tointeger(pL, nIndex);
	}
	template<> inline int32_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(int32)");
			return 0;
		}
		return (int32_t)lua_tointeger(pL, nIndex);
	}
	template<> inline uint32_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(uint32)");
			return 0;
		}
		return (uint32_t)lua_tointeger(pL, nIndex);
	}
	template<> inline int64_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		int64_t nRet = 0;
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(int64_t)");
			return 0;
		}
		return (int64_t)lua_tointeger(pL, nIndex);
	}
	template<> inline uint64_t read2Cpp(lua_State* pL, int32_t nIndex)
	{
		uint64_t nRet = 0;
		if (!lua_isinteger(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(uint64_t)");
			return 0;
		}
		return (uint64_t)lua_tointeger(pL, nIndex);
	}
	template<> inline float read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isnumber(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(float)");
			return 0.0f;
		}
		return (float)lua_tonumber(pL, nIndex);
	}
	template<> inline double read2Cpp(lua_State* pL, int32_t nIndex)
	{
		if (!lua_isnumber(pL, nIndex))
		{
			throw base::CBaseException("invalid lua arg(double)");
			return 0.0;
		}
		return (double)lua_tonumber(pL, nIndex);
	}

	template<class T>
	struct Convert2LuaType
	{
		inline static void convertType(lua_State* pL, T& val)
		{
			DebugAst("unsupport ref and value");
		}
	};

	template<class T>
	struct Convert2LuaType<const T*>
	{
		inline static void convertType(lua_State* pL, const T* val)
		{
			lua_getglobal(pL, _WEAK_OBJECT_TBL_NAME);
			int32_t tbl = lua_gettop(pL);
			///< ��ȡ�����еİ�������
			lua_pushlightuserdata(pL, (void*)val);
			lua_rawget(pL, -2);
			if (lua_isnil(pL, -1))
			{
				lua_pop(pL, 1);

				///< ��������İ����ڴ��
				SObjectWraper* pObjectWraper = static_cast<SObjectWraper*>(lua_newuserdata(pL, sizeof(SObjectWraper)));

				pObjectWraper->pObject = (void*)val;
				pObjectWraper->bGc = false;
				pObjectWraper->nRefCount = 0;
				luaL_getmetatable(pL, SClassName<T>::getName());
				///< ���ö����metatable
				lua_setmetatable(pL, -2);

				lua_pushlightuserdata(pL, (void*)val);
				lua_pushvalue(pL, -2);
				///< ���������ý������� weak_object_table[pObject] = pObjectWraper
				lua_rawset(pL, tbl);
			}
			else
			{
				SObjectWraper* pObjectWraper = static_cast<SObjectWraper*>(lua_touserdata(pL, -1));
				if (pObjectWraper == nullptr)
					lua_pushnil(pL);
				if (pObjectWraper->pObject == nullptr)
					lua_pushnil(pL);
			}

			lua_replace(pL, tbl);
			lua_settop(pL, tbl);
		}
	};

	template<class T>
	struct Convert2LuaType<T*>
	{
		inline static void convertType(lua_State* pL, T* val)
		{
			Convert2LuaType<const T*>::convertType(pL, val);
		}
	};

	template<class T>
	inline void push2Lua(lua_State* pL, T val)
	{
#ifdef _WIN32
		///< �����������﷨����ģ���Ҫȥ��
		if
			///< �����������﷨����ģ���Ҫȥ��
#endif
	}

	template<class T>
	inline void push2Lua(lua_State* pL, T* val)
	{
		Convert2LuaType<T*>::convertType(pL, val);
	};

	template<> inline void push2Lua(lua_State* pL, int8_t val)		{ lua_pushinteger(pL, val);	}
	template<> inline void push2Lua(lua_State* pL, uint8_t val)		{ lua_pushinteger(pL, val); }
	template<> inline void push2Lua(lua_State* pL, int16_t val)		{ lua_pushinteger(pL, val); }
	template<> inline void push2Lua(lua_State* pL, uint16_t val)	{ lua_pushinteger(pL, val); }
	template<> inline void push2Lua(lua_State* pL, int32_t val)		{ lua_pushinteger(pL, val); }
	template<> inline void push2Lua(lua_State* pL, uint32_t val)	{ lua_pushinteger(pL, val); }
	template<> inline void push2Lua(lua_State* pL, int64_t val)		{ lua_pushinteger(pL, val); }
	template<> inline void push2Lua(lua_State* pL, uint64_t val)	{ lua_pushinteger(pL, val); }
	template<> inline void push2Lua(lua_State* pL, float val)		{ lua_pushnumber(pL, val);	}
	template<> inline void push2Lua(lua_State* pL, double val)		{ lua_pushnumber(pL, val);	}
	template<> inline void push2Lua(lua_State* pL, char* val)		{ lua_pushstring(pL, val);	}
	template<> inline void push2Lua(lua_State* pL, const char* val) { lua_pushstring(pL, val);	}
	template<> inline void push2Lua(lua_State* pL, bool val)		{ lua_pushboolean(pL, val); }
}