#include "pch.h"
#include "CWorld.h"

CWorld::CWorld(HWND hwnd)
{
	GetClientRect(hwnd, &rcClient);

	// �� �Ҵ�ޱ�
	pUserTeam = new CUser;
	pEnemyTeam = new CEnemy;

	// ����� ��ü ����Ʈ ����
	pUserTeam->SetOpponentObj(pEnemyTeam->GetMyObjList());
	pEnemyTeam->SetOpponentObj(pUserTeam->GetMyObjList());

	// �ʱ� ������Ʈ �Ҵ�
	pUserTeam->SetInitObj();
	pEnemyTeam->SetInitObj();

	gamestate = 0;	// ������
}


CWorld::~CWorld()
{
	// �� ����
	delete pUserTeam;
	delete pEnemyTeam;

}

void CWorld::MSG_Mouse(UINT message, POINT clientmousepos)
{
	POINT worldmousepos =
	{ clientmousepos.x + iViewX - MIN_VIEWX,clientmousepos.y };

	pUserTeam->MSG_Mouse(message, worldmousepos);

}

void CWorld::MSG_Key(UINT message, WPARAM wParam, LPARAM lParam)
{
	pUserTeam->MSG_Key(message, wParam);
}

void CWorld::Update()
{

	// Ÿ�� ���� ����Ʈ�� ������ ��
	if (pUserTeam->gameover) {
		if (gamestate == 0) 
		gamestate = 2;
		return;
	}
	else if (pEnemyTeam->gameover)
	{
		if (gamestate == 0)
		gamestate = 1;
		return;
	}

	pUserTeam->Update();
	pEnemyTeam->Update();

	// Ÿ���� �׾��� ��
	if (pUserTeam->GetMyObjList()->IsDead()) {
		// Ÿ�� �׾��� �� ȿ����
		return;
	}
	else if (pEnemyTeam->GetMyObjList()->IsDead()) {
		// Ÿ�� �׾��� �� ȿ����
		return;
	}

	if (pUserTeam->GeteffecttimeAoE()) {
		// 15������
		iViewX = (INT)SET_VIEWX(pUserTeam->GetPlayerPos().x);

		switch (pUserTeam->GeteffecttimeAoE()) {
		case FRAMETIME * 15:
		case FRAMETIME * 14:
		case FRAMETIME * 13:
		case FRAMETIME * 9:
		case FRAMETIME * 8:
		case FRAMETIME * 7:
		case FRAMETIME * 3:
		case FRAMETIME * 2:
		case FRAMETIME * 1:
			iViewX += 3;
			break;
		case FRAMETIME * 12:
		case FRAMETIME * 11:
		case FRAMETIME * 10:
		case FRAMETIME * 6:
		case FRAMETIME * 5:
		case FRAMETIME * 4:
			iViewX -= 3;
			break;
		default:
			break;
		}
		return;
	}

	iViewX = (INT)SET_VIEWX(pUserTeam->GetPlayerPos().x);	
}

void CWorld::Draw(HDC clientDC)
{
}

void CWorld::GetUIInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
	INT* ct_shield, INT* ct_return, INT* towerhp, INT* ct_death)
{
	pUserTeam->GetUIInfo(ahp, ct_shoot, ct_AoE, ct_shield, ct_return, towerhp, ct_death);
}

INT CWorld::IsEnding()
{
	return gamestate;
}

void CWorld::OffPlayerMove()
{
	pUserTeam->OffPlayerMove();
}
