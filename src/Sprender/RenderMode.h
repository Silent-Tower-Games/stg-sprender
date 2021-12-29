#pragma once

#include <FNA3D.h>
#include "Camera.h"

typedef struct Sprender_RenderMode
{
    Sprender_Int2D resolution;
    Sprender_Camera camera;
    FNA3D_RenderTargetBinding* renderTarget;
    FNA3D_Viewport viewport;
} Sprender_RenderMode;

void Sprender_RenderMode_Create(Sprender_Int2D resolution);
