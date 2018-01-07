#pragma once

#include <vector>
#include <GL/GLUT.H>
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 300.0f;
const float SENSITIVITY = 0.5f;
const float ZOOM = 45.0f;

/***********************************************************
* [Camera]
*	This class processes input and calculates the 
*   corresponding Eular Angles, Vectors and Matrices
************************************************************/
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::mat4 Projection;
	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::mat4 projection,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter
	// in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Process input received from a mouse input system. Expects the offset value in both
	// the x and y direction.
	void ProcessMouseMovement(int xoffset, int yoffset, GLboolean constrainPitch = true);

	// Porcesses input received from a moouse scroll-wheel event. Only requires input
	// on the vertical wheel-axis
	void ProcessMouseScroll(int yoffset);
private:
	// Calculates the front vector from Camera's (updated) Eular Angles
	void updateCameraVectors();
};