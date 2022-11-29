#include "CUnit.h"

CUnit::CUnit() : Unit(glm::mat4(1.f))
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> PnDurd{ -1.f, 1.f };
	uniform_real_distribution<float> Curd{ 0.f, 1.f };

	Position = glm::vec2{ PnDurd(dre), PnDurd(dre) };
	Color = glm::vec3{ Curd(dre),Curd(dre), Curd(dre) };
	Direction = glm::vec2{ PnDurd(dre),PnDurd(dre) };
	Direction = glm::vec2{ Direction.x / 150.,Direction.y / 150. };

	Update();
}

void CUnit::FixUpdate()
{
	if (Position.x + Direction.x < -1.f || Position.x + Direction.x > 1.f) {
		Direction.x *= -1.f;
	}
	if (Position.y + Direction.y < -1.f || Position.y + Direction.y > 1.f) {
		Direction.y *= -1.f;
	}
}

void CUnit::Update()
{
	FixUpdate();

	Position.x += Direction.x;
	Position.y += Direction.y;

	Change = glm::scale(Unit, glm::vec3(0.1f));
	
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(Position, 1.f));

	Change = Trans * Change;
}

void CUnit::Draw()
{
	GLuint vColor = glGetUniformLocation(shaderID, "vColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(vColor, Color.r, Color.g, Color.b);

	GLuint modelTransform = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(Change));


	glDrawArrays(GL_POLYGON, 0, 4);
}

float CUnit::getLeft()
{
	return Position.x - 0.01f;
}

float CUnit::getRight()
{
	return Position.x + 0.01f;
}

float CUnit::getBottom()
{
	return Position.y - 0.01f;
}

float CUnit::getTop()
{
	return Position.y + 0.01f;
}
