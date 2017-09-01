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
// netclient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include ".\netclient.h"
#include "PacketHandler.h"

//�����ͻ�����������
  INetClient*   CreateNetClient(  char* ServerIP, WORD Port , ClientProcessRecvData* pProcessRecvData )
{
	INetClient* net=new  NetClient();
	if(net->Connect(ServerIP,Port,pProcessRecvData))
	{
		return net;//���ӷ������ɹ��򷵻�ָ��
	}

	return NULL;//���Ӳ��ɹ�!!.
}

 
 

INetClient* net;

VOID WINAPI RecvData(INetClient* p, BYTE *pMsg , WORD wSize )
{

	if(wSize==0){
		printf("�Ͽ�\n");
		return ;
	}


	g_PacketHandler.ParsePacket(pMsg,wSize);
 
}

//-------------------------------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{ 

	 
		net =CreateNetClient("127.0.0.1",123,RecvData ); //�����ͻ�����������
		if(net )
		{
			printf("CreateNetClient ok!   ����ok\n");
		}else{
			printf("CreateNetClient err! ����ʧ��\n");

		}
 

	MSG_Login login;
	login.cmd=C2S_LOGIN;
	strcpy(login.name,"abc");
	strcpy(login.pwd,"123456");
	net->Send((BYTE*)&login,sizeof(MSG_Login));
 

 

	while (1)
	{
		Sleep(1);

		
		net->Update();
		
 
	}

	
	return 0;
}

