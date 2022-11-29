#include "CPlayer.h"

CPlayer::CPlayer() : Unit(glm::mat4(1.f)), Position(glm::vec2(0)), Change(glm::mat4(1.f)), HP(10)
{
}

void CPlayer::Update()
{
	if (isW) {
		Position.y += 0.01f;
	}
	if (isA) {
		Position.x -= 0.01f;
	}
	if (isS) {
		Position.y -= 0.01f;
	}
	if (isD) {
		Position.x += 0.01f;
	}

	if (Position.x <= -0.98)
		Position.x = -0.98;
	else if (Position.x >= 0.98)
		Position.x = 0.98;
	if (Position.y <= -0.98)
		Position.y = -0.98;
	else if (Position.y >= 0.98)
		Position.y = 0.98;


	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.2));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(Position, 1.f));

	Change = Trans * Scale;
}

void CPlayer::Draw()
{
	GLuint vColor = glGetUniformLocation(shaderID, "vColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(vColor, 1.f, 1.f, 1.f);

	GLuint modelTransform = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(Change));


	glDrawArrays(GL_POLYGON, 0, 4);
}

float CPlayer::getLeft()
{
	return Position.x - 0.02f;
}

float CPlayer::getRight()
{
	return Position.x + 0.02f;
}

float CPlayer::getBottom()
{
	return Position.y - 0.02f;
}

float CPlayer::getTop()
{
	return Position.y + 0.02f;
}

void CPlayer::Damage()
{
	--HP;
}

int CPlayer::getHP()
{
	return HP;
}

void CPlayer::showHP()
{
	cout << "HP : " << HP << endl;
}

bool CPlayer::isDead()
{
	return 0 == HP;
}

void CPlayer::Reset()
{
	HP = 10;
}
