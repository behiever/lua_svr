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


#include "SelectIOServer.h"
#include "PacketHandler.h"


/************************************************************************/
/* 
   主Server服务端类
*/
/************************************************************************/

class Server
{
	PacketHandler m_PacketHandler;//消息处理 

	SelectIOServer s;//网络引擎

	Server(void);
	 
public:
	~Server(void);
	 
	static Server& GetInstance(){
		static Server s;
		return s;
	};

	bool Init();

	void Run();

	void ParsePacket(DWORD idx , BYTE * pMsg, WORD wSize ){return  m_PacketHandler.ParsePacket(idx,pMsg,wSize);}

	BOOL Send(   DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize ){return s.Send(dwNetworkIndex,pMsg,wSize);} 
};
