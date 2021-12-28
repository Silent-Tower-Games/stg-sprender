#pragma once

#include <stdint.h>

typedef struct Sprender_Vertex
{
	float x, y;
	float u, v;
	uint32_t color;
} Sprender_Vertex;
