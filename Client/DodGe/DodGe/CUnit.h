#pragma once
#include "Header.h"

class CUnit
{
	const glm::mat4 Unit;

	glm::vec3 Color;
	glm::vec2 Position;
	glm::vec2 Direction;

	glm::mat4 Change;

public:
	CUnit();
	
	void FixUpdate();
	void Update();
	void Draw();

	float getLeft();
	float getRight();
	float getBottom();
	float getTop();
};


