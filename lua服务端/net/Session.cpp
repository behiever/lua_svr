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
#include ".\session.h"

Session::Session(SOCKET c)
{
	this->m_c=c;
	m_remove=false;
}

Session::~Session(void)
{
}

BOOL Session::Send( const BYTE *pMsg ,const WORD wSize  )
{ 
	 
	 
	//包头
	PACKET_HEADER pack_hdr;
	pack_hdr.size=wSize;

	//发送包头
	int rt=send(m_c, (char*)&pack_hdr , sizeof(PACKET_HEADER) ,0 );
	if(rt == SOCKET_ERROR || rt == 0)
	{
		m_remove=true;
		return FALSE;
	}
	//发送包体
	rt=send(m_c,(const char*) pMsg ,wSize,0);
	if(rt == SOCKET_ERROR || rt == 0)
	{
		m_remove=true;
		return FALSE;
	}
	return TRUE; 
 
}
	
BOOL Session::RecvData(ServerProcessRecvData* m_pProcessRecvData)
{
	//包头 
	pack_hdr.size=0;

	//接收包头
	int rt=recv(m_c, (char*)&pack_hdr , sizeof(PACKET_HEADER) ,0 );
	if(rt == SOCKET_ERROR || rt == 0)
	{
		m_remove=true;
		return FALSE;
	}

	//校验包头数据有效性
	if (pack_hdr.size==0)
	{
		m_remove=true;
		return FALSE;
	}

	//接收包体
	rt=recv(m_c, buf ,  pack_hdr.size ,0 );
	if(rt == SOCKET_ERROR || rt == 0)
	{
		m_remove=true;
		return FALSE;
	}

	m_pProcessRecvData(m_c,(BYTE*)buf, pack_hdr.size);
	return TRUE;
}
