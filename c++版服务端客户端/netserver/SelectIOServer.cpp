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
#include "StdAfx.h"
#include ".\selectioserver.h"
#include "SockInit.h"

SelectIOServer::SelectIOServer(void)
{
	m_s=INVALID_SOCKET;

	SockInit::Instance();
}

SelectIOServer::~SelectIOServer(void)
{
	if(m_s!=INVALID_SOCKET) Shutdown();
 
}
//网络初始化
BOOL SelectIOServer::Init(  char* IP,   WORD Port ,   ServerProcessRecvData* pProcessRecvData ,   DWORD MaxConnectNum )
{
	m_dwMaxSessionNum=MaxConnectNum;
	m_pProcessRecvData=pProcessRecvData;

	//建立socket
	m_s = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in my_addr;		/*	本地地址信息	*/ 
	//bind本机的端口
	my_addr.sin_family = AF_INET;			/* 协议类型是INET	*/ 
	my_addr.sin_port = htons(Port);		/* 绑定 端口	*/ 
	my_addr.sin_addr.s_addr = INADDR_ANY;	/* 本机IP			*/ 

	int ret=bind(m_s, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));	
	if( ret == SOCKET_ERROR )
	{
		closesocket( m_s );
		m_s=INVALID_SOCKET;
		return FALSE;
	}
 
	listen(m_s, 5);

	return TRUE;
}

//停止网络服务
VOID SelectIOServer::Shutdown()
{
 
		//关闭本地监听
		closesocket(m_s);
		m_s=INVALID_SOCKET;
 
 
}
//是否已初始化监听
BOOL SelectIOServer::IsListening()
{
	return m_s!=INVALID_SOCKET ;
}

VOID SelectIOServer::Update() 
{
	m_selector.reset();
	m_selector.add_sock(m_s);

	 
	SESSION_LIST_ITER  it;
	for(it=m_clientlist.begin();it!=m_clientlist.end(); ++it )
	{
		Session* pSession=(*it);
		m_selector.add_sock(pSession->GetSocket()); 
	}

	if( m_selector.select()>0 )	 
	{ 
  
		//是否有新客户连接
		HandleAccept( );

		//是否客户有数据发来
		HandleSession( );
	}

	//处理断开的连接 
	for(it=m_clientlist.begin();it!=m_clientlist.end(); ++it )
	{
		Session* pSession=(*it); 
		if( pSession->IsRemoved()  )
		{

			//链表移除
			m_clientlist.erase( it-- ); 

			//接口回调通知 客户断开消息
			m_pProcessRecvData( pSession->GetSocket(),0,0);

			//回收 
			delete pSession;

		} 
	}
}

void SelectIOServer::HandleAccept( )
{
	// （1）监听套节字接收到新连接
	if( m_selector.test_sock( m_s ) )
	{
		if( GetNumberOfConnections() < m_dwMaxSessionNum ) 
		{
 
			SOCKET c = ::accept(m_s, 0,0);
			if (c==INVALID_SOCKET)
			{
				return  ;
			}
			//接受新会话
			Session* pSession=new Session(c); 
			m_clientlist.push_back(pSession);
		}
		else
		{
			//printf(" Too much connections! \n");
		}
	}
}

void SelectIOServer::HandleSession(    )
{
	// 3）通过将原来fdSocket集合与select处理过的fdRead集合比较，
	// 确定都有哪些套节字有未决I/O，并进一步处理这些I/O。
	 
	SESSION_LIST_ITER  it;
	for(it=m_clientlist.begin();it!=m_clientlist.end(); ++it )
	{ 
		Session* pSession=(*it);
		if(  m_selector.test_sock( pSession->GetSocket() ))
		{
			 pSession->RecvData(m_pProcessRecvData);
			 
		}
	}
}

BOOL SelectIOServer::Send(  DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize  )
{
 
	//sock找客户
	Session* pSession=_List_Find(dwNetworkIndex);
	if (!pSession)
	{
		return FALSE;//没找到 
	}

	//发送
	return pSession->Send(pMsg,wSize );
}

//得到ip
char* SelectIOServer::GetIP( const DWORD dwNetworkIndex  )
{
	 sockaddr_in sa;   
	int  len   =   sizeof(sa); 
	getpeername(dwNetworkIndex,   (  sockaddr   *)&sa,   &len);    
	return inet_ntoa(sa.sin_addr);

	//printf("对方IP %s:%d", inet_ntoa(sa.sin_addr),sa.sin_port);  
}
//得到所有连接客户数量
DWORD  SelectIOServer::GetNumberOfConnections()
{ 

	return (DWORD)m_clientlist.size();;
}

//断开一个客户
BOOL SelectIOServer::DisConnect( const DWORD dwNetworkIndex   )
{
	//shutdown(dwNetworkIndex,SD_BOTH);
	closesocket(dwNetworkIndex);
	return TRUE;
}

//查找
Session* SelectIOServer::_List_Find(  DWORD dwNetworkIndex  )
{
 
	SESSION_LIST_ITER  it;
	for(it=m_clientlist.begin();it!=m_clientlist.end(); ++it )
	{
		Session* pSession=(*it);
		if (pSession->GetSocket() == dwNetworkIndex )
		{
			return pSession;
		}
	}

	return NULL;
}


 
