#pragma once
//---클라이언트 네트워크 송수신 객체
#include "CPlayer.h"

//-----------------네트워크 전처리기
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

//#include <filesystem>
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...
#include <iostream>

#include <fstream>
using namespace std;

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크
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
	int My_objTag; // 게임 객체의 태그값을 저장하여 객체 타입을 구별하는 변수
	int len;
	char buf[BUFF_MAXSIZE+1]; // 데이터 전송 버퍼
	int retval; // 에러 확인용 변수
public:
	WSADATA wsa; // 윈속 생성
	SOCKET sock; // 소켓 생성
	struct sockaddr_in serveraddr; // 소켓 주소 구조체
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

