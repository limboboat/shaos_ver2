#pragma once
#include "Header.h"
#include "CPlayer.h"

class CdrawHP
{
	const glm::mat4 Unit;

	CPlayer& Player;

	glm::vec3 Color;

	glm::mat4 Change;

public:
	CdrawHP(CPlayer& Player);

	void Update();
	void Draw();
};

