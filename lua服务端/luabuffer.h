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

#pragma once
 
#define LIBNAME "lua buffer"
 
#ifndef LUANET_API
#define LUANET_API
#endif
 
LUANET_API int luaopen_buffer (lua_State *L) ;
 

 /*


 struct MSG_Login
 {
 int cmd; //命令  C2S_LOGIN 
 char name[20]; //用户名
 char pwd[20];  //密码 
 };

 对应 解包
 b = buffer.new()
 b:init( data,len)
 cmd=b:read_int()
 name=b:read_string(20) 参数是字符数组的大小
 pwd=b:read_string(20)
 print("cmd:" ..  cmd   )
 print("name:" ..  name ,#name )
 print("pwd:" .. pwd ,#pwd )

 对应 打包
 ret = buffer.new()
 ret:write_int(1)
 ret:write_string("user1",20)  参数2是字符数组的大小
 ret:write_string("123456",20)

 网络发送数据指针和数据长度
 NetSend(   ret:size(),ret:getptr() )

  
 */
