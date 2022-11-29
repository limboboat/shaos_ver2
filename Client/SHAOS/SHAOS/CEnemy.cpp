#include "pch.h"
#include "CEnemy.h"
#include "CTurret.h"
#include "CTurret2.h"
#include "CRect.h"
#include "CEllip.h"
#include "CDia.h"
#include "CTri.h"

CEnemy::CEnemy()
{
	mTower = new CTower(ENEMYTOWER_POS, TEAM::ENEMY, nullptr);
	p_myobjlist = mTower;
	p_myobjlist->next = p_myobjlist->prev = p_myobjlist;

	iunitgentime = 0;
}


CEnemy::~CEnemy()
{
}

void CEnemy::SetInitObj()
{
	mTurret1 = new CTurret(TURRET1_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(mTurret1);

	mTurret2 = new CTurret(TURRET2_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(mTurret2);

	mTurret3 = new CTurret(TURRET3_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(mTurret3);

	mTurret4 = new CTurret(TURRET4_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(mTurret4);

}

void CEnemy::Update()
{
	if (gameover) return;

	if (p_myobjlist->IsDead()) {

		p_myobjlist->Update();

		if(p_myobjlist->IsDelete())
			gameover = TRUE;

		return;
	}

	// À¯´Ö Á¨
	if (iunitgentime) iunitgentime -= FRAMETIME;

	UnitGen();
	


	CGameObject* tmp = p_myobjlist;
	for (int i = 0; i < imyobjnum; i++) {

		tmp->Update();

		if (tmp->IsDelete()) {
			CGameObject* delp = tmp;
			tmp = tmp->next;
			DeleteInList(delp);
			continue;
		}

		tmp = tmp->next;
	}

}

void CEnemy::Draw(HDC hdc)
{
	CGameObject* tmp = p_myobjlist;
	for (int i = 0; i < imyobjnum; i++) {
		tmp->Draw(hdc);
		tmp->DrawHP(hdc);
		tmp = tmp->next;
	}
}

void CEnemy::UnitGen()
{
	int deadtowercount = 0;

	if (mTurret1->IsDead()) deadtowercount++;
	if (mTurret2->IsDead()) deadtowercount++;
	if (mTurret3->IsDead()) deadtowercount++;
	if (mTurret4->IsDead()) deadtowercount++;

	if (!iunitgentime) {
		switch (deadtowercount) {
		case 0:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 2000;
			break;
		}
		case 1:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 1750;
			break;
		}

		case 2:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 1500;
			break;
		}
		case 3:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

			iunitgentime = FRAMETIME * 1250;
			break;
		}
		case 4:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
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
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1700) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1600) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}

		break;
	}
	case 1:
	{
		if (iunitgentime == FRAMETIME * 1600) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1400) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1300) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}



		break;
	}
	case 2:
	{
		if (iunitgentime == FRAMETIME * 1400) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1300) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1200) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1100) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CTri(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}


		break;
	}
	case 3:
	{
		if (iunitgentime == FRAMETIME * 1100) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 700) {
			CUnit* unit1 = new CTri(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 600) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}

		break;
	}
	case 4:
	{
		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 700) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 600) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 500) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 400) {
			CUnit* unit1 = new CTri(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}
		if (iunitgentime == FRAMETIME * 300) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(unit2);

		}


		break;
	}
	}


}
