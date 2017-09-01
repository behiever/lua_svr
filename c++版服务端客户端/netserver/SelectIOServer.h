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

 
#include "Session.h"
#include "Selector.h"

/************************************************************************/
/* 
   基于select io模型的网络引擎

*/
/************************************************************************/

class SelectIOServer
{
public:
	SelectIOServer(void);

	virtual ~SelectIOServer(void);

	//是否已初始化监听
	virtual BOOL IsListening();

	//网络初始化
	virtual BOOL Init(  char* IP,  WORD Port ,  ServerProcessRecvData* pProcessRecvData ,   DWORD MaxConnectNum ) ;

	//停止网络服务
	virtual VOID Shutdown() ;

	//网络更新
	virtual VOID Update() ;

	//单个断开
	virtual BOOL DisConnect(   DWORD dwNetworkIndex ) ;

	//指定发送
	virtual BOOL Send(   DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize ) ;
 

	//得到当前总连接数
	virtual DWORD GetNumberOfConnections() ;

	//得到ip
	virtual char* GetIP(   DWORD dwNetworkIndex ) ;
 
private:
	SOCKET m_s;

	DWORD m_dwMaxSessionNum;

	ServerProcessRecvData* m_pProcessRecvData;

	SESSION_LIST m_clientlist;

	Selector m_selector;

	Session*  _List_Find(  DWORD dwNetworkIndex  );

	void HandleAccept( );

	void  HandleSession(    );
};
