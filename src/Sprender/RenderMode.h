#pragma once

#include <FNA3D.h>
#include "Camera.h"
#include "Texture.h"

typedef struct Sprender_RenderMode
{
    FNA3D_Vec4 bgColor;
    Sprender_Int2D resolution;
    Sprender_Camera camera;
    FNA3D_RenderTargetBinding renderTarget;
    Sprender_Texture renderTargetTexture;
    FNA3D_Viewport viewport;
} Sprender_RenderMode;

Sprender_RenderMode Sprender_RenderMode_Create(
    FNA3D_Device* device,
    Sprender_Int2D resolution,
    Sprender_Int2D position,
    FNA3D_Vec4 bgColor,
    char hasRenderTarget
);

void Sprender_RenderMode_Destroy(Sprender_RenderMode* renderMode);