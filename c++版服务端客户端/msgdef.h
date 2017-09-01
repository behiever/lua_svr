#pragma once


#define C2S_LOGIN  0		//客户请求登陆
#define S2C_LOGIN_Ret  1	//客户登陆结果
 


//客户请求登陆 
struct MSG_Login
{
	int cmd; //命令  C2S_LOGIN 
	char name[20]; //用户名
	char pwd[20];  //密码 
};

//客户登陆 
struct MSG_Login_Ret
{
	int cmd; //命令  S2C_LOGIN_Ret
	int errcode; //1=登陆成功  , 0=登陆失败
	char pwd[20];  //密码
};