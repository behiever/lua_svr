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
 

/************************************************************************/
/* 

  seleect io模型的select操作封装 

*/
/************************************************************************/

class Selector
{
public:
	Selector(void){reset();};
	~Selector(void){}

	inline void reset()
	{
		FD_ZERO(&fdSocket);
	}

	inline void add_sock(SOCKET s)
	{
		FD_SET(s, &fdSocket);
	};

	inline int test_sock(SOCKET s)
	{
		return FD_ISSET( s , &fdSocket);
	}

	int select(int timeout=10)
	{
		struct timeval tv = {0, timeout}; //超时定义,10毫秒
 
		int nRet = ::select(0, &fdSocket, NULL, NULL, &tv);
		//if (nRet==0) //超时
		//if( nRet==SOCKET_ERROR )  // 出错 
		return nRet;
	}

private:

	fd_set fdSocket;	 

};
