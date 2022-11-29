#pragma once
#include "CHp.h"

class CGameObject
{
protected:
	TEAM team;

	POINTFLOAT		mptpos;
	RECT			mrcRng;

	CHp*			mhp{ nullptr };
	RECT			mrchpbar;
	BOOL			mdeath;

	CGameObject* menemylist{ nullptr };


	INT iattackcooltime;
	INT ideatheffecttime;

public:
	CGameObject* next{ nullptr };
	CGameObject* prev{ nullptr };

	CGameObject(POINTFLOAT ainitPos, TEAM ateam ,CGameObject* aenemylist);
	virtual ~CGameObject();

	virtual void Draw(HDC) = 0;
	virtual void Update() = 0;
	void DrawHP(HDC);

	virtual void SelectedDraw(HDC, HBRUSH) {};


	POINTFLOAT		GetPos() const;
	RECT			GetRng(); // ? 필요할까?
	
	virtual INT		GetObjRadius() = 0;

	virtual void	PutDamage(INT damage);

	virtual void	Death() = 0;
	BOOL			IsDead() const;
	BOOL			IsDelete() const;
};

BOOL IsInRange(CGameObject* me, CGameObject* obj, INT range);
