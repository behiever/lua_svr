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
   ����select ioģ�͵���������

*/
/************************************************************************/

class SelectIOServer
{
public:
	SelectIOServer(void);

	virtual ~SelectIOServer(void);

	//�Ƿ��ѳ�ʼ������
	virtual BOOL IsListening();

	//�����ʼ��
	virtual BOOL Init(  char* IP,  WORD Port ,  ServerProcessRecvData* pProcessRecvData ,   DWORD MaxConnectNum ) ;

	//ֹͣ�������
	virtual VOID Shutdown() ;

	//�������
	virtual VOID Update() ;

	//�����Ͽ�
	virtual BOOL DisConnect(   DWORD dwNetworkIndex ) ;

	//ָ������
	virtual BOOL Send(   DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize ) ;
 

	//�õ���ǰ��������
	virtual DWORD GetNumberOfConnections() ;

	//�õ�ip
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
