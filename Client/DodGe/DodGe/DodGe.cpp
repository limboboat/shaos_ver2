#include "Header.h"
#include "Draw.h"
#include "Keyboard.h"
#include "KeyboardUP.h"
#include "Mouse.h"
#include "Timer.h"

#include "CNetwork.h"

GLuint make_vertexShaders();
GLuint make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();

GLvoid Reshape(int w, int h);

char* filetobuf(string file);

// 전역변수
GLint winWidth = 800, winHeight = 800;		//--- 생성 창 크기
GLuint VAO, VBO;
GLuint shaderID; //--- 세이더 프로그램 이름

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	CNetwork Network;
	Network.ClientToServerComm();
	
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("DodGe");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	//도형 쉐이더 생성 및 버퍼 생성
	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glUseProgram(shaderID);
	InitBuffer();									// VAO, VBO 생성

	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUP);
	glutSpecialFunc(SpecialKeyboard);
	glutSpecialUpFunc(SpecialKeyboardUP);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
	glutTimerFunc(TimerValue, TimerFunction, 1);
	glutMainLoop();
	Network.Close();
}

GLuint make_vertexShaders()
{
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	GLchar* vertexSource = filetobuf("vertex.glsl");
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);		//--- 버텍스 세이더 객체
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
	else
		return vertexShader;
}

//--- 프래그먼트 세이더 객체 만들기
GLuint make_fragmentShaders()
{
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	GLchar* fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);		//--- 프래그먼트 세이더 객체
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
	else
		return fragmentShader;
}

//--- 세이더 프로그램 만들기
GLuint make_shaderProgram()
{
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	GLuint vertexShader = make_vertexShaders(); //--- 버텍스 세이더 만들기
	GLuint fragmentShader = make_fragmentShaders(); //--- 프래그먼트 세이더 만들기

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID);			//--- 만들어진 세이더 프로그램 사용하기
											//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
											//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
											//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;
}

void InitBuffer()					// 도형 버퍼 생성
{
	float vertex[4][3]{
		{-0.1, 0.1, 0},
		{-0.1, -0.1, 0},
		{0.1, -0.1, 0},
		{0.1, 0.1, 0}
	};


	//--- VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//// 정점, 색상 접근 규칙 만들기
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)

}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
}

char* filetobuf(string file) {
	ifstream in(file);

	string* str_buf = new string;
	while (in) {
		string temp;
		getline(in, temp);
		str_buf->append(temp);
		str_buf->append("\n");
	}

	return const_cast<char*>(str_buf->c_str());
}
