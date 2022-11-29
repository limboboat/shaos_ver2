#include "pch.h"
#include "CPlayer.h"
#include "CHp.h"
#include "Bullet.h"
#include "Sound.h"
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

	msound = new CSound;
	
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
			keyInform.A_Key = TRUE;
			break;
		case 'D':
			keyInform.D_Key = TRUE;
			break;
		case 'S':
			keyInform.S_Key = TRUE;
			break;
		case 'W':
			keyInform.W_Key = TRUE;
			break;
		case VK_SHIFT:
			keyInform.Sft_Key = TRUE;
			break;
		case VK_SPACE:
			keyInform.SPACE_KEY = TRUE;
			break;
		case 'Q':
			keyInform.Q_Key = TRUE;
			break;
		case 'V':
			keyInform.V_Key = TRUE;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
			keyInform.A_Key = FALSE;
			break;
		case 'D':
			keyInform.D_Key = FALSE;
			break;
		case 'S':
			keyInform.S_Key = FALSE;
			break;
		case 'W':
			keyInform.W_Key = FALSE;
			break;
		case VK_SHIFT:
			keyInform.Sft_Key = FALSE;
			break;
		case 'Q':
			keyInform.Q_Key = FALSE;
			break;
		}
		break;
	}
}


void CPlayer::MSG_MouseMove(POINT mousepos)
{
	keyInform.mousepos_x = mousepos.x;
	keyInform.mousepos_y = mousepos.y;
}

void CPlayer::MSG_MouseUp(POINT mousepos)
{
}
void CPlayer::MSG_MouseDown(POINT mousepos)
{
	keyInform.mouse_Down = TRUE;
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

	//RECT tmp;
	//RECT nextrc = { (LONG)nextpos.x - PLAYER_RADIUS,(LONG)nextpos.y - PLAYER_RADIUS,
	//	(LONG)nextpos.x + PLAYER_RADIUS, (LONG)nextpos.y + PLAYER_RADIUS };
	//
	//
	//if (IntersectRect(&tmp, &nextrc, &rcTURRET1) ||
	//	IntersectRect(&tmp, &nextrc, &rcTURRET2) ||
	//	IntersectRect(&tmp, &nextrc, &rcTURRET3) ||
	//	IntersectRect(&tmp, &nextrc, &rcTURRET4)
	//	) {
	//	return;
	//}

	mptpos.x = PLAY_CLIENTX(mptpos.x + dirvector.x);
	mptpos.y = PLAY_CLIENTY(mptpos.y + dirvector.y);

	// 플레이어 영역
	mrcRng = { (LONG)mptpos.x - PLAYER_RADIUS,(LONG)mptpos.y - PLAYER_RADIUS,
		(LONG)mptpos.x + PLAYER_RADIUS, (LONG)mptpos.y + PLAYER_RADIUS };

}

void CPlayer::Skill_AreaOfEffect()
{
	msound->MyPlaySound(8, 3);
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

	msound->MyPlaySound(7, 5);
	//msound->SetVolum(5, 1);
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
	msound->MyPlaySound(9, 3);
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
	msound->MyPlaySound(2, 3);
	return TRUE;
}

void CPlayer::Draw(HDC hdc)
{

	if (mdeath) {
		// 죽었을 때 이펙트 그리기

		return;
	}


	if (pressSft) {
		// shoot의 공격경로 그리기
		// 방향벡터를 노말라이즈해서 길이만큼 곱한다

		float dx = worldmousepos.x - mptpos.x;
		float dy = worldmousepos.y - mptpos.y;

		float sizeofvector = sqrt(dx * dx + dy * dy);

		float nomalizedx = dx / sizeofvector;
		float nomalizedy = dy / sizeofvector;

		POINTFLOAT ptdraw = {
			mptpos.x + nomalizedx * PLAYER_SHOOT_LENGTH,
			mptpos.y + nomalizedy * PLAYER_SHOOT_LENGTH
		};

		HPEN hOld = (HPEN)SelectObject(hdc, hPLAYERSHOOTPEN);
		MoveToEx(hdc, mptpos.x, mptpos.y, nullptr);
		LineTo(hdc, ptdraw.x, ptdraw.y);
		SelectObject(hdc, hOld);
	}

	if (effecttime_Shoot) {
		for (int i = 0; i < 7; i++) {
		
			FillRect(hdc, &shootattackrange[i], hPLAYERSHOOTRECTBRUSH);
		}

		HBRUSH hOld = (HBRUSH)SelectObject(hdc, hPLAYERSHOOTCIRBRUSH);

		// 이펙트타임 25
		float tmp1 = effecttime_Shoot / FRAMETIME;

		switch (effecttime_Shoot) {
		case FRAMETIME*25:
		case FRAMETIME*24:
		case FRAMETIME*23:
			Ellipse(hdc, shootattackrange[4].left - 10, shootattackrange[4].bottom - 10,
				shootattackrange[4].left + 10, shootattackrange[4].bottom + 10);

			break;
		case FRAMETIME*22:
		case FRAMETIME*21:
			Ellipse(hdc, shootattackrange[4].left - 10, shootattackrange[4].top - 10,
				shootattackrange[4].left + 10, shootattackrange[4].top + 10);
			break;

		case FRAMETIME*20:
		case FRAMETIME*19:
			Ellipse(hdc, shootattackrange[4].right - 10, shootattackrange[4].top - 10,
				shootattackrange[4].right + 10, shootattackrange[4].top + 10);

			break;
		case FRAMETIME*18:
		case FRAMETIME*17:
			Ellipse(hdc, shootattackrange[4].right - 10, shootattackrange[4].bottom - 10,
				shootattackrange[4].right + 10, shootattackrange[4].bottom + 10);

			break;
		case FRAMETIME*16:
		case FRAMETIME*15:
			Ellipse(hdc, shootattackrange[5].left - 10, shootattackrange[5].bottom - 10,
				shootattackrange[5].left + 10, shootattackrange[5].bottom + 10);

			break;
		case FRAMETIME*14:
		case FRAMETIME*13:
			Ellipse(hdc, shootattackrange[5].left - 10, shootattackrange[5].top - 10,
				shootattackrange[5].left + 10, shootattackrange[5].top + 10);

			break;
		case FRAMETIME*12:
		case FRAMETIME*11:
			Ellipse(hdc, shootattackrange[5].right - 10, shootattackrange[5].top - 10,
				shootattackrange[5].right + 10, shootattackrange[5].top + 10);

			break;
		case FRAMETIME*10:
		case FRAMETIME*9:
			Ellipse(hdc, shootattackrange[5].right - 10, shootattackrange[5].bottom - 10,
				shootattackrange[5].right + 10, shootattackrange[5].bottom + 10);

			break;
		case FRAMETIME*8:
		case FRAMETIME*7:
			Ellipse(hdc, shootattackrange[6].left - 10, shootattackrange[6].bottom - 10,
				shootattackrange[6].left + 10, shootattackrange[6].bottom + 10);
			break;
		case FRAMETIME*6:
		case FRAMETIME*5:
			Ellipse(hdc, shootattackrange[6].left - 10, shootattackrange[6].top - 10,
				shootattackrange[6].left + 10, shootattackrange[6].top + 10);
			break;

		case FRAMETIME*4:
		case FRAMETIME*3:
			Ellipse(hdc, shootattackrange[6].right - 10, shootattackrange[6].top - 10,
				shootattackrange[6].right + 10, shootattackrange[6].top + 10);
			break;
		case FRAMETIME*2:
		case FRAMETIME*1:
			Ellipse(hdc, shootattackrange[6].right - 10, shootattackrange[6].bottom - 10,
				shootattackrange[6].right + 10, shootattackrange[6].bottom + 10);

			break;

		}
		

		SelectObject(hdc, hOld);
	}

	if (effecttime_AoE) {
		// 광역기 이펙트 그리기

		POINT iAoe1[8] = {
		{mrcRng.left, mrcRng.top},
		{mptpos.x,mptpos.y - iAoERadius},
		{mrcRng.right, mrcRng.top},
		{mptpos.x + iAoERadius,mptpos.y},
		{mrcRng.right, mrcRng.bottom},
		{mptpos.x , mptpos.y + iAoERadius},
		{mrcRng.left, mrcRng.bottom},
		{mptpos.x - iAoERadius,mptpos.y}
		};

		float prdcos45 = PLAYER_RADIUS * 1.4;
		float circos45 = iAoERadius * 1.4;

		POINT iAoe2[8] = {
			{(LONG)(mptpos.x),				(LONG)(mptpos.y - prdcos45)		},
			{(LONG)(mptpos.x + circos45),	(LONG)(mptpos.y - circos45)		},
			{(LONG)(mptpos.x + prdcos45),	(LONG)(mptpos.y)				},
			{(LONG)(mptpos.x + circos45),	(LONG)(mptpos.y + circos45)		},
			{(LONG)(mptpos.x),				(LONG)(mptpos.y + prdcos45)		},
			{(LONG)(mptpos.x - circos45),	(LONG)(mptpos.y + circos45)		},
			{(LONG)(mptpos.x - prdcos45),	(LONG)(mptpos.y)				},
			{(LONG)(mptpos.x - circos45),	(LONG)(mptpos.y - circos45)		}
		};

			

		HBRUSH hOld = (HBRUSH)SelectObject(hdc, hIAOEBRUSH2);
		Ellipse(hdc, mptpos.x - iAoERadius, mptpos.y - iAoERadius, 
			mptpos.x + iAoERadius, mptpos.y + iAoERadius);

		SelectObject(hdc, hIAOEBRUSH);
		//Polygon(hdc, iAoe, 8);
		Polygon(hdc, iAoe1, 8);
		Polygon(hdc, iAoe2, 8);
		SelectObject(hdc, hOld);
	}

	if (onshield) {

		
		HBRUSH hOld = (HBRUSH)SelectObject(hdc, hSDBRUSH);

		Ellipse(hdc, mptpos.x - SHIELD_RAD, mptpos.y - SHIELD_RAD,
			mptpos.x + SHIELD_RAD, mptpos.y + SHIELD_RAD);

		SelectObject(hdc, hOld);

	}


	if (pressQ) {
		// 파란색 바 그리기
		RECT rcrtbarRng = { mrcRng.left - 10, mrcRng.bottom + 2,
			mrcRng.right + 10, mrcRng.bottom + 7 };
		FillRect(hdc, &rcrtbarRng, hRTBRUSH);

		// 검은 바 그리기
		INT width = rcrtbarRng.right - rcrtbarRng.left;
		INT crt = castingtime_return / FRAMETIME;
		INT max = CASTINGTIME_RETURN / FRAMETIME;
		INT leftposX = rcrtbarRng.right - ((crt * width) / max);
		RECT rcemptybar = {
			leftposX,
			rcrtbarRng.top + 1,
			rcrtbarRng.right,
			rcrtbarRng.bottom - 1
		};
		FillRect(hdc, &rcemptybar, (HBRUSH)GetStockObject(BLACK_BRUSH));

	};

	if (effecttime_Return) {
		// 귀환 이펙트 그리기
		mrcRng = { (LONG)(mptpos.x - PLAYER_CENTER2VERTAX),
		(LONG)(mptpos.y - PLAYER_CENTER2VERTAX),
		(LONG)(mptpos.x + PLAYER_CENTER2VERTAX),
		(LONG)(mptpos.y + PLAYER_CENTER2VERTAX)
		};
	// 먼저 그리는 삼각형
		
		HBRUSH hOld = (HBRUSH)SelectObject(hdc,rtHbrush);
		Ellipse(hdc, mrcRng.left, mrcRng.top, mrcRng.right, mrcRng.bottom);
		SelectObject(hdc,hOld);


		triangle1[0] = { (LONG)mptpos.x, (LONG)(mptpos.y + PLAYER_CENTER2VERTAX) };
		triangle1[1] = { (LONG)(mptpos.x - PLAYER_HALFSIDE),(LONG)(mptpos.y - PLAYER_CENTER2SIDE) };
		triangle1[2] = { (LONG)(mptpos.x + PLAYER_HALFSIDE),(LONG)(mptpos.y - PLAYER_CENTER2SIDE) };
		// 나중에 그리는 삼각형
		triangle2[0] = { (LONG)mptpos.x, (LONG)(mptpos.y - PLAYER_CENTER2VERTAX) };
		triangle2[1] = { (LONG)(mptpos.x - PLAYER_HALFSIDE),(LONG)(mptpos.y + PLAYER_CENTER2SIDE) };
		triangle2[2] = { (LONG)(mptpos.x + PLAYER_HALFSIDE),(LONG)(mptpos.y + PLAYER_CENTER2SIDE) };

		hOld = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));

		Polygon(hdc, triangle1, 3);
		Polygon(hdc, triangle2, 3);

		SelectObject(hdc, hOld);
		
		
	}
	HBRUSH hOld = (HBRUSH)SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	FLOAT TriHeight = PLAYER_RADIUS / 2 * sqrt(3);
	POINT pt[6];
	pt[0] = { (LONG)mptpos.x - PLAYER_RADIUS, (LONG)mptpos.y };
	pt[1] = { (LONG)mptpos.x - (PLAYER_RADIUS / 2), (LONG)(mptpos.y - TriHeight) };
	pt[2] = { (LONG)mptpos.x + (PLAYER_RADIUS / 2), (LONG)(mptpos.y - TriHeight) };
	pt[3] = { (LONG)mptpos.x + PLAYER_RADIUS, (LONG)mptpos.y };
	pt[4] = { (LONG)mptpos.x + (PLAYER_RADIUS / 2), (LONG)(mptpos.y + TriHeight) };
	pt[5] = { (LONG)mptpos.x - (PLAYER_RADIUS / 2), (LONG)(mptpos.y + TriHeight) };


	Polygon(hdc, pt, 6);
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));

	Ellipse(hdc, mptpos.x - PLAY_ELLIPSE_RAD, mptpos.y - PLAY_ELLIPSE_RAD,
		mptpos.x + PLAY_ELLIPSE_RAD, mptpos.y + PLAY_ELLIPSE_RAD);
	SelectObject(hdc, hOld);

	// hpbar 
	mrchpbar.left = mrcRng.left - 7;
	mrchpbar.top = mrcRng.bottom - GETHPBAR(mhp->GetHp(), PLAYER_RADIUS * 2, PLAYER_MAXHP);
	mrchpbar.right = mrcRng.left - 4;
	mrchpbar.bottom = mrcRng.bottom;



	CGameObject* tmp = nullptr;
	while (tmp != menemylist) {
		if (!tmp) tmp = menemylist;

		if (tmp->IsDead()) {
			tmp = tmp->next;
			continue;
		}

		float dx = mptpos.x - tmp->GetPos().x;
		float dy = mptpos.y - tmp->GetPos().y;

		float center_d = dx * dx + dy * dy;
		float range = PLAYER_ATTACK_RANGE + tmp->GetObjRadius();
		if (center_d <= range * range) {
			tmp->SelectedDraw(hdc, hINRANGEBRUSH);
		}

		tmp = tmp->next;
	}


	// 총알
	if (pbullet) {
		pbullet->Draw(hdc);
	}
	if (ptarget) {

		if (ptarget->IsDead()) ptarget = nullptr;			// 다른 유닛에 의해 죽었을 때
		else ptarget->SelectedDraw(hdc, hSELECTEDBRUSH);

	}
}


void CPlayer::Update()
{

	//// 죽음
	//if (ideatheffecttime) {
	//	if (ideatheffecttime == PLAYER_EFFECTTIME_DEATH) {
	//	}

	//	ideatheffecttime -= FRAMETIME;
	//	if (!ideatheffecttime) {
	//		SetPos(PLAYER_DEFAULT_POSITION);
	//		mhp->SetHp(PLAYER_MAXHP);
	//		mdeath = FALSE;
	//	}
	//	return;	// 죽었으면 업데이트 멈추기
	//}


	// 플레이어 움직임
	Move();

	//// 기지에 있으면 체력 회복
	//if (mrcRng.left <= 650) {
	//	mhp->AddHp(RECOVERAMOUNT);
	//}


	//if (effecttime_Shoot) effecttime_Shoot -= FRAMETIME;
	//if (effecttime_AoE) effecttime_AoE -= FRAMETIME;

	//if (effecttime_Return) {
	//	effecttime_Return -= FRAMETIME;
	//	if (!effecttime_Return)
	//	{
	//		SetPos(PLAYER_DEFAULT_POSITION);
	//	}
	//}

	//if (pressQ) {
	//	if (castingtime_return == 0)
	//	{
	//		cooltime_Return = COOLTIME_RETURN;
	//		effecttime_Return = FRAMETIME * 50;
	//		msound->MyPlaySound(3, 3);
	//		pressQ = FALSE;
	//	}
	//	castingtime_return -= FRAMETIME;
	//}

	//if (onshield) {
	//	if (activetime_shield == 0) {
	//		//msound->MyPlaySound(9, 3);
	//		onshield = FALSE;
	//	}
	//	activetime_shield -= FRAMETIME;
	//}


	//// 쿨타임이 0이 아닐 때 감소
	//if (cooltime_Shoot) {
	//	cooltime_Shoot -= FRAMETIME;
	//	if (!cooltime_Shoot) {
	//		msound->MyPlaySound(4, 3);
	//		// 쿨타임 끝~
	//	}
	//}
	//if (cooltime_AoE) {
	//	cooltime_AoE -= FRAMETIME;
	//	if (!cooltime_AoE) {
	//		msound->MyPlaySound(4, 3);
	//	}
	//}
	//if (cooltime_Shield)
	//{
	//	cooltime_Shield -= FRAMETIME;
	//	if (!cooltime_Shield) {
	//		msound->MyPlaySound(4, 3);
	//	}
	//}
	//if (cooltime_Return)
	//{
	//	cooltime_Return -= FRAMETIME;
	//	if (!cooltime_Return) {
	//		msound->MyPlaySound(4, 3);
	//	}
	//}
	//		

	//if (iattackcooltime)
	//	iattackcooltime -= FRAMETIME;
	//else {
	//	if (Attack())
	//		iattackcooltime = FRAMETIME * 50;
	//}

	//if (pbullet) pbullet = pbullet->Move();

	//if (ptarget) {
	//	if (ptarget->IsDead()) {		// 내 공격으로 죽었을 때
	//		ptarget = nullptr;
	//		return;
	//	}

	//	// 플레이어 공격 범위에서 벗어났을 때
	//	float dx = mptpos.x - ptarget->GetPos().x;
	//	float dy = mptpos.y - ptarget->GetPos().y;

	//	float center_d = dx * dx + dy * dy;
	//	float range = PLAYER_ATTACK_RANGE + ptarget->GetObjRadius();
	//	if (center_d > range * range) {
	//		ptarget = nullptr;
	//		return;
	//	}

	//}
	//return;
}

void CPlayer::Death()
{
	ideatheffecttime = PLAYER_EFFECTTIME_DEATH;
	msound->MyPlaySound(12, 3);
}
