#include "pch.h"
#include "CNetwork.h"
#include "CUser.h"
#include "CTeam.h"
#include "CTower.h"
#include "CTurret.h"
#include "CUnit.h"
CNetwork::CNetwork()
{
	//������ ȣ�� �� ���ӻ���~connect()���� �����ϴ� ������
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
	printf("[����] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void CNetwork::ClientToServerComm()
{
	//������ �����ؼ� �����͸� �۽� �� �����ϰ� ClientDataSet()�� ȣ���Ͽ� ������ �����͸� �Ѱ��ش�.
	const char* temp = "m_player1";
	ServerDataSet(temp);

	// ������ ������ ���
	while (1) {
		// ������ �Է�

		// ������ ������
		Send();

		// ������ �ޱ�
		Recv();
	}
}

void CNetwork::ClientDataSet(CGameObject* inmylist, int inmynum, CGameObject* inopplist,int inoppnum)
{
	//�������� ���� �����͸� Ŭ���̾�Ʈ���� �����Ѵ�. CTeam�� p_myobjlist,p_opponentobjlist �� imyobjnum ũ�⸸ŭ list��ȸ�Ͽ� �����Ѵ�.

	//�������� ���� �����͸� CGameobject�� ����ȯ
	// list�� CGameopject������ ��ü���� �÷��̾�,Ÿ��,���� ������ ��ȯ�� ���Ḯ��Ʈ��. �÷��̾ ����. 
	//�߰��� �� ����� ��(���Ḯ��Ʈ ���� ��) ��ġ
	//���Ḯ��Ʈ�� list�� ����
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
	// gameobjlist�� Ÿ��, �÷��̾�, ���� ������ ����
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
	////enemy // gameobjlist�� Ÿ��, ��ž, ���� ������ ����
}

void CNetwork::ServerDataSet(const char* m_player)
{
	//Ŭ���̾�Ʈ���� ������ �÷��̾� �����͸� �����Ѵ�.
	strcpy_s(buf, BUFF_MAXSIZE+1, (char*)m_player);
}

void CNetwork::Connect()
{
	//������ �����ϱ� ���� �Լ��̸�, connect()�� �����Ѵ�
	// ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// ���� ����
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
	//������ �����͸� �����ϴ� �Լ��̸�, send()�� �����Ѵ�. CUser->Update()���� ����ȴ�.
	retval = send(sock, buf, (int)strlen(buf), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}
	printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
}

void CNetwork::Recv()
{
	// �������� �����͸� ���۹޴� �Լ��̸�, recv()�� �����Ѵ�.CUser->Update()���� ����ȴ�.
	// ������ �ޱ�
	retval = recv(sock, buf, retval, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return;
	}
	else if (retval == 0)
		return;

	// ���� ������ ���
	buf[retval] = '\0';
	printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
	printf("[���� ������] %s\n", buf);
}

void CNetwork::Close()
{
	//���� ����� �����ϴ� �Լ��̴�
	closesocket(sock);
	WSACleanup();
}

