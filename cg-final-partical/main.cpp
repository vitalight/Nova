#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>

#include "Application.h"

/****************************************************************
���⣺
1. ��Ⱥ
2. ӣ��
=================================================================
�Ż���
ʹ��vbo, glsl��core-profile��������
Mipmap
=================================================================
���⣺
[v] glew.h
	SysWOW64 instead of System32
[v] exit�ض���
	include iostream before GL
[v] �����ӳ�
	GetKeyState()
[v] obj�ض���
	call�Ͷ���Ĳ������Ͳ�ͬ(��ʽ����)
[v] gl����
	glewInit()!
[v] ����ʾ
	clearDepthBufferBit
=================================================================
TODO:
ʵ��������������������������Ч��
ϸ��ģ��
���h->h+cpp
֧�ֹ��֣�
glEnable(GL_CULL_FACE); ���޳�
��������ͼ ��պ�
*****************************************************************/

#define KEY_EXIT 27

Application app;
bool firstMouse = true;
int lastX, lastY;

void display(void)
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	app.Update();
	glutSwapBuffers();
}

// this function doesn't handle movement(like wasd)
// because it can't complete per frame call
void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_EXIT:
		exit(0);
	}
}

void motionFunc(int x, int y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	int xoffset = x - lastX;
	int yoffset = lastY - y;// reversed since y-coordinates go from bottom to top

	lastX = x;
	lastY = y;

	app.camera.ProcessMouseMovement(xoffset, yoffset);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(A_SCR_WIDTH, A_SCR_HEIGHT);
	glutCreateWindow("Day 0");

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::cout << "Loading application..." << std::endl;
	app.Init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardFunc);
	glutPassiveMotionFunc(motionFunc);

	glutMainLoop();

	return 0;
}