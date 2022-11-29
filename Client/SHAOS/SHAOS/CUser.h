#pragma once
#include "CTeam.h"


class CPlayer;
class CUser : public CTeam
{
	// gameobjlist에 타워, 플레이어, 유닛 순으로 연결
	CTower* mTower;
	CPlayer* mPlayer;

	const POINTFLOAT ptUnitSponPos1 = { 500, 200 };
	const POINTFLOAT ptUnitSponPos2 = { 500, 500 };

public:
	CUser();
	~CUser();
	
	void SetInitObj();

	void Update();
	void Draw(HDC hdc);
	void MSG_Key(UINT message, WPARAM wParam);
	void MSG_Mouse(UINT message, POINT mousepos);

	void UnitGen();

	POINTFLOAT GetPlayerPos();
	void SetSound(CSound*);
	void GetUIInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
		INT* ct_shield, INT* ct_return, INT* towerhp, INT* ct_death);

	INT GeteffecttimeAoE();
	void OffPlayerMove();

};

