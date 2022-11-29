#pragma once
//---Ŭ���̾�Ʈ ��Ʈ��ũ �ۼ��� ��ü
#include "CPlayer.h"

//-----------------��Ʈ��ũ ��ó����
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

//#include <filesystem>
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...
#include <iostream>

#include <fstream>
using namespace std;

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ
constexpr auto BUFF_MAXSIZE = 116;	//sizeof(CPlayer);;

#define TYPE_TAG_PLAYER	0
#define TYPE_TAG_TOWER	1
#define TYPE_TAG_TURRET	2
#define TYPE_TAG_DIA	3
#define TYPE_TAG_ELLIP	4
#define TYPE_TAG_RECT	5
#define TYPE_TAG_TRI	6


#define SERVERPORT 9000

//--------------------------------------------------


class CNetwork	
{
private:
	int My_objTag; // ���� ��ü�� �±װ��� �����Ͽ� ��ü Ÿ���� �����ϴ� ����
	int len;
	char buf[BUFF_MAXSIZE+1]; // ������ ���� ����
	int retval; // ���� Ȯ�ο� ����
public:
	WSADATA wsa; // ���� ����
	SOCKET sock; // ���� ����
	struct sockaddr_in serveraddr; // ���� �ּ� ����ü
	char* SERVERIP = (char*)"127.0.0.1";
public:
	CNetwork();
	~CNetwork();
	void err_quit(const char* msg);
	void err_display(const char* msg);
	void err_display(int errcode);
	DWORD WINAPI ClientToServerComm(LPVOID arg);
	void ClientToServerComm();
	/*void ClientDataSet( CGameObject* inmylist, int inmynum, CGameObject* inopplist, int inoppnum);*/
	void ClientDataSet();
	void ServerDataSet(const char* player);
	void Connect();
	void Send();
	void Recv();
	void Close();
};

