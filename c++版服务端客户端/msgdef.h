#pragma once


#define C2S_LOGIN  0		//�ͻ������½
#define S2C_LOGIN_Ret  1	//�ͻ���½���
 


//�ͻ������½ 
struct MSG_Login
{
	int cmd; //����  C2S_LOGIN 
	char name[20]; //�û���
	char pwd[20];  //���� 
};

//�ͻ���½ 
struct MSG_Login_Ret
{
	int cmd; //����  S2C_LOGIN_Ret
	int errcode; //1=��½�ɹ�  , 0=��½ʧ��
	char pwd[20];  //����
};