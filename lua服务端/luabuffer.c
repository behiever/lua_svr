/*
===============================================================================
The MIT License
Copyright (C) 1994-2015 simawei<simawei@qq.com>
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

 

#include "lua.h"
#include "lauxlib.h"
#if ! defined (LUA_VERSION_NUM) || LUA_VERSION_NUM < 501
#include "compat-5.1.h"
#endif

#include "luabuffer.h"

typedef struct BUFF {
  
	int writepos;
	int readpos;
	char values[1024*64];  /* 默认 64k 够用了 */

} BUFF;

static BUFF *checkarray (lua_State *L) {

	void *ud = luaL_checkudata(L, 1, LIBNAME );

	luaL_argcheck(L, ud != NULL, 1, "`array' expected");

	return (BUFF *)ud;

}


static int newbuffer (lua_State *L)
{

	//int n = luaL_checkint(L, 1);

 

	BUFF *a = (BUFF *)lua_newuserdata(L, sizeof(BUFF));
 
	luaL_getmetatable(L, LIBNAME );

	lua_setmetatable(L, -2);
 

	a->readpos=0;
	a->writepos=0;
  

	return 1;  /* new userdatum is already on the stack */

}
static int getsize (lua_State *L) {

	BUFF *a = checkarray(L);
	lua_pushnumber(L, a->writepos);
  
	return 1;

}
//static double *getelem (lua_State *L) {
//
//	BUFF *a = checkarray(L);
//
//	int index = luaL_checkint(L, 2);
//
//
//
//	luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");
//
//
//
//	/* return element address */
//
//	return &a->values[index - 1];
//
//}
static int init (lua_State *L) {

	//double newvalue = luaL_checknumber(L, 3);

//	*getelem(L) = newvalue;
	char*  data;int len;

	BUFF *a = checkarray(L);

 

	  len = luaL_checkint(L, 3);//len
	// 获取第1个参数
//	char*  data = (char* )lua_touserdata(L, 2);
//	char*  data = (char* )lua_topointer(L,2);
	  data = (char* )lua_tostring(L,2);
//	char* data = lual_topo;//luaL_checklstring(L, 2,len);//data

	memcpy(a->values,data,len);

	a->writepos=len;


	//lua_pushnumber(L, a->writepos);
 
	return 0;

}



static int getarray (lua_State *L) {

///	lua_pushnumber(L, *getelem(L));

	return 1;

}

//返回buffer指针给lua  采用lstring
static int getptr (lua_State *L) {

	///	lua_pushnumber(L, *getelem(L));
	BUFF *a = checkarray(L);
	//lua_pushlightuserdata(L, &a->values);
	char* s=&a->values[0];
	lua_pushlstring(L,s,a->writepos);

	return 1;

}

static int read_int (lua_State *L) {

	int r;
	///	lua_pushnumber(L, *getelem(L));
	BUFF *a = checkarray(L);
 
	//读指针不能超过写指针
	luaL_argcheck(L,   a->readpos < a->writepos , 2, "read out of length");

	//int index = luaL_checkint(L, 2);

	 
	memcpy((char*)&r,a->values+a->readpos,sizeof(int));
	a->readpos+=sizeof(int);

	lua_pushinteger(L,r );

	if ( a->readpos > a->writepos)
	{
		//printf("read out of end!\n");
		return luaL_error (L, LIBNAME" read out of end ");
	}

	return 1;

}

static int write_int (lua_State *L) {

 
	///	lua_pushnumber(L, *getelem(L));
	BUFF *a = checkarray(L);

//	luaL_argcheck(L,   a->readpos < a->size , 2, "read out of range");

	int v = luaL_checkint(L, 2);


	memcpy(a->values+a->writepos,(char*)&v,sizeof(int));
	a->writepos+=sizeof(int);

	lua_pushinteger(L,sizeof(int)  );

	return 1;

}

static int read_bytes (lua_State *L) {

	int len;char* s;

	///	lua_pushnumber(L, *getelem(L));
	BUFF *a = checkarray(L);

	//读指针不能超过写指针
	luaL_argcheck(L,   a->readpos < a->writepos , 2, "read out of length");

	
	len = luaL_checkint(L, 2);

	s=a->values+a->readpos;
	 
	a->readpos+=len ;

 
	lua_pushlstring(L,s,len );

	if ( a->readpos == a->writepos)
	{
		//printf("read reach end!\n");
	}
	if ( a->readpos > a->writepos)
	{
		//printf("read out of end!\n");
			return luaL_error (L, LIBNAME" read out of end ");
	}

	return 1;

}

static int write_bytes (lua_State *L) {


	///	lua_pushnumber(L, *getelem(L));
	BUFF *a = checkarray(L);

	//	luaL_argcheck(L,   a->readpos < a->size , 2, "read out of range");

	size_t len = luaL_checkint(L, 3);
	// char* s = luaL_checklstring(L, 2,len );
	 char* data  = (char* )lua_tostring(L,2);

	memcpy(a->values+a->writepos, data , len );
	a->writepos+= len;

	lua_pushinteger(L, len );

	return 1;

}


static int read_string (lua_State *L) {

	int len;char* s;

	///	lua_pushnumber(L, *getelem(L));
	BUFF *a = checkarray(L);

	//读指针不能超过写指针
	luaL_argcheck(L,   a->readpos < a->writepos , 2, "read out of length");


	len = luaL_checkint(L, 2);

	s=a->values+a->readpos;

	a->readpos+=len ;

	//遇空截断的string,不能用lstring,因为lstring用#获得长度是它的数组大小而不是数据大小
	lua_pushstring(L,s );

	if ( a->readpos == a->writepos)
	{
		//printf("read reach end!\n");
	}

	if ( a->readpos > a->writepos)
	{
		//printf("read out of end!\n");
			return luaL_error (L, LIBNAME" read out of end ");
	}

	return 1;

}

 

int array2string (lua_State *L) {

	BUFF *a = checkarray(L);

	lua_pushfstring(L, "buffer(%d)", a->writepos);

	return 1;

}



static const struct luaL_reg arraylib_f [] = {

	{"new", newbuffer},

	{NULL, NULL}

};



static const struct luaL_reg arraylib_m [] = {

	{"__tostring", array2string},

	{"init", init},

	//{"get", getarray},//

	{"size", getsize},//字节长度

	{"getptr", getptr}, //返回lstring


	{"read_int", read_int},
 	{"write_int", write_int},

	{"read_bytes", read_bytes},
	{"write_bytes", write_bytes},

	{"read_string", read_string},
	{"write_string", write_bytes}, //通用。
 
	{NULL, NULL}

};
 
 

LUANET_API int luaopen_buffer (lua_State *L)
{

	luaL_newmetatable(L,  LIBNAME );



	lua_pushstring(L, "__index");

	lua_pushvalue(L, -2);    /* pushes the metatable */

	lua_settable(L, -3); /* metatable.__index = metatable */



	luaL_openlib(L, NULL, arraylib_m, 0);



	luaL_openlib(L, "buffer", arraylib_f, 0);

	return 1;

}
