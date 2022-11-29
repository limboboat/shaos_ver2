#include "pch.h"
#include "CUser.h"
#include "CPlayer.h"

#include "CRect.h"
#include "CEllip.h"

CUser::CUser()
{
	mTower = new CTower(USERTOWER_POS, TEAM::USER, nullptr);
	p_myobjlist = mTower;
	p_myobjlist->next = p_myobjlist->prev = p_myobjlist;

	iunitgentime = 0;
}

CUser::~CUser()
{
}

void CUser::SetInitObj()
{
	POINTFLOAT tmp = {
		PLAYER_DEFAULT_POSITION.x,
		PLAYER_DEFAULT_POSITION.y
	};
	mPlayer = new CPlayer(tmp, TEAM::USER, p_opponentobjlist);
	this->AddMyObjList(mPlayer);

}

void CUser::Update()
{
	if (gameover) return;

	if (p_myobjlist->IsDead()) {

		p_myobjlist->Update();

		if (p_myobjlist->IsDelete())
			gameover = TRUE;

		return;
	}

	// 유닛 젠
	if (iunitgentime) iunitgentime -= FRAMETIME;
	
	UnitGen();
	

	// 겜오브젝 업데이트
	CGameObject* tmp = p_myobjlist;
	for (int i = 0; i < imyobjnum; i++) {

		tmp->Update();
		if (p_myobjlist->IsDead()) return;

		if (tmp->IsDelete()) {

			CGameObject* delp = tmp;
			tmp = tmp->next;
			DeleteInList(delp);
			continue;
		}

		tmp = tmp->next;
	}

}

void CUser::MSG_Key(UINT message, WPARAM wParam)
{
	mPlayer->MSG_Key(message, wParam);
}

void CUser::MSG_Mouse(UINT message, POINT mousepos)
{
	// 상대편 obj 영역들을 검사해서 마우스 좌표에 있으면
	// 플레이어에게 신호를 보냄

	switch (message) {
	case WM_LBUTTONUP:
		mPlayer->MSG_MouseUp(mousepos);
		break;
	case WM_LBUTTONDOWN:
		mPlayer->MSG_MouseDown(mousepos);
		break;
	case WM_MOUSEMOVE:
		mPlayer->MSG_MouseMove(mousepos);
		break;

	}
}

void CUser::UnitGen()
{
	CGameObject* firsttower = p_opponentobjlist->next;

	int deadtowercount = 0;

	if (firsttower->IsDead()) deadtowercount++;
	if (firsttower->next->IsDead()) deadtowercount++;
	if (firsttower->next->next->IsDead()) deadtowercount++;
	if (firsttower->next->next->next->IsDead()) deadtowercount++;

	if (!iunitgentime) {
		switch (deadtowercount) {
		case 0:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 2000;
			break;
		}
		case 1:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 1750;
			break;
		}

		case 2:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 1500;
			break;
		}
		case 3:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 1250;
			break;
		}
		case 4:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 1000;
			break;
		}

		}
	}

	switch (deadtowercount) {
	case 0:
	{
		if (iunitgentime == FRAMETIME * 1800) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}

		break;
	}
	case 1:
		if (iunitgentime == FRAMETIME * 1600) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}

		break;
	case 2:
		if (iunitgentime == FRAMETIME * 1300) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}

		break;
	case 3:
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}

		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}
		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}

		break;
	case 4:
		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}

		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}
		if (iunitgentime == FRAMETIME * 600) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}
		if (iunitgentime == FRAMETIME * 500) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(unit2);
		}

		break;
	}


}

void CUser::Draw(HDC hdc)
{
	CGameObject* tmp = p_myobjlist;
	for (int i = 0; i < imyobjnum; i++) {
		tmp->Draw(hdc);
		tmp->DrawHP(hdc);
		tmp = tmp->next;
	}
}

POINTFLOAT CUser::GetPlayerPos()
{
	return mPlayer->GetPos();
}

void CUser::SetSound(CSound* sound)
{
	mPlayer->msound = sound;
}

void CUser::GetUIInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
	INT* ct_shield, INT* ct_return, INT* towerhp, INT* ct_death)
{
	mPlayer->UI_GetPlayerInfo(ahp, ct_shoot, ct_AoE, ct_shield, ct_return, ct_death);
	*towerhp = mTower->GetTowerHp();
}

INT CUser::GeteffecttimeAoE()
{
	return mPlayer->GetEffectTimeAoE();
}

void CUser::OffPlayerMove()
{
	mPlayer->OffPlayerMove();
}

