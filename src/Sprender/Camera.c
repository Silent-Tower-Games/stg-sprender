#include <assert.h>
#include <string.h>
#include "Camera.h"
#include "Matrix.h"

Sprender_Camera Sprender_Camera_Create(Sprender_Int2D resolution, Sprender_Float3D zoom)
{
    Sprender_Camera camera;
    memset(&camera, 0, sizeof(camera));
    
    camera.resolution = resolution;
    camera.zoom = zoom;
    
    return camera;
}

void Sprender_Camera_LoadInto(Sprender_Camera* camera, void* dest)
{
    assert(camera != NULL);
    assert(dest != NULL);
    
    Sprender_Matrix matrix = Sprender_Matrix_CreateFromCamera(camera);
    
    Sprender_Matrix_ToTransform(&matrix, dest);
}
