#include "pch.h"
#include "CTurret.h"
#include "Bullet.h"

CTurret::CTurret(POINTFLOAT initPos, TEAM team, CGameObject* enemylist)
	: CGameObject(initPos, team, enemylist)
{
	mrcRng = { (LONG)initPos.x - TURRET_RADIUS ,(LONG)initPos.y - TURRET_RADIUS,
		(LONG)initPos.x + TURRET_RADIUS, (LONG)initPos.y + TURRET_RADIUS };


	mhp = new CHp(TURRET_MAXHP);
	mrchpbar = { mrcRng.right + 4, mrcRng.top, mrcRng.right + 7, mrcRng.bottom };

	iattackcooltime = 0;
	ptarget = nullptr;
	pbullet = nullptr;

	ideatheffecttime = 0;
}


CTurret::~CTurret()
{
	delete mhp;
	if (pbullet) delete pbullet;
}

void CTurret::Draw(HDC hdc)
{
}

void CTurret::Update()
{
	if (mdeath) {
		if(ideatheffecttime > FRAMETIME) ideatheffecttime -= FRAMETIME;
		return;
	}

	// hp바 업데이트
	mrchpbar.top = mrcRng.bottom - GETHPBAR(mhp->GetHp(), TURRET_RADIUS * 2, TURRET_MAXHP);


	ptarget = FindTarget();
	if (ptarget == nullptr) {
		iattackcooltime = 0;
	}

	if (!iattackcooltime) {
		if (ptarget && !pbullet) {
			Attack();
			iattackcooltime = FRAMETIME * 100;
		}
	}
	else {
		iattackcooltime -= FRAMETIME;
	}

	if (pbullet) pbullet = pbullet->Move();


}

CGameObject* CTurret::FindTarget()
{
	// 먼저 생성된 유닛 먼저 공격하는 알고리즘
	CGameObject* tmp = menemylist->next->next;	//유닛 먼저 검사
	while (tmp != menemylist) {
		if (tmp->IsDead()) {
			tmp = tmp->next;
			continue;
		}

		INT range = TURRET_ATTACK_RANGE + tmp->GetObjRadius();
		if (IsInRange(this, tmp, range)) {
			return tmp;					// 뭐가 됐든 타겟이 있으면 리턴
		}
		tmp = tmp->next;
	}

	INT range = TURRET_ATTACK_RANGE + PLAYER_RADIUS;
	if (IsInRange(this, menemylist->next, range)) {
		return menemylist->next;
	}

	return nullptr;
}

void CTurret::Attack()
{
	if (ptarget == menemylist->next) // 플레이어이면
	{
		pbullet = new Bullet(&mptpos, ptarget, TURRET_BULLETDAMAGE*4);
	}
	else pbullet = new Bullet(&mptpos, ptarget, TURRET_BULLETDAMAGE);
}

void CTurret::SelectedDraw(HDC hdc, HBRUSH hbr)
{
	HBRUSH hOld = (HBRUSH)SelectObject(hdc, hbr);

	RoundRect(hdc, mrcRng.left - 3, mrcRng.top - 3,
		mrcRng.right + 4, mrcRng.bottom + 4,
		TURRET_RADIUS / 5 * 3, TURRET_RADIUS / 5 * 3);

	SelectObject(hdc, hOld);
}

INT CTurret::GetObjRadius()
{
	return TURRET_RADIUS;
}

void CTurret::Death()
{
	
	ideatheffecttime = TURRET_EFFECTTIME_DEATH;
}