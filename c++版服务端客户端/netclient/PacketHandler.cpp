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
#include ".\packethandler.h"


PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
}

PacketHandler::~PacketHandler(void)
{
}

//处理 Server消息 
void PacketHandler::ParsePacket(  BYTE * pMsg, WORD wSize )
{
	int  cmd;
	memcpy(&cmd,pMsg,sizeof(cmd));

	printf("PacketHandler::ParsePacket  msg %d \n",cmd);

	switch (cmd)
	{
		//登陆返回
	case S2C_LOGIN_Ret :		 return  On_S2C_LOGIN_Ret( pMsg , wSize  );


	default:
		printf(  "ParsePacket 未知消息id %d\n",cmd);
		break;
	}
}

//登陆返回
void PacketHandler::On_S2C_LOGIN_Ret(  BYTE * pMsg, WORD wSize )
{
	MSG_Login_Ret s;
	memcpy((char*)&s,  pMsg,  wSize);

	if (s.errcode>0)
	{
		printf("登陆成功！\n");
	}else{
		printf("登陆失败!\n");
	}

}
