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
#include ".\server.h"
#include <string>
#include <sstream>

void WRITE_LOG(std::string  s)
{
	printf("%s\n", s.c_str() ); 
}


//网络消息处理
void WINAPI RecvDataCall(DWORD dwNetworkIndex , BYTE *pMsg, WORD wSize)
{ 
	if( wSize == 0 || pMsg == NULL ) 
	{
		//处理 断开
		Server::GetInstance().OnClose2Lua( dwNetworkIndex );
		return;
	}else{

		//处理 消息
		Server::GetInstance().OnRecvData2Lua(  dwNetworkIndex ,(char*)pMsg,wSize );

	}
}
  
 
void  Server::OnRecvData2Lua( int idx , char * pMsg, WORD wSize )
{

	//
	lua_getglobal(  L , "OnRecvData" );

	lua_pushinteger( L, idx );
	lua_pushlstring( L,  pMsg ,wSize);
	lua_pushinteger ( L, wSize );
 
	//参数2 表示传入参数 参数3 返回参数
	if( lua_pcall( L ,3,0,0) ) 
	{

		std::string errStr = "OnRecvData2Lua err:";

		errStr+=lua_tostring(  L  , -1 );

		WRITE_LOG( errStr.c_str() );

		lua_pop( L  , 1 );

	}

}

void Server::OnClose2Lua( int idx   )
{
	//
	lua_getglobal(  L , "OnClose" );
	lua_pushnumber( L, idx ); 
	//参数2 表示传入参数 参数3 返回参数
	if( lua_pcall(L   ,1,0,0) ) 
	{
		std::string errStr = "OnClose2Lua err:";
		errStr+=lua_tostring(  L  , -1 );
		WRITE_LOG( errStr.c_str() );
		lua_pop( L  , 1 );
	}
}


int G_NetInit( lua_State * L )
{
	INetServer* net;//网络引擎
 
	try
	{

		int paras = lua_gettop( L );
		if( paras < 2 )
		{
			std::ostringstream os;
			os << "G_NetInit: parameter missing. " << 2 << " " << paras;
			WRITE_LOG(  os.str() );
		}
		else
		{		
			std::string ip=lua_tostring( L , 1 ) ;
			int   port =  lua_tointeger( L , 2 );

			net= CreateNetServer( 
				(char*)ip.c_str()  ,   //"0.0.0.0" ,
				port,
				RecvDataCall,2000); //创建网络引擎
 
		}

	}
	catch( ... )
	{
		WRITE_LOG(  "G_NetInit: unknown exception" );
	}		

	lua_pushlightuserdata(L,net); 

	return 1;
}

int G_NetSend( lua_State * L )
{
	bool result=false;
	try
	{
		int paras = lua_gettop( L );
		if( paras < 4 )
		{
			std::ostringstream os;
			os << "G_NetSend: parameter missing. " << 3 << " " << paras;
			WRITE_LOG(  os.str() );
		}
		else
		{		
			INetServer* net= (INetServer*)lua_topointer( L , 1 );
			int   idx =  lua_tointeger( L , 2 );
			if( idx )
			{
 
				int   len =  lua_tointeger( L , 3 );
				char* data=(char*)lua_tolstring( L ,4 ,(size_t*)&len ) ;
				 
				result=net->Send(idx, (BYTE*)data, len );
			}
		}
	}
	catch( ... )
	{
		WRITE_LOG(  "G_NetSend: unknown exception" );
	}		

	lua_pushboolean( L, result );

	return 1;
}

int G_NetConnNums( lua_State * L )
{
	int result=false;
	try
	{
		int paras = lua_gettop( L );
		if( paras < 1 )
		{
			std::ostringstream os;
			os << "G_NetConnNums: parameter missing. " << 1 << " " << paras;
			WRITE_LOG(  os.str() );
		}
		else
		{		
			INetServer* net= (INetServer*)lua_topointer( L , 1 );
			
			if( net )
			{
		 
				 
				result=net->GetNumberOfConnections();
			}
		}
	}
	catch( ... )
	{
		WRITE_LOG(  "G_NetConnNums: unknown exception" );
	}		

	lua_pushinteger( L, result );

	return 1;
}


 

int G_NetUpdate( lua_State * L )
{
	try
	{
		INetServer* net= (INetServer*)lua_topointer( L , 1 );
		if (net)
		{

			net->Update();
		} 
	}
	catch( ... )
	{
		WRITE_LOG(  "G_NetUpdate: unknown exception" );
	}		

	return 0;
}

int G_NetShutdown( lua_State * L )
{
	try
	{
		INetServer* net= (INetServer*)lua_topointer( L , 1 );
		if (net)
		{
			DestroyNetServer(net);
			net=NULL;
		} 
	}
	catch( ... )
	{
		WRITE_LOG(  "G_NetShutdown: unknown exception" );
	}		

	return 0;
}

Server::Server(void)
{
	g_bTerminate=false;//退出标志
}

Server::~Server(void)
{
 
} 
 

bool Server::Init()
{ 
	init_Lua();
 
	return true;
}
 
void Server::Run()
{

	printf("按任意键退出\n");


 
	while(// s.IsListening() &&
		!g_bTerminate )
	{
		if (kbhit())
		{
			getch(); 
			break;
		}

		 

		//不断更新lua
		Update_Lua();

		Sleep(1000);

	} 


	 
	//关闭lua
	Shutdown_Lua();

}
 


 

//调用lua函数
bool Server::call_lua(const char* fun)
{
	lua_getglobal( L, fun );
	if( lua_pcall(L,0,0,0) )
	{
		std::string errStr =fun; 
		errStr+="  err:";
		errStr+=lua_tostring( L, -1 );
		WRITE_LOG( errStr );
		lua_pop( L, 1 );
		return false;
	}
	return true;
}

int G_Exit(lua_State * L)
{
	Server::GetInstance().Terminate();
 
	return 0;
 
}
 
void Server::init_Lua()
{
	//初始化lua环境
	L = lua_open();  
	luaL_openlibs( L );
	luaopen_buffer(L);
 
	//注册一个函数给lua,提供lua退出函数
	lua_pushcfunction( L, G_Exit );
	lua_setglobal( L, "G_Exit" );

	//网络api
	lua_pushcfunction( L, G_NetInit );
	lua_setglobal( L, "G_NetInit" );

	lua_pushcfunction( L, G_NetSend );
	lua_setglobal( L, "G_NetSend" );

	lua_pushcfunction( L, G_NetShutdown );
	lua_setglobal( L, "G_NetShutdown" );

	lua_pushcfunction( L, G_NetUpdate );
	lua_setglobal( L, "G_NetUpdate" );

	lua_pushcfunction( L, G_NetConnNums );
	lua_setglobal( L, "G_NetConnNums" );
	 

	std::string ResultStr;

	//加载并启动 
	if(luaL_dofile(L,"script/sim.lua"))
	//if( luaL_loadfile(L,"sim.lua") || lua_pcall(L,0,0,0) )
	{
		std::string errStr = "加载失败:";
		errStr+=lua_tostring( L, -1 );
		WRITE_LOG( errStr );
		lua_pop( L, 1 );
	}
 
	//调用lua 的StartUp 
	if(! call_lua( "StartUp" ) )
	{
		//("启动失败");
	}
}

void Server::Update_Lua(   ) 
{ 
 
    //调用lua 的Update
	if(! call_lua( "Update" ) )
	{
		// "Update err:";
		 
	}
  
}

void Server::Shutdown_Lua(   ) 
{ 
	//调用lua 的Shutdown
	if(! call_lua( "Shutdown" ) )
	{
		// "Shutdown err:";

	}

	//关闭lua环境
	lua_close(L);    
}

 
