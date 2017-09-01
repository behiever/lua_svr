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

#include "INet.h"
#include "SockInit.h"
#include "Selector.h"
/************************************************************************/
/* 

   网络引擎客户端 2014.5.13


*/
/************************************************************************/


class  NetClient : public INetClient
{
 

public:
	NetClient();
	virtual ~NetClient();

	//连接服务器  
	virtual BOOL Connect(  char* ServerIP, WORD Port , ClientProcessRecvData* pProcessRecvData );
	//断开连接
	virtual BOOL DisConnect( );

	//发送
	virtual BOOL Send( BYTE *pMsg , WORD wSize );
  
	//网络更新
	virtual VOID Update(); 

 
private:
	bool m_bRemove;
	bool IsRemove(){return m_bRemove;};

	  
	SOCKET m_c; 

	//回调函数指针
	ClientProcessRecvData* m_pProcessRecvData;

	Selector m_selector;

	void Recv();

	char			buf[MAX_BUF_SIZE];//数据缓冲区

	PACKET_HEADER   pack_hdr; //包头
};


 
