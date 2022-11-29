#pragma once
#include "CUnit.h"
#include "CTower.h"

class CGameObject;
class CTeam
{
protected:
	// 헤드는 항상 타워를 가리킨다
	CGameObject* p_myobjlist{ nullptr };
	CGameObject* p_opponentobjlist{ nullptr };


	INT imyobjnum;
	INT iopnobjnum;

	UINT iunitgentime;

public:
	BOOL gameover;


	CTeam();
	virtual ~CTeam();


	CGameObject* GetMyObjList() const;
	void SetOpponentObj(CGameObject* objlist);


	void AddMyObjList(CGameObject*);
	void DeleteInList(CGameObject*);


};

