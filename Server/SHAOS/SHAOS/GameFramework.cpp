#include "pch.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
}
CGameFramework::~CGameFramework()
{
}

void CGameFramework::Create(HWND hwnd, HWND htitlewnd, HINSTANCE hInst)
{
	mhInst = hInst;
	mhWnd = hwnd;
	mhhTitleWnd = htitlewnd;

	pworld = new CWorld(hwnd);

	HDC hdc = GetDC(hwnd);
	memdc = CreateCompatibleDC(hdc);
	ReleaseDC(hwnd, hdc);

	hpausebutton = (HBITMAP)LoadImage(NULL, L"Resource/button_pause.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_shoot = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_shoot.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_aoe = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_aoe.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_shield = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_shield.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_return = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_return.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

}

void CGameFramework::Relese()
{

	DeleteObject(hpausebutton);
	DeleteObject(hskill_shoot);
	DeleteObject(hskill_aoe);
	DeleteObject(hskill_shield);
	DeleteObject(hskill_return);

	DeleteDC(memdc);
	delete pworld;
	pworld = nullptr;
}

void CGameFramework::Update()
{
	if (pworld) pworld->Update();
	pworld->GetUIInfo(&p_hp, &p_ctshoot, &p_ctAoE, 
		&p_ctshield, &p_ctreturn, &t_hp , &p_ctdeath);

}

void CGameFramework::MSG_Key(UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (pworld) pworld->MSG_Key(message, wParam, lParam);
}

void CGameFramework::MSG_Mouse(UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT mousepos = { LOWORD(lParam),HIWORD(lParam) };
	if (PtInRect(&BUTTONPAUSE_RNG, mousepos)) {
		if (message == WM_LBUTTONUP) {
			pworld->OffPlayerMove();
			//DialogBox(mhInst, MAKEINTRESOURCE(IDD_DIALOGPAUSE), mhWnd, (DLGPROC)DialogProc);
		}
		return;
	}
	else {
		if (pworld) pworld->MSG_Mouse(message, mousepos);
	}

}



void CGameFramework::Draw(HDC hdc)
{
}


