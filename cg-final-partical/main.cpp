#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>

#include "Application.h"

/****************************************************************
主题：
1. 鱼群
2. 樱花
=================================================================
优化：
使用vbo, glsl等底层管线改善性能
=================================================================
问题：
[v] glew.h
	SysWOW64 instead of System32
[v] exit重定义
	include iostream before GL
[v] 键盘延迟
	GetKeyState()
=================================================================
TODO:
细分模块
拆分h->h+cpp
支持滚轮？
*****************************************************************/

#define KEY_EXIT 27

Application app;
bool firstMouse = true;
int lastX, lastY;

void display(void)
{
	app.run();
}

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
	glutInitWindowSize(600, 600);
	glutCreateWindow("Alpha Test");

	std::cout << "Loading application..." << std::endl;
	app.init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardFunc);
	glutPassiveMotionFunc(motionFunc);

	glutMainLoop();

	return 0;
}