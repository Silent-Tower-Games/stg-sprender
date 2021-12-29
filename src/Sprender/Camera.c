#include <string.h>
#include "Camera.h"
#include "Matrix.h"

Sprender_Camera Sprender_Camera_Create(float resolutionX, float resolutionY, float zoomX, float zoomY)
{
    Sprender_Camera camera;
    memset(&camera, 0, sizeof(camera));
    
    camera.resolution.X = resolutionX;
    camera.resolution.Y = resolutionY;
    camera.zoom.X = zoomX;
    camera.zoom.Y = zoomY;
    
    return camera;
}

void Sprender_Camera_LoadInto(Sprender_Camera* camera, void* dest)
{
    Sprender_Matrix matrix = Sprender_Matrix_CreateFromCamera(camera);
    
    Sprender_Matrix_ToTransform(&matrix, dest);
}
