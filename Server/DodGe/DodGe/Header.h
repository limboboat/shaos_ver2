#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <random>
#include <chrono>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
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
extern GLuint shaderID; //--- 세이더 프로그램 이름

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
