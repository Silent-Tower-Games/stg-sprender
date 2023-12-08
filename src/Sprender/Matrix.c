#include <assert.h>
#include <string.h>
#include "Matrix.h"

Sprender_Matrix Sprender_Matrix_Create()
{
    Sprender_Matrix matrix;
    memset(&matrix, 0, sizeof(matrix));
    
    matrix.M11 = 1;
    matrix.M22 = 1;
    matrix.M33 = 1;
    matrix.M44 = 1;
    
    return matrix;
}

void Sprender_Matrix_ToTransform(Sprender_Matrix* matrix, float* transform)
{
    assert(matrix != NULL);
    assert(transform != NULL);
    
    transform[0] = matrix->M11;
    transform[1] = matrix->M12;
    transform[2] = matrix->M13;
    transform[3] = matrix->M14;
    transform[4] = matrix->M21;
    transform[5] = matrix->M22;
    transform[6] = matrix->M23;
    transform[7] = matrix->M24;
    transform[8] = matrix->M31;
    transform[9] = matrix->M32;
    transform[10] = matrix->M33;
    transform[11] = matrix->M34;
    transform[12] = matrix->M41;
    transform[13] = matrix->M42;
    transform[14] = matrix->M43;
    transform[15] = matrix->M44;
}

Sprender_Matrix Sprender_Matrix_CreateFromCamera(Sprender_Camera* camera)
{
    assert(camera != NULL);
    
    Sprender_Matrix matrix = Sprender_Matrix_Create();
    
    const Sprender_Float2D position = {
        .X = camera->position.X + camera->origin.X,
        .Y = camera->position.Y + camera->origin.Y,
    };
    
    matrix.M11 = (2.0f / camera->resolution.X) * (camera->zoom.X);
    matrix.M14 = 0.0f - (2.0f / (camera->resolution.X / position.X / camera->zoom.X));
    matrix.M22 = -(2.0f / camera->resolution.Y) * (camera->zoom.Y);
    matrix.M24 = 0.0f + (2.0f / (camera->resolution.Y / position.Y / camera->zoom.Y));
    matrix.M33 = 1;
    matrix.M44 = 1;
    
    return matrix;
}
