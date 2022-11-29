#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <random>
#include <chrono>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <gl/glm/glm.hpp>
//#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>

#include <glm/ext.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

extern int TimerValue;
extern GLint winWidth, winHeight;
extern GLuint VAO;
extern GLuint shaderID; //--- ���̴� ���α׷� �̸�

extern bool isW, isA, isS, isD;

#include "CUnit.h"
class CUnit;
extern CUnit Unit[75];

#include "CPlayer.h"
class CPlayer;
extern CPlayer Player;

#include "CdrawHP.h"
class CdrawHP;
extern CdrawHP drawHP;
