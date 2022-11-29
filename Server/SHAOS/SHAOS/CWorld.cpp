#include "pch.h"
#include "CWorld.h"

CWorld::CWorld(HWND hwnd)
{
	GetClientRect(hwnd, &rcClient);

	// 팀 할당받기
	pUserTeam = new CUser;
	pEnemyTeam = new CEnemy;

	// 상대편 객체 리스트 설정
	pUserTeam->SetOpponentObj(pEnemyTeam->GetMyObjList());
	pEnemyTeam->SetOpponentObj(pUserTeam->GetMyObjList());

	// 초기 오브젝트 할당
	pUserTeam->SetInitObj();
	pEnemyTeam->SetInitObj();

	gamestate = 0;	// 진행중
}


CWorld::~CWorld()
{
	// 팀 해제
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

	// 타워 죽음 이펙트가 끝났을 때
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

	// 타워가 죽었을 때
	if (pUserTeam->GetMyObjList()->IsDead()) {
		// 타워 죽었을 때 효과음
		return;
	}
	else if (pEnemyTeam->GetMyObjList()->IsDead()) {
		// 타워 죽었을 때 효과음
		return;
	}

	if (pUserTeam->GeteffecttimeAoE()) {
		// 15프레임
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
