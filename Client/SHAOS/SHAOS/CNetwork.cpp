#include "pch.h"
#include "CNetwork.h"
#include "CUser.h"
#include "CTeam.h"
#include "CTower.h"
#include "CTurret.h"
#include "CUnit.h"
CNetwork::CNetwork()
{
	//생성자 호출 시 윈속생성~connect()까지 관여하는 생성자
	My_objTag = -1;
	len = 0;
	retval = 0;
}

CNetwork::~CNetwork()
{
}

void CNetwork::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CNetwork::err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void CNetwork::err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void CNetwork::ClientToServerComm()
{
	//서버에 접속해서 데이터를 송신 및 수신하고 ClientDataSet()을 호출하여 수신한 데이터를 넘겨준다.
	const char* temp = "m_player1";
	ServerDataSet(temp);

	// 서버와 데이터 통신
	while (1) {
		// 데이터 입력

		// 데이터 보내기
		Send();

		// 데이터 받기
		Recv();
	}
}

void CNetwork::ClientDataSet(CGameObject* inmylist, int inmynum, CGameObject* inopplist,int inoppnum)
{
	//서버에서 받은 데이터를 클라이언트에서 적용한다. CTeam의 p_myobjlist,p_opponentobjlist 를 imyobjnum 크기만큼 list순회하여 매핑한다.

	//서버에서 받은 데이터를 CGameobject로 형변환
	// list는 CGameopject형태의 객체에서 플레이어,타워,유닛 순으로 순환형 연결리스트다. 플레이어가 헤드다. 
	//추가될 시 헤드의 앞(연결리스트 제일 끝) 위치
	//연결리스트로 list에 저장
	//CTeam* team;
	//team->SetOpponentObj(inopplist);
	//CGameObject* tmp = p_myobjlist;
	//for (int i=0; i <inmynum;i++)
	//{
	//	tmp = inmylist;
	//	tmp->prev = inmylist->prev;
	//	tmp->next = inmylist->next;

	//	tmp = tmp->next;
	//	inmylist = inmylist->next;
	//}

	//for (int i = 0; i < inoppnum; i++)
	//{
	//	p_opponentobjlist = inopplist;
	//	p_opponentobjlist->prev = inopplist->prev;
	//	p_opponentobjlist->next = inopplist->next;
	//	p_opponentobjlist = p_opponentobjlist->next;
	//	inopplist = inopplist->next;
	//}
}

void CNetwork::ClientDataSet()
{
	// gameobjlist에 타워, 플레이어, 유닛 순으로 연결
	//switch (My_objTag&&p_myobjlist)
	//{
	//case 0:
	//	CGameObject* tmp = (CPlayer*)buf;
	//	p_myobjlist = tmp;
	//	break;
	//case 1:
	//	CGameObject * tmp = (CTower*)buf;
	//	p_myobjlist->next = tmp;
	//	break;
	//case 2:
	//	CGameObject * tmp = (CUnit*)buf;
	//	tmp->next = p_myobjlist;
	//	tmp->prev = p_myobjlist->prev;
	//	p_myobjlist->prev->next = tmp;
	//	p_myobjlist->prev = tmp;
	//}
	////enemy // gameobjlist에 타워, 포탑, 유닛 순으로 연결
}

void CNetwork::ServerDataSet(const char* m_player)
{
	//클라이언트에서 전송할 플레이어 데이터를 매핑한다.
	strcpy_s(buf, BUFF_MAXSIZE+1, (char*)m_player);
}

void CNetwork::Connect()
{
	//서버에 접속하기 위한 함수이며, connect()를 포함한다
	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
}

void CNetwork::Send()
{
	//서버에 데이터를 전송하는 함수이며, send()를 포함한다. CUser->Update()에서 실행된다.
	retval = send(sock, buf, (int)strlen(buf), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}
	printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
}

void CNetwork::Recv()
{
	// 서버에서 데이터를 전송받는 함수이며, recv()를 포함한다.CUser->Update()에서 실행된다.
	// 데이터 받기
	retval = recv(sock, buf, retval, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return;
	}
	else if (retval == 0)
		return;

	// 받은 데이터 출력
	buf[retval] = '\0';
	printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
	printf("[받은 데이터] %s\n", buf);
}

void CNetwork::Close()
{
	//소켓 통신을 종료하는 함수이다
	closesocket(sock);
	WSACleanup();
}

