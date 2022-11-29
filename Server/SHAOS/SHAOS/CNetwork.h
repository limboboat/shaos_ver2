#pragma once
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

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

	static HWND hEdit;			  // 에디트 컨트롤
	static CRITICAL_SECTION cs; // DisplayText()의 임계 영역

public:
	CNetwork();
	~CNetwork();

	void ServerToClientConnect();
	static DWORD WINAPI ClientComm(LPVOID arg);

	void ServerDataSet();

	static void DisplayText(const char* fmt, ...);	// 에디트 컨트롤 출력 함
	static void DisplayError(const char* msg);		// 소켓 함수 오류 출력

	void err_quit(const char* msg);
};