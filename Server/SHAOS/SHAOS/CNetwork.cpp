#include "pch.h"
#include "CNetwork.h"

CNetwork::CNetwork()
{
	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// 소켓 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}

CNetwork::~CNetwork()
{
	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
}

void CNetwork::ServerToClientConnect()
{
	/*
	클라이언트 세개의 접속을 Accept 해주고, 클라이언트가 접속할 때 마다
	CreateThread(ClientComm)를 호출하여 송수신용 쓰레드를 만들어 준다.
	*/

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			DisplayError("accept()");
			break;
		}

		//// 접속한 클라이언트 정보 출력
		//char addr[INET_ADDRSTRLEN];
		//inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		//printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		//	addr, ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ClientComm,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
}

DWORD WINAPI CNetwork::ClientComm(LPVOID arg)
{
	/*
		클라이언트에게서 struct Client_Data CData 를 수신하고 ServerDataSet()를
		호출하여 struct Server_Data SData;를 수정해 준 뒤,
		struct Server_Data SData를 클라이언트에게 송신.
	*/

	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));


	/*
	여기 While문 수정 필요. 데이터 송수신 정의.
	*/
	while (1) {
		// 데이터 받기
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			DisplayError("recv()");
			break;
		}
		else if (retval == 0)
			break;

		//// 받은 데이터 출력
		//buf[retval] = '\0';
		//printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		// 데이터 보내기
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			DisplayError("send()");
			break;
		}
	}

	// 소켓 닫기
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));
	return 0;
}

void CNetwork::ServerDataSet()
{
	// 받은 클라이언트의 데이터를 serverData에 적용.

	// 받은 클라이언트의 데이터를 serverData에 적용한다.

}

void CNetwork::DisplayText(const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[BUFSIZE * 2];
	vsprintf_s(cbuf, fmt, arg);
	va_end(arg);

	EnterCriticalSection(&cs);
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessageA(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);
}

void CNetwork::DisplayError(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	DisplayText("[%s] %s\r\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
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