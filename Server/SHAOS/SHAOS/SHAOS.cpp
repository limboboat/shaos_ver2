#include "pch.h"
#include "GameFramework.h"
#include "CNetwork.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;										// 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING] = L"SHAOS";               // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING] = L"WndClass";         // 기본 창 클래스 이름입니다.
CGameFramework g_GameFrameWork;

CNetwork cnetwork;

// 송신데이터
CGameObject* p_myobjlist{ nullptr };
CGameObject* p_opponentobjlist{ nullptr };

CPlayer* player[3];	// 수신 데이터


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
void                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    TitleProc(HWND, UINT, WPARAM, LPARAM);
void	CALLBACK	TimerProc(HWND, UINT, UINT, DWORD);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, L"SHAOS");

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//  함수: MyRegisterClass()
//  용도: 창 클래스를 등록합니다.
void MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	wcex.lpfnWndProc = TitleProc;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName = L"TitleWndClass";

	RegisterClassEx(&wcex);

}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	RECT rcWindow{};
	GetWindowRect(GetDesktopWindow(), &rcWindow);         //전체 윈도우에서 창이 어디에 있는지 반환하는 함수(전체 윈도우의 해상도 구하는 함수,)
	RECT rcClient{ 0,0,CLIENT_WIDTH,CLIENT_HEIGHT };

	POINT Start{ (rcWindow.right - rcClient.right) / 2         //창이 윈도우 중간에 뜨도록 
		,(rcWindow.bottom - rcClient.bottom) / 2 };

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, dwStyle,
		Start.x,
		Start.y,
		rcClient.right,
		rcClient.bottom,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//타이틀 윈도우가 활성화되어있을 때는 부모 윈도우의 타이머를 꺼준다
// -> 게임 프레임워크의 함수를 부르지 않는다
// 메인 윈도우의 타이머관리를 타이틀 윈도우에서 해준다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		CreateWindow(L"TitleWndClass", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, rcClient.right, rcClient.bottom, hWnd, NULL, hInst, NULL);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		g_GameFrameWork.MSG_Key(message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:

		g_GameFrameWork.MSG_Mouse(message, wParam, lParam);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
		g_GameFrameWork.Relese();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK TitleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static HBITMAP hTitleBit;
	switch (message) {
	case WM_CREATE:
		break;
	case WM_COMMAND:
	{
		UINT ctrlID = LOWORD(wParam);
		switch (ctrlID) {
		case IDC_BUTTON_START:
			g_GameFrameWork.Create(GetParent(hWnd), hWnd, hInst);
			SetTimer(GetParent(hWnd), 0, FRAMETIME, (TIMERPROC)TimerProc);
			DestroyWindow(hWnd);
			break;

		case IDC_BUTTON_EXIT:
			DestroyWindow(GetParent(hWnd));
			break;
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP hOld = (HBITMAP)SelectObject(memdc, hTitleBit);
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		// 부모 죽으면 차일드는 알아서 죽는다
		DeleteObject(hTitleBit);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CALLBACK TimerProc(HWND hWnd, UINT iMsg, UINT idEvent, DWORD dwTime) {
	g_GameFrameWork.Update();
	InvalidateRect(hWnd, nullptr, false);
}
