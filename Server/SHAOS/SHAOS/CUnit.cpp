#include "pch.h"
#include "CUnit.h"


CUnit::CUnit(POINTFLOAT ainitPos, TEAM team, CGameObject* enemylist)
	: CGameObject(ainitPos, team, enemylist)
{
	moveOn = TRUE;
	ptenemytowerpos = menemylist->GetPos();
}


CUnit::~CUnit()
{
}

void CUnit::Draw(HDC hdc)
{
}

void CUnit::Death()
{
	ideatheffecttime = UNIT_EFFECTTIME_DEATH;
}
