// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#include <iostream>
#include <tchar.h>

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

#include "../msgdef.h"  //������Ϣ 