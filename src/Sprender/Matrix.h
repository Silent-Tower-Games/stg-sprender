#pragma once

#include "Camera.h"

typedef struct Sprender_Matrix
{
	float
		M11,
		M12,
		M13,
		M14,
		M21,
		M22,
		M23,
		M24,
		M31,
		M32,
		M33,
		M34,
		M41,
		M42,
		M43,
		M44
	;
} Sprender_Matrix;

Sprender_Matrix Sprender_Matrix_Create();

void Sprender_Matrix_ToTransform(Sprender_Matrix* matrix, float* transform);

Sprender_Matrix Sprender_Matrix_CreateFromCamera(Sprender_Camera* camera);
