#pragma once

#include <stdint.h>

typedef struct Sprender_Vertex
{
	float x, y, z;
	float u, v;
	uint32_t color;
} Sprender_Vertex;
