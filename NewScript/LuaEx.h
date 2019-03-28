#pragma once

#include <lua.hpp>

#ifdef _DEBUG
#pragma comment(lib, "LuaLib535d.lib")
#else
#pragma comment(lib, "LuaLib535.lib")
#endif


class LuaEx
{
public:
	LuaEx();
	virtual ~LuaEx();

	BOOL Lua_Start(long hwnd, CString strText);

	lua_State* L;
	void Init();

};

