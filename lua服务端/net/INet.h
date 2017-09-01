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

 
 

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include <conio.h>

#include <list>

#define MAX_BUF_SIZE 1024*15

#define FD_SETSIZE 10240

#include <Windows.h> 
#pragma comment(lib, "ws2_32.lib")


struct PACKET_HEADER
{
	WORD size; //�����յ����ݰ��ܴ�С 
};


//#include "../msgdef.h"  //������Ϣ 

/************************************************************************/
/* 
  �������������ӿ�
 
*/
/************************************************************************/


//�ص��������ݺ���ԭ��
typedef VOID WINAPI ServerProcessRecvData( DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize  );

class INetServer 
{
public: 
	virtual ~INetServer(void) {};

	//�Ƿ��ѳ�ʼ������
	virtual BOOL IsListening()=0;

	//�����ʼ��
	virtual BOOL Init(  char* IP,  WORD Port ,  ServerProcessRecvData* pProcessRecvData ,   DWORD MaxConnectNum )=0;
 
	//ֹͣ�������
	virtual VOID Shutdown()=0;

	//�������
	virtual VOID Update()=0;

	//�����Ͽ�
	virtual BOOL DisConnect(   DWORD dwNetworkIndex )=0;

	//��������
	virtual BOOL Send(   DWORD dwNetworkIndex ,   BYTE *pMsg ,  WORD wSize )=0;

  

	//�õ���ǰ��������
	virtual DWORD GetNumberOfConnections()=0;

	//�õ�ip
	virtual char* GetIP(   DWORD dwNetworkIndex )=0;
 
};

 

    
/************************************************************************/
/* 
    �ͻ�����������ӿ�

*/
/************************************************************************/
  
class INetClient;
//�ص��������ݺ���ԭ��
typedef VOID WINAPI ClientProcessRecvData(INetClient* p,   BYTE *pMsg ,   WORD wSize   );

class INetClient
{
 
public:
	//���ӷ�����
	virtual BOOL Connect(  char* ServerIP,  WORD Port ,  ClientProcessRecvData* pProcessRecvData )=0;

	//�Ͽ�����
	virtual BOOL DisConnect( )=0;

	//ָ������
	virtual BOOL Send(    BYTE *pMsg ,   WORD wSize )=0;
	
	//�������
	virtual VOID Update()=0;
 
};
