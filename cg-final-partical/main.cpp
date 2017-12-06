#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>

#include "Application.h"

/****************************************************************
NOVA
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
[v] assimp��ȡ����ɫ
	������Ŀ�document
[v] model load in error
	ȫ���ϲ��󵼳�obj
	����.x��ʽ��
=================================================================
TODO:
ϸ��ģ��
���h->h+cpp
֧�ֹ���
��������ͼ ��պ�
����״̬�л�
ResourceManager ��������
*****************************************************************/

#define KEY_EXIT 27

Application app;
bool firstMouse = true;
bool polygonMode = false;
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
	case '1':
		if (polygonMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			polygonMode = false;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			polygonMode = true;
		}
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