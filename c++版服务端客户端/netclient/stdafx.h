// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once

#include <iostream>
#include <tchar.h>

// TODO: 在此处引用程序要求的附加头文件
#include <conio.h>

#include <list>

#define MAX_BUF_SIZE 1024*15

#define FD_SETSIZE 10240

#include <Windows.h> 
#pragma comment(lib, "ws2_32.lib")

struct PACKET_HEADER
{
	WORD size; //待接收的数据包总大小 
};

#include "../msgdef.h"  //公共消息 