#pragma once

#include "Float2D.h"
#include "Int2D.h"

// TODO: Implement Camera rotation one day
typedef struct Sprender_Camera
{
	Sprender_Int2D resolution;
	Sprender_Float2D position;
	Sprender_Float2D zoom;
	float rotation;
	float projection[16];
} Sprender_Camera;

Sprender_Camera Sprender_Camera_Create(Sprender_Int2D resolution, Sprender_Float2D zoom);

void Sprender_Camera_LoadInto(Sprender_Camera* camera, void* dest);
