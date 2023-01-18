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

Sprender_Matrix Sprender_Matrix_Multiply(
    Sprender_Matrix matrix1,
    Sprender_Matrix matrix2
)
{
    Sprender_Matrix result;
    
    result.M11 = (
          (matrix1.M11 * matrix2.M11)
        + (matrix1.M12 * matrix2.M21)
        + (matrix1.M13 * matrix2.M31)
        + (matrix1.M14 * matrix2.M41)
    );
    result.M12 = (
          (matrix1.M11 * matrix2.M12)
        + (matrix1.M12 * matrix2.M22)
        + (matrix1.M13 * matrix2.M32)
        + (matrix1.M14 * matrix2.M42)
    );
    result.M13 = (
          (matrix1.M11 * matrix2.M13)
        + (matrix1.M12 * matrix2.M23)
        + (matrix1.M13 * matrix2.M33)
        + (matrix1.M14 * matrix2.M43)
    );
    result.M14 = (
          (matrix1.M11 * matrix2.M14)
        + (matrix1.M12 * matrix2.M24)
        + (matrix1.M13 * matrix2.M34)
        + (matrix1.M14 * matrix2.M44)
    );
    
    result.M21 = (
          (matrix1.M21 * matrix2.M11)
        + (matrix1.M22 * matrix2.M21)
        + (matrix1.M23 * matrix2.M31)
        + (matrix1.M24 * matrix2.M41)
    );
    result.M22 = (
          (matrix1.M21 * matrix2.M12)
        + (matrix1.M22 * matrix2.M22)
        + (matrix1.M23 * matrix2.M32)
        + (matrix1.M24 * matrix2.M42)
    );
    result.M23 = (
          (matrix1.M21 * matrix2.M13)
        + (matrix1.M22 * matrix2.M23)
        + (matrix1.M23 * matrix2.M33)
        + (matrix1.M24 * matrix2.M43)
    );
    result.M24 = (
          (matrix1.M21 * matrix2.M14)
        + (matrix1.M22 * matrix2.M24)
        + (matrix1.M23 * matrix2.M34)
        + (matrix1.M24 * matrix2.M44)
    );
    
    result.M31 = (
          (matrix1.M31 * matrix2.M11)
        + (matrix1.M32 * matrix2.M21)
        + (matrix1.M33 * matrix2.M31)
        + (matrix1.M34 * matrix2.M41)
    );
    result.M32 = (
          (matrix1.M31 * matrix2.M12)
        + (matrix1.M32 * matrix2.M22)
        + (matrix1.M33 * matrix2.M32)
        + (matrix1.M34 * matrix2.M42)
    );
    result.M33 = (
          (matrix1.M31 * matrix2.M13)
        + (matrix1.M32 * matrix2.M23)
        + (matrix1.M33 * matrix2.M33)
        + (matrix1.M34 * matrix2.M43)
    );
    result.M34 = (
          (matrix1.M31 * matrix2.M14)
        + (matrix1.M32 * matrix2.M24)
        + (matrix1.M33 * matrix2.M34)
        + (matrix1.M34 * matrix2.M44)
    );
    
    result.M41 = (
          (matrix1.M41 * matrix2.M11)
        + (matrix1.M42 * matrix2.M21)
        + (matrix1.M43 * matrix2.M31)
        + (matrix1.M44 * matrix2.M41)
    );
    result.M42 = (
          (matrix1.M41 * matrix2.M12)
        + (matrix1.M42 * matrix2.M22)
        + (matrix1.M43 * matrix2.M32)
        + (matrix1.M44 * matrix2.M42)
    );
    result.M43 = (
          (matrix1.M41 * matrix2.M13)
        + (matrix1.M42 * matrix2.M23)
        + (matrix1.M43 * matrix2.M33)
        + (matrix1.M44 * matrix2.M43)
    );
    result.M44 = (
          (matrix1.M41 * matrix2.M14)
        + (matrix1.M42 * matrix2.M24)
        + (matrix1.M43 * matrix2.M34)
        + (matrix1.M44 * matrix2.M44)
    );
    
    return result;
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
    
    Sprender_Matrix translate = Sprender_Matrix_Create();
    translate.M14 = -camera->position.X;
    translate.M24 = -camera->position.Y;
    
    Sprender_Matrix scale = Sprender_Matrix_Create();
    scale.M11 = camera->zoom.X;
    scale.M22 = camera->zoom.Y;
    
    Sprender_Matrix rotationX = Sprender_Matrix_Create();
    rotationX.M22 = cosf(camera->rotation.X);
    rotationX.M23 = -sinf(camera->rotation.X);
    rotationX.M32 = sinf(camera->rotation.X);
    rotationX.M33 = cosf(camera->rotation.X);
    
    Sprender_Matrix rotationY = Sprender_Matrix_Create();
    rotationY.M11 = cosf(camera->rotation.Y);
    rotationY.M13 = sinf(camera->rotation.Y);
    rotationY.M31 = -sinf(camera->rotation.Y);
    rotationY.M33 = cosf(camera->rotation.Y);
    
    Sprender_Matrix rotationZ = Sprender_Matrix_Create();
    rotationZ.M11 = cosf(camera->rotation.Z);
    rotationZ.M12 = -sinf(camera->rotation.Z);
    rotationZ.M21 = sinf(camera->rotation.Z);
    rotationZ.M22 = cosf(camera->rotation.Z);
    
    if (camera->is3D) {
        const float ar = (float)camera->resolution.X / (float)camera->resolution.Y;
        const float zNear = 0.0f;
        const float zFar = 1.0f;
        const float zRange = zNear - zFar;
        const float tanHalfFOV = 0.5f * tanf(1.55f);
        
        matrix.M11 = 1.0f / (tanHalfFOV * ar);
        matrix.M22 = -1.0f / tanHalfFOV;
        matrix.M33 = (-zNear - zFar) / zRange;
        matrix.M34 = 2.0f * zFar * zNear / zRange;
        matrix.M43 = 1.0f;
    } else {
        matrix.M11 = (2.0f / camera->resolution.X);
        matrix.M22 = -(2.0f / camera->resolution.Y);
    }
    
    matrix = Sprender_Matrix_Multiply(
        matrix,
        translate
    );
    matrix = Sprender_Matrix_Multiply(
        matrix,
        scale
    );
    matrix = Sprender_Matrix_Multiply(
        matrix,
        rotationX
    );
    matrix = Sprender_Matrix_Multiply(
        matrix,
        rotationY
    );
    matrix = Sprender_Matrix_Multiply(
        matrix,
        rotationZ
    );
    
    return matrix;
}
