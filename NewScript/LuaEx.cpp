#include "StdAfx.h"
#include "LuaEx.h"
#include "CppLibs.h"
#include "CppClass.h"
#include "CppScript.h"
#include "CppCrypt.h"

LuaEx::LuaEx()
{
	Init();
}


LuaEx::~LuaEx()
{
	
}

static int l_my_print(lua_State* L)
{
	int nargs = lua_gettop(L);
	for (int i = 1; i <= nargs; i++)
	{
		LogTrace("%s\n",lua_tolstring(L, i, NULL));
	}
	return 0;
}
// 打印出错误位置
static CString stackDump(lua_State* L)
{
	int stackSize = lua_gettop(L);

	CString allInfo = "";
	for (int index = 1; index <= stackSize; index++)
	{
		int t = lua_type(L, index);
		CString strInfo;
		switch (t)
		{
		case LUA_TSTRING:
		{
			strInfo = lua_tostring(L, index);
			break;
		}
		case LUA_TBOOLEAN:
		{
			strInfo = lua_toboolean(L, index) ? "true" : "false";
			break;
		}
		case LUA_TNUMBER:
		{
			int result = lua_tonumber(L, index);
			strInfo.Format("%d", result);
			break;
		}
		default:
		{
			strInfo = lua_typename(L, index);
			break;
		}
		};
		allInfo += strInfo + "\n";
		printf("%s\n", strInfo);
	}
	return allInfo;
}

void LuaEx::Init()
{
	L = luaL_newstate();
	// 加载普通的类,print等
	luaL_openlibs(L);
	static const luaL_Reg lualibs[] =
	{
		{ "base", luaopen_base},
		{ "string", luaopen_string},
		{ "math", luaopen_math},
		{ "mylib", luaopen_mylib},
		{ "crypt", luaopen_mycrypt},
		//{ "dm", luaopen_script},
		{ NULL, NULL}
	};

	const luaL_Reg *lib = lualibs;
	for (; lib->func != NULL; lib++)
	{
		//lib->func(L);
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_settop(L, 0);
		//lua_pop(L, 1);
	}
	lua_register(L, "print", l_my_print);
	//lua_pop(L, 1);
}

BOOL LuaEx::Lua_Start(long hwnd, CString strScriptName)
{
	BOOL bRet = FALSE;

	// 设置全局窗口句柄
	lua_pushnumber(L, hwnd);
	lua_setglobal(L, "hwnd");

	int status = luaL_dofile(L, strScriptName);

	int result = 0;
	if (status == LUA_OK)
	{
		result = lua_pcall(L, 0, LUA_MULTRET, 0);
		LogTrace("脚本结束：%d\n", result);
		bRet = TRUE;
	}
	else
	{
		CString errInfo = stackDump(L);
		LogTrace("Lua Error：%s\n", errInfo);
		result = lua_pcall(L, 0, LUA_MULTRET, 0);
		LogTrace("加载LUA脚本出错：%d %s\n", result, lua_tostring(L,-1));
	}

	if (lua_gettop(L) > 1) {
		//cout << lua_tostring(L, -1) << endl;
		LogTrace("脚本结束：%s\n", lua_tostring(L, -1));
	}

	lua_gc(L, LUA_GCCOLLECT, 0); // assertion failed
	lua_close(L);

	return bRet;
}