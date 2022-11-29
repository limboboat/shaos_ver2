#include "KeyboardUP.h"

GLvoid KeyboardUP(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
	case 'W':
		isW = false;
		break;
	case 'a':
	case 'A':
		isA = false;
		break;
	case 's':
	case 'S':
		isS = false;
		break;
	case 'd':
	case 'D':
		isD = false;
		break;
	}
}

GLvoid SpecialKeyboardUP(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		isA = false;
		break;
	case GLUT_KEY_RIGHT:
		isD = false;
		break;
	case GLUT_KEY_UP:
		isW = false;
		break;
	case GLUT_KEY_DOWN:
		isS = false;
		break;
	}
}
