#pragma once
#include "Header.h"
class CPlayer
{
	const glm::mat4 Unit;

	glm::vec2 Position;

	glm::mat4 Change;
	
	int HP;

public:
	CPlayer();

	void Update();
	void Draw();

	float getLeft();
	float getRight();
	float getBottom();
	float getTop();

	void Damage();
	int getHP();
	void showHP();
	bool isDead();
	void Reset();
};

