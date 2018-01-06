#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>

#include "Application.h"


// this enables selecting high performance graphics card
//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}

/****************************************************************
NOVA
=================================================================
优化：
使用vbo, glsl等core-profile改善性能
Mipmap
=================================================================
问题：
[v] glew.h
	SysWOW64 instead of System32
[v] exit重定义
	include iostream before GL
[v] 键盘延迟
	GetKeyState()
[v] obj重定义
	call和定义的参数类型不同(隐式声明)
[v] gl卡死
	glewInit()!
[v] 不显示
	clearDepthBufferBit
[v] assimp读取材料色
	很辛苦的看document
[v] model load in error
	全部合并后导出obj
	或者.x格式！
	fbx better!
[v] singleton
	static both in .h and .cpp
[v] 光照纹理贴图
=================================================================
TODO:
支持滚轮
立方体贴图 天空盒
减少状态切换
load model miss 不报错
*****************************************************************/

#define KEY_EXIT 27

Application app;
bool firstMouse = true;
bool polygonMode = false;
int lastX, lastY;

void display(void)
{
	glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
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
			glPolygonMode(GL_FRONT, GL_FILL);
			polygonMode = false;
		}
		else {
			glPolygonMode(GL_FRONT, GL_LINE);
			polygonMode = true;
		}
		break;
	case '2':
		glEnable(GL_MULTISAMPLE);
		break;
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

	app.camera->ProcessMouseMovement(xoffset, yoffset);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
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

	//glEnable(GL_MULTISAMPLE_ARB);
	//glEnable(GL_MULTISAMPLE);
	printf("%s\n", glGetString(GL_RENDERER));
	/*查看系统是否支持多重采样*/
	/*static GLint buf[1], sbuf[1];
	glGetIntegerv(GL_SAMPLE_BUFFERS_ARB, buf);
	printf("number of sample buffers is %d\n", buf[0]);
	glGetIntegerv(GL_SAMPLES_ARB, sbuf);
	printf("number of samples is %d\n", sbuf[0]);*/
	
	std::cout << "Starting application..." << std::endl;
	app.Init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardFunc);
	glutPassiveMotionFunc(motionFunc);

	glutMainLoop();

	return 0;
}