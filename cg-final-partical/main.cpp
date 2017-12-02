#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>

#include "Application.h"
// note that this can't be removed, otherwise error will be throwed
#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"

using namespace std;

/****************************************************************
���⣺
1. ��Ⱥ
2. ӣ��
=================================================================
�Ż���
ʹ��vbo�ײ���߸�������
=================================================================
���⣺
[v] glew.h
	SysWOW64 instead of System32
[v] exit�ض���
	include iostream before GL
=================================================================
TODO:
ϸ��ģ��
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