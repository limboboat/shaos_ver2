#include <iostream>
#include "pch.h"
#include "CPlayer.h"
#include "CHp.h"
#include "Bullet.h"
using namespace std;

CPlayer::CPlayer(POINTFLOAT ainitPos, TEAM team, CGameObject* enemylist)
	: CGameObject(ainitPos, team, enemylist), iAoERadius(PLAYER_RADIUS * 7)
{
	// 충돌체크용 플레이어 영역 설정해주기
	mrcRng = { (LONG)mptpos.x - PLAYER_RADIUS,(LONG)mptpos.y - PLAYER_RADIUS,
		(LONG)mptpos.x + PLAYER_RADIUS, (LONG)mptpos.y + PLAYER_RADIUS };

	// hp 관련 초기화
	mhp = new CHp(PLAYER_MAXHP);
	mrchpbar = { mrcRng.left - 7, mrcRng.top, mrcRng.left - 4, mrcRng.bottom };

	// 이동
	R_On = FALSE;
	L_On = FALSE;
	U_On = FALSE;
	D_On = FALSE;

	// 스킬
	pressSft = FALSE;
	onshield = FALSE;
	pressQ = FALSE;

	activetime_shield = 0;
	castingtime_return = 0;


	effecttime_Shoot = 0;
	effecttime_AoE = 0;
	effecttime_Return = 0;



	cooltime_Shoot = 0;
	cooltime_AoE = 0;
	cooltime_Shield = 0;
	cooltime_Return = 0;
	cooltime_Death = 0;

	//공격
	iattackcooltime = FRAMETIME * 50;
	ptarget = nullptr;
	pbullet = nullptr;
	
	for (int i = 0; i < 7; i++) {
		shootattackrange[i] = { 0,0 };
	}
	immotal = FALSE;
}


CPlayer::~CPlayer()
{
	// hp 해제 
	delete mhp;
	if (pbullet) delete pbullet;
}


void CPlayer::MSG_Key(UINT message, WPARAM wParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F1:
			(immotal) ? immotal = FALSE : immotal = TRUE;
			break;
		case 'A':
			L_On = TRUE;
			break;
		case 'D':
			R_On = TRUE;
			break;
		case 'S':
			D_On = TRUE;
			break;
		case 'W':
			U_On = TRUE;
			break;
		case VK_SHIFT:
			if (!cooltime_Shoot && !pressSft) {
				pressSft = TRUE;
			}
			break;
		case VK_SPACE:
			if (!cooltime_AoE) {
				Skill_AreaOfEffect();
			}
			break;
		case 'Q':
			if (!pressQ && !cooltime_Return) {
				pressQ = TRUE;
				ReturnHome();				
			}
			break;
		case 'V':
			if (!cooltime_Shield) {
				ActiveShield();
			}
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
			L_On = FALSE;
			break;
		case 'D':
			R_On = FALSE;
			break;
		case 'S':
			D_On = FALSE;
			break;
		case 'W':
			U_On = FALSE;
			break;
		case VK_SHIFT:
			pressSft = FALSE;
			break;
		case 'Q':
			if (pressQ) {
				pressQ = FALSE;
				castingtime_return = 0;
			}
			break;
		}
		break;
	}
}


void CPlayer::MSG_MouseMove(POINT mousepos)
{
	worldmousepos = mousepos;
}

void CPlayer::MSG_MouseUp(POINT mousepos)
{
}

void CPlayer::MSG_MouseDown(POINT mousepos)
{
	if (pressSft) {
		Skill_Shoot();
		pressSft = FALSE;
		return;
	}

	CGameObject* tmp = nullptr;
	while (tmp != menemylist) {
		if (!tmp) tmp = menemylist;

		if (tmp->IsDead()) {
			tmp = tmp->next;
			continue;
		}
		const RECT rect = tmp->GetRng();
		if (PtInRect(&rect, mousepos)) {
			float dx = mptpos.x - tmp->GetPos().x;
			float dy = mptpos.y - tmp->GetPos().y;

			float center_d = dx * dx + dy * dy;
			float range = PLAYER_ATTACK_RANGE + tmp->GetObjRadius();
			if (center_d <= range * range) {
				ptarget = tmp;
				return;
			}
		}
		tmp = tmp->next;
	}
}



void CPlayer::Move() {
	if (pressSft) return;
	if (effecttime_Return) return;
	if (pressQ) {
		if (R_On || L_On || U_On || D_On) {
			pressQ = FALSE;
			castingtime_return = 0;
		}
	}

	// 플레이어 중심점 좌표
	POINTFLOAT dirvector = this->Player_Vector();

	POINTFLOAT nextpos = {mptpos.x + dirvector.x,mptpos.y + dirvector.y};
	
	FLOAT etd = sqrt((ENEMYTOWER_POS.x - nextpos.x) * (ENEMYTOWER_POS.x - nextpos.x) +
		(ENEMYTOWER_POS.y - nextpos.y) * (ENEMYTOWER_POS.y - nextpos.y));
	
	if (etd <= TOWER_CENTER2VERTAX + PLAYER_RADIUS)
		return;

	FLOAT utd = sqrt((USERTOWER_POS.x - nextpos.x) * (USERTOWER_POS.x - nextpos.x) +
		(USERTOWER_POS.y - nextpos.y) * (USERTOWER_POS.y - nextpos.y));

	if (utd <= TOWER_CENTER2VERTAX + PLAYER_RADIUS)
		return;

	mptpos.x = PLAY_CLIENTX(mptpos.x + dirvector.x);
	mptpos.y = PLAY_CLIENTY(mptpos.y + dirvector.y);

	// 플레이어 영역
	mrcRng = { (LONG)mptpos.x - PLAYER_RADIUS,(LONG)mptpos.y - PLAYER_RADIUS,
		(LONG)mptpos.x + PLAYER_RADIUS, (LONG)mptpos.y + PLAYER_RADIUS };

}

void CPlayer::Skill_AreaOfEffect()
{
	cooltime_AoE = COOLTIME_AOE;

	// 이펙트 신호 주기
	effecttime_AoE = PLAYER_EFFECTTIME_AOE;

	// 충돌체크해서 데미지 주기
	CGameObject* tmp = nullptr;
	while (tmp != menemylist) {
		if (!tmp) tmp = menemylist;
		
		// 두 원의 중심의 거리가 두 원의 반지름의 합보다 작은 경우 충돌
		float dx = mptpos.x - tmp->GetPos().x;
		float dy = mptpos.y - tmp->GetPos().y;

		float center_d = dx * dx + dy * dy;
		float range = iAoERadius + tmp->GetObjRadius();
		if (center_d <= range*range) {
			tmp->PutDamage(PLAYER_AOEDAMAGE);
		}

		tmp = tmp->next;
	}
}

void CPlayer::Skill_Shoot()
{
	cooltime_Shoot = COOLTIME_SHOOT;
	effecttime_Shoot = FRAMETIME * 25;

	float dx = worldmousepos.x - mptpos.x;
	float dy = worldmousepos.y - mptpos.y;

	float sizeofvector = sqrt(dx * dx + dy * dy);
	
	float standardx = dx / sizeofvector * 25;
	float standardy = dy / sizeofvector * 25;
	// 노말라이즈 구했고 사각형 7개 구하기

	for (int i = 0; i < 7; i++) {
		shootattackpt[i] = { (INT)(mptpos.x + standardx * (i + 1)),
			(INT)(mptpos.y + standardy * (i + 1)) };

		shootattackrange[i] = {
			shootattackpt[i].x - 25, shootattackpt[i].y - 25,
			shootattackpt[i].x + 25, shootattackpt[i].y + 25
		};
	}

	CGameObject* tmp = nullptr;
	while (tmp != menemylist) {
		if (!tmp) tmp = menemylist;

		RECT lprcDest;
		const RECT lprcSrc2 = tmp->GetRng();
		if (IntersectRect(&lprcDest, &shootattackrange[0], &lprcSrc2) ||
			IntersectRect(&lprcDest, &shootattackrange[1], &lprcSrc2) ||
			IntersectRect(&lprcDest, &shootattackrange[2], &lprcSrc2) ||
			IntersectRect(&lprcDest, &shootattackrange[3], &lprcSrc2) ||
			IntersectRect(&lprcDest, &shootattackrange[4], &lprcSrc2) ||
			IntersectRect(&lprcDest, &shootattackrange[5], &lprcSrc2) ||
			IntersectRect(&lprcDest, &shootattackrange[6], &lprcSrc2)) {
			tmp->PutDamage(PLAYER_SHOOTDAMAGE);
		}

		tmp = tmp->next;
	}

}

void CPlayer::ReturnHome()
{
	castingtime_return = CASTINGTIME_RETURN;
}

void CPlayer::ActiveShield()
{
	onshield = TRUE;
	activetime_shield = ACTIVETIME_SHIELD;
	cooltime_Shield = COOLTIME_SHIELD;
}


POINTFLOAT CPlayer::Player_Vector()
{
	if (R_On && U_On)
		return DIRVECTOR_RT;
	if (R_On && D_On)
		return DIRVECTOR_RB;
	if (D_On && L_On)
		return DIRVECTOR_LB;
	if (U_On && L_On)
		return DIRVECTOR_LT;
	if (R_On)
		return DIRVECTOR_RIGHT;
	if (L_On)
		return DIRVECTOR_LEFT;
	if (U_On)
		return DIRVECTOR_TOP;
	if (D_On)
		return DIRVECTOR_BOTTOM;
	return DIRVECTOR_STOP;
}


void CPlayer::SetPos(POINT setpos)
{
	mptpos.x = (INT)setpos.x;
	mptpos.y = (INT)setpos.y;
}
void CPlayer::UI_GetPlayerInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
	INT* ct_shield, INT* ct_return, INT* ct_death)
{
	*ahp = mhp->GetHp();
	*ct_shoot = cooltime_Shoot;
	*ct_AoE = cooltime_AoE;
	*ct_shield = cooltime_Shield;
	*ct_return = cooltime_Return;
	*ct_death = cooltime_Death;

}
void CPlayer::PutDamage(INT damage)
{
	if (immotal) return;
	if (effecttime_Return) return;

	// 귀환 취소
	if (pressQ) {
		pressQ = FALSE;
		castingtime_return = 0;
		cooltime_Return = FRAMETIME * 20;
	}
	
	// 쉴드가 켜져있을 때 데미지가 1/2만 들어가게
	if (onshield) damage /= 2;

	if (mhp->SubHp(damage)) {
		if (!mdeath) Death();
		mdeath = TRUE;
	}
}
INT CPlayer::GetEffectTimeAoE()
{
	return effecttime_AoE;
}

void CPlayer::OffPlayerMove()
{
	R_On = FALSE;
	L_On = FALSE;
	U_On = FALSE;
	D_On = FALSE;
}
INT CPlayer::GetObjRadius()
{
	return PLAYER_RADIUS;
}
BOOL CPlayer::Attack()
{
	if (!ptarget) return FALSE;
	pbullet = new Bullet(&mptpos, ptarget, PLAYER_BULLETDAMAGE);
	return TRUE;
}

void CPlayer::Draw(HDC hdc)
{
}


void CPlayer::Update()
{

	// 죽음
	if (ideatheffecttime) {
		if (ideatheffecttime == PLAYER_EFFECTTIME_DEATH) {
		}

		ideatheffecttime -= FRAMETIME;
		if (!ideatheffecttime) {
			SetPos(PLAYER_DEFAULT_POSITION);
			mhp->SetHp(PLAYER_MAXHP);
			mdeath = FALSE;
		}
		return;	// 죽었으면 업데이트 멈추기
	}


	// 플레이어 움직임
	Move();

	// 기지에 있으면 체력 회복
	if (mrcRng.left <= 650) {
		mhp->AddHp(RECOVERAMOUNT);
	}


	if (effecttime_Shoot) effecttime_Shoot -= FRAMETIME;
	if (effecttime_AoE) effecttime_AoE -= FRAMETIME;

	if (effecttime_Return) {
		effecttime_Return -= FRAMETIME;
		if (!effecttime_Return)
		{
			SetPos(PLAYER_DEFAULT_POSITION);
		}
	}

	if (pressQ) {
		if (castingtime_return == 0)
		{
			cooltime_Return = COOLTIME_RETURN;
			effecttime_Return = FRAMETIME * 50;
			pressQ = FALSE;
		}
		castingtime_return -= FRAMETIME;
	}

	if (onshield) {
		if (activetime_shield == 0) {
			onshield = FALSE;
		}
		activetime_shield -= FRAMETIME;
	}


	// 쿨타임이 0이 아닐 때 감소
	if (cooltime_Shoot) {
		cooltime_Shoot -= FRAMETIME;
		if (!cooltime_Shoot) {
			// 쿨타임 끝~
		}
	}
	if (cooltime_AoE) {
		cooltime_AoE -= FRAMETIME;
		if (!cooltime_AoE) {
		}
	}
	if (cooltime_Shield)
	{
		cooltime_Shield -= FRAMETIME;
		if (!cooltime_Shield) {
		}
	}
	if (cooltime_Return)
	{
		cooltime_Return -= FRAMETIME;
		if (!cooltime_Return) {
		}
	}

	if (iattackcooltime)
		iattackcooltime -= FRAMETIME;
	else {
		if (Attack())
			iattackcooltime = FRAMETIME * 50;
	}

	if (pbullet) pbullet = pbullet->Move();

	if (ptarget) {
		if (ptarget->IsDead()) {		// 내 공격으로 죽었을 때
			ptarget = nullptr;
			return;
		}

		// 플레이어 공격 범위에서 벗어났을 때
		float dx = mptpos.x - ptarget->GetPos().x;
		float dy = mptpos.y - ptarget->GetPos().y;

		float center_d = dx * dx + dy * dy;
		float range = PLAYER_ATTACK_RANGE + ptarget->GetObjRadius();
		if (center_d > range * range) {
			ptarget = nullptr;
			return;
		}

	}
	return;
}

void CPlayer::Death()
{
	ideatheffecttime = PLAYER_EFFECTTIME_DEATH;
}
