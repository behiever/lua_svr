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

#include "stdafx.h"
#include ".\netclient.h"

 

NetClient::NetClient(void)
{
	m_bRemove=false;
	m_c = INVALID_SOCKET;
	m_pProcessRecvData=0;

	SockInit::Instance();
}

NetClient::~NetClient(void)
{
}


 
BOOL NetClient::Connect(  char* ServerIP, WORD Port , ClientProcessRecvData* pProcessRecvData )
{
	this->m_pProcessRecvData=pProcessRecvData;
	m_bRemove=false;

	// Create client socket
	m_c = socket(AF_INET, SOCK_STREAM, 0);  
 

	SOCKADDR_IN server;
	// Connect to server
	memset(&server, 0, sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr( ServerIP );
	server.sin_port = htons(Port);
	//connect无错返回0.否则SOCKET_ERROR
	int ret=connect(m_c, (struct sockaddr *)&server, sizeof(SOCKADDR_IN));
	if (ret==SOCKET_ERROR)
	{ 
		closesocket(m_c);
		return false;
	}
 
		return true;
	   
}

//断开连接
BOOL NetClient::DisConnect( )
{ 
	closesocket(m_c);

	return TRUE;
}


BOOL NetClient::Send( BYTE *pMsg , WORD wSize )
{
  
	//包头
	PACKET_HEADER pack_hdr;
	pack_hdr.size=wSize;

	//发送包头
	int rt=send(m_c, (char*)&pack_hdr , sizeof(PACKET_HEADER) ,0 );
	if(rt == SOCKET_ERROR || rt == 0)	return FALSE;

	//发送包体
	rt=send(m_c,(const char*) pMsg ,wSize,0);
	if(rt == SOCKET_ERROR || rt == 0)	return FALSE;

	return TRUE; 
 
}


//网络更新
VOID NetClient::Update()
{
 
	//标志移除了
	if(  IsRemove()  && m_pProcessRecvData!=NULL  )
	{ 
 
		m_pProcessRecvData(this,0,0);//回调断开消息

		m_pProcessRecvData=NULL; 
		return;
	}

	 m_selector.reset();
	 m_selector.add_sock(m_c);
	if(m_selector.select()>0 &&  m_selector.test_sock(m_c) )
	{ 
		 Recv( ); 
	}
 
}

void NetClient::Recv()
{
	//接收包头
	int rt= recv(m_c, (char*)&pack_hdr , sizeof(PACKET_HEADER) ,0);
	if(rt == SOCKET_ERROR || rt == 0)  
	{
		m_bRemove=true;
		return;
	}
  
	//接收包体
	rt = recv(m_c, buf , pack_hdr.size ,0);
	if(rt == SOCKET_ERROR || rt == 0)  
	{
		m_bRemove=true;
		return;
	}

	 
	m_pProcessRecvData(this,(BYTE*)buf,pack_hdr.size);
}
