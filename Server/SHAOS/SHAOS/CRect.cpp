#include "pch.h"
#include "CRect.h"
#include "CHp.h"

CRect::CRect(POINTFLOAT ainitPos, TEAM team, CGameObject* enemylist)
	: CUnit(ainitPos, team, enemylist)
{
	mrcRng = { (LONG)mptpos.x - RECT_RADIUS, (LONG)mptpos.y - RECT_RADIUS,
		(LONG)mptpos.x + RECT_RADIUS, (LONG)mptpos.y + RECT_RADIUS };

	mhp = new CHp(RECT_MAXHP);

	if (team == TEAM::USER) {
		mrchpbar = { mrcRng.left - 7, mrcRng.top, mrcRng.left - 4, mrcRng.bottom };
	}
	else {
		mrchpbar = { mrcRng.right + 4, mrcRng.top, mrcRng.right + 7, mrcRng.bottom };
	}

	iattackcooltime = FRAMETIME * 50;

	pattacktarget = menemylist;
	ideatheffecttime = 0;

}

CRect::~CRect()
{
	delete mhp;
}

void CRect::Draw(HDC hdc)
{
}

void CRect::SelectedDraw(HDC hdc, HBRUSH hbr)
{
}

void CRect::Update()
{
	// ����
	if (ideatheffecttime) {
		ideatheffecttime -= FRAMETIME;
		return;
	}

	// ������ ��� ���ϱ�
	SetTarget();
	if (pattacktarget->IsDead()) {
		pattacktarget = menemylist;
	}

	Move();

	if(iattackcooltime)
		iattackcooltime -= FRAMETIME;
	else Attack();
}

void CRect::Move()
{
	POINTFLOAT movevector;

	// Ÿ�ٰ� �Ÿ� ���ϱ�
	float projX = pattacktarget->GetPos().x - mptpos.x;
	float projY = pattacktarget->GetPos().y - mptpos.y;
	
	float distance = sqrt(projX * projX + projY * projY);
	INT stopdistance = pattacktarget->GetObjRadius() + iattakradius;

	// movevector ���ϴ� ��
	if (distance > stopdistance) {
		float nomalizedX = projX / distance;
		float nomalizedY = projY / distance;

		movevector = { nomalizedX,nomalizedY };
	}
	else movevector = { 0,0 };

	mptpos.x += movevector.x;
	mptpos.y += movevector.y;


	// ���� �̵�
	mrcRng = {
		(LONG)mptpos.x - RECT_RADIUS, (LONG)mptpos.y - RECT_RADIUS,
		(LONG)mptpos.x + RECT_RADIUS, (LONG)mptpos.y + RECT_RADIUS
	};

	//hp�� �̵�
	if (team == TEAM::USER) {
		mrchpbar = {
			mrcRng.left - 7,
			mrcRng.bottom - (INT)GETHPBAR(mhp->GetHp(), RECT_RADIUS * 2, RECT_MAXHP),
			mrcRng.left - 4,
			mrcRng.bottom
		};
	}
	else {
		mrchpbar = { mrcRng.right + 4,
			mrcRng.bottom - (INT)GETHPBAR(mhp->GetHp(), RECT_RADIUS * 2, RECT_MAXHP), mrcRng.right + 7, mrcRng.bottom };
	}
}

void CRect::Attack()
{
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
		float range = iattakradius + tmp->GetObjRadius();

		if (center_d <= range * range) {
			tmp->PutDamage(RECT_DAMAGE);
			if (tmp->IsDead()) {
				pattacktarget = menemylist;
			}
		}

		tmp = tmp->next;
	}

	iattackcooltime = FRAMETIME * 50;
}

void CRect::SetTarget()
{
	CGameObject* tmp = nullptr;
	while (tmp != menemylist) {
		if (!tmp) tmp = menemylist;

		//�׾����� Ÿ���� �� �� ����
		if (tmp->IsDead()) {
			tmp = tmp->next;
			continue;
		}

		INT range = UNIT_RECOGRNGRADIUS + RECT_RADIUS;
		if (IsInRange(this, tmp, range)) {
			pattacktarget = tmp;
		}

		tmp = tmp->next;
	}

}

INT CRect::GetObjRadius()
{
	return RECT_RADIUS;
}
