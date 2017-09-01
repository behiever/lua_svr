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
#include "Server.h"
#include ".\packethandler.h"




PacketHandler::PacketHandler(void)
{
}

PacketHandler::~PacketHandler(void)
{
}

//处理 客户消息 
void PacketHandler::ParsePacket(DWORD idx , BYTE * pMsg, WORD wSize )
{
	int  cmd;
	memcpy(&cmd,pMsg,sizeof(cmd));

	printf("PacketHandler::ParsePacket  msg %d \n",cmd);

	switch (cmd)
	{
		//登陆
	case C2S_LOGIN :		 return  On_C2S_LOGIN( idx,pMsg , wSize  );
 

	default:
		printf(  "ParsePacket 未知消息id %d\n",cmd);
		break;
	}
}

//客户请求登陆
void  PacketHandler::On_C2S_LOGIN(DWORD idx,  BYTE * pMsg, WORD wSize )
{
	printf(  "On_C2S_LOGIN   \n" );

	MSG_Login s;
	memcpy((char*)&s,  pMsg,  wSize);

	//账号abc　　密码123456
	if (strcmp(s.name,"abc")==0 && strcmp(s.pwd,"123456")==0 )
	{
		printf(  "登陆成功    %s\n", s.name );
		MSG_Login_Ret ret;
		ret.cmd=S2C_LOGIN_Ret; 
		ret.errcode=1;//登陆成功

		Server::GetInstance().Send(idx,(BYTE*)&ret,sizeof(MSG_Login_Ret));
		return ;
	}else{
		printf(  "登陆失败    %s\n", s.name );
		MSG_Login_Ret ret;
		ret.cmd=S2C_LOGIN_Ret; 
		ret.errcode=0; //登陆失败

		Server::GetInstance().Send(idx,(BYTE*)&ret,sizeof(MSG_Login_Ret));
		return ;
	}

}
