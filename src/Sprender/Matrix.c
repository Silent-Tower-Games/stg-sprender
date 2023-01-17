#include <assert.h>
#include <math.h>
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
    memset(&matrix, 0, sizeof(matrix));
    
    /*
    aspect = your screen width / height (iirc)
    fovY = vertical fov in radians
    zNear = the closest stuff can get to the eye without clipping
    zFar = the furthest away stuff can be without clipping

    height = 1 / tan(fovY * 0.5)
    width = height / aspect
    invDepth = 1.0 / (zFar - zNear)

    [ width,      0,                              0,      0]
    [     0, height,                              0,      0]
    [     0,      0,     -(zFar + zNear) * invDepth,      0]
    [     0,      0, -(2 * zFar * zNear) * invDepth,      0]
    */
    
    const char is3D = 1;
    
    if (is3D) {
        const float aspect = camera->resolution.X / camera->resolution.Y;
        const float fovY = 1.5708f; // 90deg to rad
        const float zNear = 0.01f;
        const float zFar = 1.0f;
        const float height = 1 / tanf(fovY * 0.5f);
        const float width = height / aspect;
        const float invDepth = 1.0f / (zFar - zNear);

        matrix.M11 = width;
        matrix.M22 = height;
        matrix.M33 = -(zFar + zNear) * invDepth;
        matrix.M43 = -(2 * zFar * zNear) * invDepth;
    } else {
        matrix.M11 = (2.0f / camera->resolution.X) * (camera->zoom.X);
        matrix.M14 = 0.0f - (2.0f / (camera->resolution.X / camera->position.X / camera->zoom.X));
        matrix.M22 = -(2.0f / camera->resolution.Y) * (camera->zoom.Y);
        matrix.M24 = 0.0f + (2.0f / (camera->resolution.Y / camera->position.Y / camera->zoom.Y));
        matrix.M33 = 1;
        matrix.M44 = 1;
    }
    
    return matrix;
}
