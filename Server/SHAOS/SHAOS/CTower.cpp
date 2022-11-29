#include "pch.h"
#include "CTower.h"

CTower::CTower(POINTFLOAT initPos, TEAM team, CGameObject* enemylist)
	: CGameObject(initPos, team, enemylist)
{
	mrcRng = { (LONG)(mptpos.x - TOWER_CENTER2VERTAX),
		(LONG)(mptpos.y - TOWER_CENTER2VERTAX),
		(LONG)(mptpos.x + TOWER_CENTER2VERTAX),
		(LONG)(mptpos.y + TOWER_CENTER2VERTAX)
	};

	//// 먼저 그리는 삼각형
	//triangle1[0] = { (LONG)mptpos.x, (LONG)(mptpos.y + TOWER_CENTER2VERTAX) };
	//triangle1[1] = { (LONG)(mptpos.x - TOWER_HALFSIDE),(LONG)(mptpos.y - TOWER_CENTER2SIDE) };
	//triangle1[2] = { (LONG)(mptpos.x + TOWER_HALFSIDE),(LONG)(mptpos.y - TOWER_CENTER2SIDE) };
	//// 나중에 그리는 삼각형
	//triangle2[0] = { (LONG)mptpos.x, (LONG)(mptpos.y - TOWER_CENTER2VERTAX) };
	//triangle2[1] = { (LONG)(mptpos.x - TOWER_HALFSIDE),(LONG)(mptpos.y + TOWER_CENTER2SIDE) };
	//triangle2[2] = { (LONG)(mptpos.x + TOWER_HALFSIDE),(LONG)(mptpos.y + TOWER_CENTER2SIDE) };

	//mhp = new CHp(TOWER_MAXHP);
	//if (team == TEAM::USER) {
	//	mrchpbar = { mrcRng.left - 7, mrcRng.top, mrcRng.left - 4, mrcRng.bottom };
	//	hTriBrush = CreateSolidBrush(RGB(0, 0, 200));
	//}
	//else {
	//	mrchpbar = { mrcRng.right + 4, mrcRng.top, mrcRng.right + 7, mrcRng.bottom };
	//	hTriBrush = CreateSolidBrush(RGB(200, 0, 0));
	//}
}


CTower::~CTower()
{
	DeleteObject(hTriBrush);
	delete mhp;
}


void CTower::Draw(HDC hdc)
{
}

void CTower::SelectedDraw(HDC hdc, HBRUSH hbr)
{
}

void CTower::Update()
{
	mrchpbar.top = mrcRng.bottom - (INT)GETHPBAR(mhp->GetHp(), TOWER_CENTER2VERTAX * 2, TOWER_MAXHP);
	
	
	if (ideatheffecttime) {
		if (ideatheffecttime == TOWER_EFFECTTIME_DEATH) {
		}
		ideatheffecttime -= FRAMETIME;
	}
}

INT CTower::GetObjRadius()
{
	return TOWER_CENTER2VERTAX;
}

void CTower::Death()
{
	if (!ideatheffecttime) {
		ideatheffecttime = TOWER_EFFECTTIME_DEATH;
	}
}

INT CTower::GetTowerHp()
{
	return mhp->GetHp();
}

