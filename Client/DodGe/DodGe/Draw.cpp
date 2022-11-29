#include "Draw.h"

CUnit Unit[75];
CPlayer Player;
CdrawHP drawHP(Player);

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//드로우

	for (CUnit& unit : Unit) {
		unit.Draw();
	}
	Player.Draw();
	drawHP.Draw();

	glutSwapBuffers();							// 화면에 출력하기
}
