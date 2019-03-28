#pragma once

//
//  CppClass.h
//  MyCppGame
//
//  Created by guanghui on 8/11/14.
//
//

#ifndef MyCppGame_CppClass_h
#define MyCppGame_CppClass_h

#include "stdafx.h"
#include <lua.hpp>
#include "Student.h"

#pragma warning(disable : 4244)


int newStudent(lua_State* L)
{
	Student **s = (Student**)lua_newuserdata(L, sizeof(Student*));  // lua will manage Student** pointer
	*s = new Student;
	luaL_getmetatable(L, "cc.Student");
	lua_setmetatable(L, -2);
	return 1;
}

int l_setName(lua_State* L)
{
	Student **s = (Student**)luaL_checkudata(L, 1, "cc.Student");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");

	luaL_checktype(L, -1, LUA_TSTRING);

	CString name = lua_tostring(L, -1);
	(*s)->setName(name);
	return 0;
}

int l_setAge(lua_State* L)
{
	Student **s = (Student**)luaL_checkudata(L, 1, "cc.Student");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	luaL_checktype(L, -1, LUA_TNUMBER);
	int age = lua_tonumber(L, -1);
	(*s)->setAge(age);
	return 0;
}

int l_getName(lua_State* L)
{
	Student **s = (Student**)luaL_checkudata(L, 1, "cc.Student");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	lua_settop(L, 0);
	lua_pushstring(L, (*s)->getName());
	return 1;
}

int l_getAge(lua_State* L)
{
	Student **s = (Student**)luaL_checkudata(L, 1, "cc.Student");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	lua_settop(L, 0);
	lua_pushnumber(L, (*s)->getAge());
	return 1;
}

int l_print(lua_State* L)
{
	Student **s = (Student**)luaL_checkudata(L, 1, "cc.Student");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	(*s)->print();

	return 0;
}

int student2string(lua_State *L) {
	Student **s = (Student**)luaL_checkudata(L, 1, "cc.Student");
	lua_pushfstring(L, "name = %s, age = %d", (*s)->getName(), (*s)->getAge());

	return 1;
}

static int auto_gc(lua_State *L)
{
	Student **s = (Student**)luaL_checkudata(L, 1, "cc.Student");
	luaL_argcheck(L, s != NULL, 1, "invalid user data");
	if (s)
	{
		delete *s;
	}

	return 0;
}



static const struct luaL_Reg stuentlib_f[] = {
	{"create", newStudent},
	{NULL, NULL}
};
static const struct luaL_Reg studentlib_m[] = {
	{"__tostring",student2string},
	{"setName",l_setName},
	{"setAge", l_setAge},
	{"print", l_print},
	{"getName",l_getName},
	{"getAge", l_getAge},
	{"__gc", auto_gc},
	{NULL, NULL}
};

int luaopen_student(lua_State *L) {
	luaL_newmetatable(L, "cc.Student");
	/* metatable.__index = metatable */
	lua_pushvalue(L, -1);  /* duplicates the metatable */
	lua_setfield(L, -2, "__index");

	luaL_setfuncs(L, studentlib_m, 0);

	luaL_newlib(L, stuentlib_f);
	return 1;
}

#endif