#include "Draw.h"

CUnit Unit[75];
CPlayer Player;
CdrawHP drawHP(Player);

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//��ο�

	for (CUnit& unit : Unit) {
		unit.Draw();
	}
	Player.Draw();
	drawHP.Draw();

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}
