#pragma once

#ifndef MyCppGame_CppLibs_h
#define MyCppGame_CppLibs_h

#include "stdafx.h"
#include <lua.hpp>

static int lua_Messagebox(lua_State *L)						// Êä³ö
{
	lua_getglobal(L, "myname");
	CString chName = lua_tostring(L, -1);
	lua_pop(L, -1);
	AfxMessageBox(chName);
	return 0;
}

static int lua_Msgbox(lua_State *L)						// Êä³ö
{
	CString strText = lua_tolstring(L, 1, NULL);
	AfxMessageBox(strText);
	return 0;
}

static const luaL_Reg myLibs[] =
{
	{"lua_Messagebox", lua_Messagebox},
	{"Msgbox", lua_Msgbox},
	{NULL, NULL}
};

static int luaopen_mylib(lua_State *L)
{
	luaL_newlib(L, myLibs);
	return 1;
};

#endif