#include "Timer.h"

int TimerValue = 10;

using namespace chrono;

GLvoid TimerFunction(int value)
{
	static system_clock::time_point start = system_clock::now();
	static system_clock::time_point end = system_clock::now();

	if (Player.isDead()) {
		end = system_clock::now();
		auto time = duration_cast<milliseconds>(end - start);
		stringstream str;
		str << "시간 : " << time.count() / 1000. << "초" << endl << "Game Over\n다시 시작하시겠습니까?" << endl;
		if (IDYES == MessageBoxA(NULL, str.str().c_str(), "Game Over", MB_YESNO | MB_ICONINFORMATION)) {
			Player.Reset();
			start = chrono::system_clock::now();
		}
		else
			glutLeaveMainLoop();
	}

	for (CUnit& unit : Unit) {
		unit.Update();
	}
	Player.Update();

	static int wait = 0;
	for (CUnit& unit : Unit) {
		if (isCollide(unit, Player) && wait == 0) {
			++wait;
			Player.Damage();
			Player.showHP();
		}
	}

	if (wait != 0) {
		++wait;
		if (wait == 100)
			wait = 0;
	}

	drawHP.Update();

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}

bool isCollide(CUnit r1, CPlayer r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getTop() < r2.getBottom() || r1.getBottom() > r2.getTop()) return false;
	return true;
}