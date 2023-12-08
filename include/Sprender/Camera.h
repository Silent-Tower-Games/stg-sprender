#pragma once

#include <Sprender/Float2D.h>
#include <Sprender/Int2D.h>

typedef struct Sprender_Camera
{
	Sprender_Int2D resolution;
	Sprender_Float2D position;
	Sprender_Float2D origin;
	Sprender_Float2D zoom;
	float rotation;
	float projection[16];
} Sprender_Camera;

/**
 * @brief Create a camera object with the given resolution & zoom.
 * 
 * @param resolution size of the camera's viewing rectangle
 * @param zoom zoom intensity X and Y. can zoom independently on each axis
 * @return Sprender_Camera completed camera object
 */
Sprender_Camera Sprender_Camera_Create(Sprender_Int2D resolution, Sprender_Float2D zoom);

/**
 * @brief Load your camera object's matrix into a given pointer.
 * 
 * @param camera camera object
 * @param dest matrix pointer
 */
void Sprender_Camera_LoadInto(Sprender_Camera* camera, void* dest);
