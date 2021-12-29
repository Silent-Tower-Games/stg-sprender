#pragma once

#include "Float2D.h"
#include "Int2D.h"

typedef struct Sprender_Camera
{
	Sprender_Int2D resolution;
	Sprender_Float2D position;
	Sprender_Float2D zoom;
	float rotation;
	float projection[16];
} Sprender_Camera;

Sprender_Camera Sprender_Camera_Create(float resolutionX, float resolutionY, float zoomX, float zoomY);

void Sprender_Camera_LoadInto(Sprender_Camera* camera, void* dest);
