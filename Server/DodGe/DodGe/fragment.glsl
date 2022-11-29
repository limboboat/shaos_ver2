#version 330 core

out vec4 FragColor;			//--- 최종 객체의 색 저장

uniform vec3 vColor;

void main()
{
	FragColor = vec4 (vColor, 1.0);									//--- 픽셀 색을 출력
}