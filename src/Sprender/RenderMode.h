#pragma once

#include <FNA3D.h>
#include "Camera.h"
#include "Sprender.h"

typedef struct Sprender_RenderMode
{
    FNA3D_Vec4 bgColor;
    Sprender_Int2D resolution;
    Sprender_Camera camera;
    FNA3D_RenderTargetBinding renderTarget;
    FNA3D_Viewport viewport;
} Sprender_RenderMode;

Sprender_RenderMode Sprender_RenderMode_Create(
    FNA3D_Device* device,
    Sprender_Int2D resolution,
    Sprender_Int2D position,
    FNA3D_Vec4 bgColor,
    char hasRenderTarget
);

void Sprender_RenderMode_Load(Sprender* sprender, Sprender_RenderMode* renderMode);

void Sprender_RenderMode_Destroy(Sprender_RenderMode* renderMode);
