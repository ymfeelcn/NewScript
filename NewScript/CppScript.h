#pragma once

#ifndef MyScript_CppScript_h
#define MyScript_CppScript_h

#include "stdafx.h"
#include <lua.hpp>
#include "Script.h"

#define CppScriptName "dm.Script"

#pragma warning(disable : 4244)

int newScript(lua_State* L)
{
	Script **s = (Script**)lua_newuserdata(L, sizeof(Script*));  // lua will manage Student** pointer
	
	// 在这里进行脚本的初始化
	lua_getglobal(L, "hwnd");
	long hwnd = lua_tointeger(L, -1);
	lua_pop(L, 1);
/*	long hwnd = 0;*/
	
	*s = new Script(hwnd);

	luaL_getmetatable(L, CppScriptName);
	lua_setmetatable(L, -2);
	return 1;
}

int L_ver(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	lua_pushstring(L, (*s)->dm.Ver());
	
	return 1;
}

int L_GetConfigValue(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	CString strKey = lua_tolstring(L, 2, NULL);
	CString strValue = (*s)->GetConfigValue(strKey);
	lua_pushstring(L, strValue);
	return 1;
}

int L_GetConfigValueInt(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	CString strKey = lua_tolstring(L, 2, NULL);
	int nValue = (*s)->GetConfigValueInt(strKey);
	lua_pushinteger(L, nValue);
	return 1;
}

int L_FindPic(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString pic_name = lua_tolstring(L, 6, NULL);
	CString color_format = lua_tolstring(L, 7, NULL);
	double sim = lua_tonumber(L, 8);
	int dir = lua_tointeger(L, 9);

	VARIANT intX, intY;
	int nRet = (*s)->dm.FindPic(x1, y1, x2, y2, pic_name, color_format, sim, dir, &intX, &intY);
	lua_pushinteger(L, nRet);
	lua_pushinteger(L, intX.intVal);
	lua_pushinteger(L, intY.intVal);

	return 3;
}

int L_FindPicEx(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString pic_name = lua_tolstring(L, 6, NULL);
	CString color_format = lua_tolstring(L, 7, NULL);
	double sim = lua_tonumber(L, 8);
	int dir = lua_tointeger(L, 9);

	CString strText = (*s)->dm.FindPicEx(x1, y1, x2, y2, pic_name, color_format, sim, dir);
	lua_pushstring(L, strText);

	return 1;
}

int L_Ocr(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString color_format = lua_tolstring(L, 6, NULL);
	double sim = lua_tonumber(L, 7);

	CString strText = (*s)->dm.Ocr(x1, y1, x2, y2, color_format, sim);
	lua_pushstring(L, strText);

	return 1;
}

int L_OcrEx(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString color_format = lua_tolstring(L, 6, NULL);
	double sim = lua_tonumber(L, 7);

	CString strText = (*s)->dm.OcrEx(x1, y1, x2, y2, color_format, sim);
	lua_pushstring(L, strText);

	return 1;
}

int L_FindStr(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString font_name = lua_tolstring(L, 6, NULL);
	CString color_format = lua_tolstring(L, 7, NULL);
	double sim = lua_tonumber(L, 8);

	VARIANT intX, intY;
	int nRet = (*s)->dm.FindStr(x1, y1, x2, y2, font_name, color_format, sim, &intX, &intY);
	lua_pushinteger(L, nRet);
	lua_pushinteger(L, intX.intVal);
	lua_pushinteger(L, intY.intVal);

	return 3;
}

int L_FindStrFast(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString font_name = lua_tolstring(L, 6, NULL);
	CString color_format = lua_tolstring(L, 7, NULL);
	double sim = lua_tonumber(L, 8);

	VARIANT intX, intY;
	int nRet = (*s)->dm.FindStrFast(x1, y1, x2, y2, font_name, color_format, sim, &intX, &intY);
	lua_pushinteger(L, nRet);
	lua_pushinteger(L, intX.intVal);
	lua_pushinteger(L, intY.intVal);

	return 3;
}

int L_FindStrEx(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString font_name = lua_tolstring(L, 6, NULL);
	CString color_format = lua_tolstring(L, 7, NULL);
	double sim = lua_tonumber(L, 8);

	CString strText = (*s)->dm.FindStrEx(x1, y1, x2, y2, font_name, color_format, sim);
	lua_pushstring(L, strText);

	return 1;
}

int L_FindColor(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString color_format = lua_tolstring(L, 6, NULL);
	double sim = lua_tonumber(L, 7);
	int dir = lua_tointeger(L, 8);

	VARIANT intX, intY;
	int nRet = (*s)->dm.FindColor(x1, y1, x2, y2, color_format, sim, dir, &intX, &intY);
	lua_pushinteger(L, nRet);
	lua_pushinteger(L, intX.intVal);
	lua_pushinteger(L, intY.intVal);

	return 3;
}

int L_FindColorEx(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x1 = lua_tointeger(L, 2);
	int y1 = lua_tointeger(L, 3);
	int x2 = lua_tointeger(L, 4);
	int y2 = lua_tointeger(L, 5);
	CString color_format = lua_tolstring(L, 6, NULL);
	double sim = lua_tonumber(L, 7);
	int dir = lua_tointeger(L, 8);

	CString strText = (*s)->dm.FindColorEx(x1, y1, x2, y2, color_format, sim, dir);
	lua_pushstring(L, strText);

	return 1;
}

int L_MoveTo(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int nRet = (*s)->dm.MoveTo(x, y);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_LeftClick(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int nRet = (*s)->dm.LeftClick();
	lua_pushinteger(L, nRet);
	return 1;
}

int L_LeftDoubleClick(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int nRet = (*s)->dm.LeftDoubleClick();
	lua_pushinteger(L, nRet);
	return 1;
}

int L_RightClick(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int nRet = (*s)->dm.RightClick();
	lua_pushinteger(L, nRet);
	return 1;
}

int L_CmpColor(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);

	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	CString color = lua_tolstring(L, 4, NULL);
	double sim = lua_tonumber(L, 5);

	int nRet = (*s)->dm.CmpColor(x, y, color, sim);
	lua_pushinteger(L, nRet);

	return 1;
}

int L_KeyPress(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int key = lua_tointeger(L, 2);
	int nRet = (*s)->dm.KeyPress(key);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_KeyPressChar(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	CString key = lua_tolstring(L, 2, NULL);
	int nRet = (*s)->dm.KeyPressChar(key);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_KeyPressStr(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	CString keys = lua_tolstring(L, 2, NULL);
	int delay = lua_tointeger(L, 3);
	int nRet = (*s)->dm.KeyPressStr(keys,delay);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_SendString(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	CString strText = lua_tolstring(L, 2, NULL);
	int nRet = (*s)->dm.SendString((*s)->hwnd, strText);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_SendString2(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	CString strText = lua_tolstring(L, 2, NULL);
	int nRet = (*s)->dm.SendString2((*s)->hwnd, strText);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_GetWindowState(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int flag = lua_tointeger(L, 2);
	int nRet = (*s)->dm.GetWindowState((*s)->hwnd, flag);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_SetWindowState(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int flag = lua_tointeger(L, 2);
	int nRet = (*s)->dm.SetWindowState((*s)->hwnd, flag);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_UseDict(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int index = lua_tointeger(L, 2);
	int nRet = (*s)->dm.UseDict(index);
	lua_pushinteger(L, nRet);
	return 1;
}

int L_Delay(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	int nTime = lua_tointeger(L, 2);
	(*s)->Delay(nTime);
	return 0;
}

int L_IsStop(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	lua_pushinteger(L, *(*s)->is_stop);
	return 1;
}

int L_IsPause(lua_State* L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	lua_pushinteger(L, *(*s)->is_pause);
	return 1;
}

static int script_auto_gc(lua_State *L)
{
	Script **s = (Script**)luaL_checkudata(L, 1, CppScriptName);
	if (s)
	{
		delete *s;
	}

	return 0;
}

static const struct luaL_Reg scriptlib_f[] = {
	{"create", newScript},
	{NULL, NULL}
};
static const struct luaL_Reg scriptlib_m[] = {
	{"ver", L_ver},
	{"FindStr", L_FindStr},
	{"FindStrFast", L_FindStrFast},
	{"FindStrEx", L_FindStrEx},
	{"FindPic", L_FindPic},
	{"FindPicEx", L_FindPicEx},
	{"FindColor", L_FindColor},
	{"FindColorEx", L_FindColorEx},
	{"Ocr", L_Ocr},
	{"OcrEx", L_OcrEx},
	{"LeftClick", L_LeftClick},
	{"LeftDoubleClick", L_LeftDoubleClick},
	{"RightClick", L_RightClick},
	{"MoveTo", L_MoveTo},
	{"CmpColor", L_CmpColor},
	{"KeyPress", L_KeyPress},
	{"KeyPressChar", L_KeyPressChar},
	{"KeyPressStr", L_KeyPressStr},
	{"SendString", L_SendString},
	{"SendString2", L_SendString2},
	{"GetWindowState", L_GetWindowState},
	{"SetWindowState", L_SetWindowState},
	{"UseDict", L_UseDict},
	{"Delay", L_Delay},
	{"IsStop", L_IsStop},
	{"IsPause", L_IsPause},
	{"__gc", script_auto_gc},
	{NULL, NULL}
};

int luaopen_script(lua_State *L) {
	luaL_newmetatable(L, CppScriptName);
	/* metatable.__index = metatable */
	lua_pushvalue(L, -1);		/* duplicates the metatable */
	lua_setfield(L, -2, "__index");

	luaL_setfuncs(L, scriptlib_m, 0);

	luaL_newlib(L, scriptlib_f);
	return 1;
}

#endif