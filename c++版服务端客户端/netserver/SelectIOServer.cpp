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
//�����ʼ��
BOOL SelectIOServer::Init(  char* IP,   WORD Port ,   ServerProcessRecvData* pProcessRecvData ,   DWORD MaxConnectNum )
{
	m_dwMaxSessionNum=MaxConnectNum;
	m_pProcessRecvData=pProcessRecvData;

	//����socket
	m_s = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in my_addr;		/*	���ص�ַ��Ϣ	*/ 
	//bind�����Ķ˿�
	my_addr.sin_family = AF_INET;			/* Э��������INET	*/ 
	my_addr.sin_port = htons(Port);		/* �� �˿�	*/ 
	my_addr.sin_addr.s_addr = INADDR_ANY;	/* ����IP			*/ 

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

//ֹͣ�������
VOID SelectIOServer::Shutdown()
{
 
		//�رձ��ؼ���
		closesocket(m_s);
		m_s=INVALID_SOCKET;
 
 
}
//�Ƿ��ѳ�ʼ������
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
  
		//�Ƿ����¿ͻ�����
		HandleAccept( );

		//�Ƿ�ͻ������ݷ���
		HandleSession( );
	}

	//����Ͽ������� 
	for(it=m_clientlist.begin();it!=m_clientlist.end(); ++it )
	{
		Session* pSession=(*it); 
		if( pSession->IsRemoved()  )
		{

			//�����Ƴ�
			m_clientlist.erase( it-- ); 

			//�ӿڻص�֪ͨ �ͻ��Ͽ���Ϣ
			m_pProcessRecvData( pSession->GetSocket(),0,0);

			//���� 
			delete pSession;

		} 
	}
}

void SelectIOServer::HandleAccept( )
{
	// ��1�������׽��ֽ��յ�������
	if( m_selector.test_sock( m_s ) )
	{
		if( GetNumberOfConnections() < m_dwMaxSessionNum ) 
		{
 
			SOCKET c = ::accept(m_s, 0,0);
			if (c==INVALID_SOCKET)
			{
				return  ;
			}
			//�����»Ự
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
	// 3��ͨ����ԭ��fdSocket������select�������fdRead���ϱȽϣ�
	// ȷ��������Щ�׽�����δ��I/O������һ��������ЩI/O��
	 
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
 
	//sock�ҿͻ�
	Session* pSession=_List_Find(dwNetworkIndex);
	if (!pSession)
	{
		return FALSE;//û�ҵ� 
	}

	//����
	return pSession->Send(pMsg,wSize );
}

//�õ�ip
char* SelectIOServer::GetIP( const DWORD dwNetworkIndex  )
{
	 sockaddr_in sa;   
	int  len   =   sizeof(sa); 
	getpeername(dwNetworkIndex,   (  sockaddr   *)&sa,   &len);    
	return inet_ntoa(sa.sin_addr);

	//printf("�Է�IP %s:%d", inet_ntoa(sa.sin_addr),sa.sin_port);  
}
//�õ��������ӿͻ�����
DWORD  SelectIOServer::GetNumberOfConnections()
{ 

	return (DWORD)m_clientlist.size();;
}

//�Ͽ�һ���ͻ�
BOOL SelectIOServer::DisConnect( const DWORD dwNetworkIndex   )
{
	//shutdown(dwNetworkIndex,SD_BOTH);
	closesocket(dwNetworkIndex);
	return TRUE;
}

//����
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


 
