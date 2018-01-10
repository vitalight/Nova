#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>

#include "Application.h"

/*******************************************************
*	This enables selecting high performance graphics card,
* However for unknown reason, the strange thing is when 
* things start to be moving, the build-in GC have better 
* performance. 
********************************************************/
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

Application app;
bool firstMouse = true;
bool polygonMode = false;
int lastX, lastY;

/*******************************************************
* [display]
*	This function is called per frame, update images by
* invoking Application class.
********************************************************/
void display(void)
{
	glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	app.Update();
	glutSwapBuffers();
}

/*****************************************************
* [keyboardFunc]
*	Process input from keyboard.
*	Note that 'wasd' key is not handled here because
* it will have unacceptable latency 
******************************************************/
void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case NV_EXIT_ASCII:
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
		app.SwitchShader();
		break;
	case '3':
		app.SwitchPartical();
		break;
	}
}

/*****************************************************
* [motionFunc]
*	Process input from mouse movement
******************************************************/
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

	app.processMouse(xoffset, yoffset);
}

int main(int argc, char *argv[])
{
	// main window setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(200, 70);
	glutInitWindowSize(NV_SCR_WIDTH, NV_SCR_HEIGHT);
	glutCreateWindow("Nova");

	// enable supported extensions from graphics driver to be exposed
	glewExperimental = GL_TRUE;
	glewInit();

	// glew initially will has a error, use this to clear it
	glGetError();

	// enable relative functions
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//查看系统是否支持多重采样
	//static GLint buf[1], sbuf[1];
	//glGetIntegerv(GL_SAMPLE_BUFFERS_ARB, buf);
	//printf("number of sample buffers is %d\n", buf[0]);
	//glGetIntegerv(GL_SAMPLES_ARB, sbuf);
	//printf("number of samples is %d\n", sbuf[0]);
	//glEnable(GL_MULTISAMPLE_ARB);
	//glEnable(GL_MULTISAMPLE);

	std::cout << glGetString(GL_RENDERER) << endl;
	std::cout << "Starting application..." << std::endl;
	app.Init();

	// link each condition to corresponding functions
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardFunc);
	glutPassiveMotionFunc(motionFunc);

	glutMainLoop();

	return 0;
}