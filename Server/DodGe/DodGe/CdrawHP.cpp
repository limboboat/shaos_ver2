#include "CdrawHP.h"

CdrawHP::CdrawHP(CPlayer& Player) : Unit(glm::mat4(1.f)), Player(Player), Change(glm::mat4(1.f))
{
	Update();
}

void CdrawHP::Update()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.3));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(-0.95, 0.95, 0));

	Change = Trans * Scale;
}

void CdrawHP::Draw()
{
	GLuint vColor = glGetUniformLocation(shaderID, "vColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(vColor, 1, 0, 0);


	for (int i = 0; i < Player.getHP(); ++i) {
		Update();
		Change = glm::translate(Unit, glm::vec3(i * 0.1f, 0, 0)) * Change;

		GLuint modelTransform = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(Change));

		glDrawArrays(GL_POLYGON, 0, 4);
	}
}
