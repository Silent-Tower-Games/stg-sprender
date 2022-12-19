#pragma once

#include <Sprender/Camera.h>

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

/**
 * @brief Create an empty matrix.
 * 
 * @return Sprender_Matrix empty matrix
 */
Sprender_Matrix Sprender_Matrix_Create();

/**
 * @brief Turn your matrix into a transform (e.g. float array).
 * 
 * @param matrix your matrix
 * @param transform float array pointer
 */
void Sprender_Matrix_ToTransform(Sprender_Matrix* matrix, float* transform);

/**
 * @brief Create a matrix from a camera object.
 * 
 * @param camera camera object
 * @return Sprender_Matrix camera matrix
 */
Sprender_Matrix Sprender_Matrix_CreateFromCamera(Sprender_Camera* camera);
