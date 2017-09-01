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

 
 

// TODO: 在此处引用程序要求的附加头文件
#include <conio.h>

#include <list>

#define MAX_BUF_SIZE 1024*15

#define FD_SETSIZE 10240

#include <Windows.h> 
#pragma comment(lib, "ws2_32.lib")


struct PACKET_HEADER
{
	WORD size; //待接收的数据包总大小 
};


//#include "../msgdef.h"  //公共消息 

/************************************************************************/
/* 
  服务端网络引擎接口
 
*/
/************************************************************************/


//回调处理数据函数原型
typedef VOID WINAPI ServerProcessRecvData( DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize  );

class INetServer 
{
public: 
	virtual ~INetServer(void) {};

	//是否已初始化监听
	virtual BOOL IsListening()=0;

	//网络初始化
	virtual BOOL Init(  char* IP,  WORD Port ,  ServerProcessRecvData* pProcessRecvData ,   DWORD MaxConnectNum )=0;
 
	//停止网络服务
	virtual VOID Shutdown()=0;

	//网络更新
	virtual VOID Update()=0;

	//单个断开
	virtual BOOL DisConnect(   DWORD dwNetworkIndex )=0;

	//单个发送
	virtual BOOL Send(   DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize )=0;

  

	//得到当前总连接数
	virtual DWORD GetNumberOfConnections()=0;

	//得到ip
	virtual char* GetIP(   DWORD dwNetworkIndex )=0;
 
};

 

    
/************************************************************************/
/* 
    客户端网络引擎接口

*/
/************************************************************************/
  
class INetClient;
//回调处理数据函数原型
typedef VOID WINAPI ClientProcessRecvData(INetClient* p,   BYTE *pMsg ,   WORD wSize   );

class INetClient
{
 
public:
	//连接服务器
	virtual BOOL Connect(  char* ServerIP,  WORD Port ,  ClientProcessRecvData* pProcessRecvData )=0;

	//断开连接
	virtual BOOL DisConnect( )=0;

	//指定发送
	virtual BOOL Send(    BYTE *pMsg ,   WORD wSize )=0;
	
	//网络更新
	virtual VOID Update()=0;
 
};
