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
#include ".\server.h"

 

//网络消息处理
void WINAPI RecvDataCall(DWORD dwNetworkIndex , BYTE *pMsg, WORD wSize)
{ 
	if( wSize == 0 || pMsg == NULL )	//客户退出了
	{
		printf("[%d]退出\n",dwNetworkIndex );
		return;
	}else{

		//处理消息
		Server::GetInstance().ParsePacket(dwNetworkIndex,pMsg,wSize); 
 
	}
}

Server::Server(void)
{
}

Server::~Server(void)
{
	if(s.IsListening()) s.Shutdown();
} 

bool Server::Init()
{
	int port=123;

	if(s.Init("",port,RecvDataCall,999))
	{
		printf("CreateNetServer   ok! listen [%d] ok\n",port);
		return 1;
	}else{
		printf("CreateNetServer   err! listen err\n");
		return 0;
	}

}

void Server::Run()
{

	printf("按任意键退出\n");


	DWORD last=::GetTickCount();
	while(s.IsListening() )
	{
		if (kbhit())
		{
			getch(); 
			break;
		}

		s.Update();

		if(GetTickCount()-last>1000)
		{
			last=GetTickCount();

			printf("有[%d]个客户连接  \n",s.GetNumberOfConnections()  );
 
		}

		Sleep(1);

	} 

}
