#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>

#include "Application.h"
// note that this can't be removed, otherwise error will be throwed
#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"

using namespace std;

/****************************************************************
主题：
1. 鱼群
2. 樱花
=================================================================
优化：
使用vbo底层管线改善性能
=================================================================
问题：
[v] glew.h
	SysWOW64 instead of System32
[v] exit重定义
	include iostream before GL
=================================================================
TODO:
细分模块
*****************************************************************/

Application *app;

void display(void)
{
	app->run();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Alpha Test");

	cout << "Loading application..." << endl;
	app = new Application();

	glutDisplayFunc(&display);
	glutIdleFunc(&display);
	glutMainLoop();

	return 0;
}