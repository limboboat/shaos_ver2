#pragma once
#include "CGameObject.h"



class Bullet;
class CTurret : public CGameObject
{
	CGameObject* ptarget;
	Bullet* pbullet;
	CSound* msound;
public:
	CTurret(POINTFLOAT initPos, TEAM team, CGameObject* enemylist);
	~CTurret();
	virtual void Draw(HDC hdc);
	virtual void Update();

	CGameObject* FindTarget();
	void Attack();

	virtual void SelectedDraw(HDC, HBRUSH);

	virtual INT		GetObjRadius();
	virtual void	Death();

	void GetSound(CSound* sound) {
		msound = sound;
	}
};

