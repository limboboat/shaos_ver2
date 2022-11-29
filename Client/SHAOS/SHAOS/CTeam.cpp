#include "pch.h"
#include "CTeam.h"
#include "CGameObject.h"

CTeam::CTeam()
{
	// Å¸¿ö 1°³
	imyobjnum = 1;
	iopnobjnum = 1;

	gameover = FALSE;
}


CTeam::~CTeam()
{

}

CGameObject* CTeam::GetMyObjList() const
{
	return p_myobjlist;
}

void CTeam::SetOpponentObj(CGameObject* objlist)
{
	p_opponentobjlist = objlist;
}

void CTeam::AddMyObjList(CGameObject* addobj)
{
	addobj->next = p_myobjlist;
	addobj->prev = p_myobjlist->prev;
	p_myobjlist->prev->next = addobj;
	p_myobjlist->prev = addobj;

	imyobjnum++;
}

void CTeam::DeleteInList(CGameObject* delobj)
{
	imyobjnum--;

	delobj->prev->next = delobj->next;
	delobj->next->prev = delobj->prev;
	delete delobj;
}

