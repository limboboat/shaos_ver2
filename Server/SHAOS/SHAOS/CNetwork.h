#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#define SERVERPORT 9000
#define BUFSIZE    512

#define cPlayerSize 3

extern CNetwork cnetwork;

class CNetwork {
	int My_objTag;
	char buf[cPlayerSize];

public:
	int retval;
	WSADATA wsa;
	SOCKET listen_sock;
	struct sockaddr_in serveraddr;

	HANDLE ClientEvent[3];
	HANDLE ServerEvent[3];

	static HWND hEdit;			  // ����Ʈ ��Ʈ��
	static CRITICAL_SECTION cs; // DisplayText()�� �Ӱ� ����

public:
	CNetwork();
	~CNetwork();

	void ServerToClientConnect();
	static DWORD WINAPI ClientComm(LPVOID arg);

	void ServerDataSet();

	static void DisplayText(const char* fmt, ...);	// ����Ʈ ��Ʈ�� ��� ��
	static void DisplayError(const char* msg);		// ���� �Լ� ���� ���

	void err_quit(const char* msg);
};