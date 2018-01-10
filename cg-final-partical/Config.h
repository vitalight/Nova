#pragma once

/****************************************
 * Main
 ****************************************/
// screen width
#define NV_SCR_WIDTH 1200
// screen height
#define NV_SCR_HEIGHT 700
// ascii number for keyboard-exit
#define NV_EXIT_ASCII 27

/****************************************
* Camera
****************************************/
#define NV_CAMERA_SPEED (600.0f)
#define NV_CAMERA_FRONT_DISTANCE (35.0f)
#define NV_CAMERA_DOWN_DISTANCE (5.0f)

/****************************************
 * Light
 ****************************************/
// light source position
#define NV_LIGHT_POS (glm::vec3(0, 0, 0))
// light source color
#define NV_LIGHT_COL (glm::vec3(1.0f, 0.8f, 1.0f))
// light source bias against (ambient, diffuse, specular)
#define NV_LIGHT_BIAS (glm::vec3(0.25f, 0.8f, 0.45f))
// the speed that light brightness change
#define NV_LIGHT_ADJUST (1.0f)

/****************************************
 * Planet
 ****************************************/
// weight for planet rotation speed
#define NV_ROTATE_WEIGHT 0.2
// initial elapse time for entities // change to 3500
#define NV_ENTITY_INITIAL_FRAME 0

/****************************************
* Fire
****************************************/
#define NV_FIRE_SPEED 15.0f
#define NV_FIRE_RAND (float(rand()%6-3))
#define NV_FIRE_RAND_MAX 6.0f

/****************************************
 * Asteroids
 ****************************************/
// numbers of asteroids flying everywhere
#define NV_FLYING_NUM 200
// numbers of asteroids that circle around sun
// discrete video card can take up to 20000 particals with 50fps
#define NV_CIRCLING_NUM 1200

// radius of circle around sun
#define NV_ROCK_RADIUS 1200
// random position generation offset
#define NV_ROCK_OFFSET 60.0f

// range of destination position for flying asteroids
#define NV_CENTER_RANGE 1600
// range of generation position for flying asteroids
#define NV_FULL_RANGE 2400

// initial elapse time for flying asteroids
#define NV_INITIAL_FRAME 180.0f
// speed of flying asteroids 10.0f
#define NV_PARTICAL_SPEED 10.0f
// rotate speed of asteroids 
#define NV_ROTATE_SPEED 0.8f
// revolute speed of circling asteroids
#define NV_ANGLE_SPEED 0.02f

