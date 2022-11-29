#include "Mouse.h"

GLvoid Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

	}

}

GLvoid Motion(int x, int y)
{

	//glutPostRedisplay();

}

GLvoid PassiveMotion(int x, int y)
{

	//glutPostRedisplay();
}
