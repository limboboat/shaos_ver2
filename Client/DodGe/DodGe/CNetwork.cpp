#include "CNetwork.h"

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

DWORD WINAPI CNetwork::ClientToServerComm(LPVOID arg)
{
	//������ �����ؼ� �����͸� �۽� �� �����ϰ� ClientDataSet()�� ȣ���Ͽ� ������ �����͸� �Ѱ��ش�.
	Connect();
	// ������ ������ ���
	while (1) {
		// ���� ������ �Է�
		ServerDataSet(buf);
		// ������ ������
		Send();
		// ������ �ޱ�
		Recv();
		// ���� ������ ����
		ClientDataSet();
	}
	Close();
}
void CNetwork::ClientToServerComm()
{
	//������ �����ؼ� �����͸� �۽� �� �����ϰ� ClientDataSet()�� ȣ���Ͽ� ������ �����͸� �Ѱ��ش�.
	Connect();
	// ������ ������ ���
	while (1) {
		// ���� ������ �Է�
		ServerDataSet(buf);
		// ������ ������
		Send();
		// ������ �ޱ�
		Recv();
		// ���� ������ ����
		ClientDataSet();
	}
	Close();
}
/*
void CNetwork::ClientDataSet(CGameObject* inmylist, int inmynum, CGameObject* inopplist,int inoppnum)
{
	//�������� ���� �����͸� Ŭ���̾�Ʈ���� �����Ѵ�. CTeam�� p_myobjlist,p_opponentobjlist �� imyobjnum ũ�⸸ŭ list��ȸ�Ͽ� �����Ѵ�.

	//�������� ���� �����͸� CGameobject�� ����ȯ
	 //list�� CGameopject������ ��ü���� �÷��̾�,Ÿ��,���� ������ ��ȯ�� ���Ḯ��Ʈ��. �÷��̾ ����. 
	//�߰��� �� ����� ��(���Ḯ��Ʈ ���� ��) ��ġ
	//���Ḯ��Ʈ�� list�� ����
	CTeam* team;
	team->SetOpponentObj(inopplist);
	CGameObject* tmp = p_myobjlist;
	for (int i=0; i <inmynum;i++)
	{
		tmp = inmylist;
		tmp->prev = inmylist->prev;
		tmp->next = inmylist->next;

		tmp = tmp->next;
		inmylist = inmylist->next;
	}

	for (int i = 0; i < inoppnum; i++)
	{
		p_opponentobjlist = inopplist;
		p_opponentobjlist->prev = inopplist->prev;
		p_opponentobjlist->next = inopplist->next;
		p_opponentobjlist = p_opponentobjlist->next;
		inopplist = inopplist->next;
	}
}
*/
void CNetwork::ClientDataSet()
{
	 // user - gameobjlist�� Ÿ��, �÷��̾�, ���� ������ ����
	switch (My_objTag)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	default :
		break;
	}
	// enemy - gameobjlist�� Ÿ��, ��ž, ���� ������ ����
}

void CNetwork::ServerDataSet(const char* m_player)
{
	//Ŭ���̾�Ʈ���� ������ �÷��̾� �����͸� �����Ѵ�.
	strcpy_s(buf, BUFF_MAXSIZE+1, (char*)m_player);

	printf("\n[���� ������] ");
	if (fgets(buf, BUFF_MAXSIZE + 1, stdin) == NULL)
		return;

	// '\n' ���� ����
	len = (int)strlen(buf);
	if (buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	if (strlen(buf) == 0)
		return;
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

