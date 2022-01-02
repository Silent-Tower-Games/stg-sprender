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
    FNA3D_Renderbuffer* depthRenderbuffer;
} Sprender_RenderMode;

/**
 * @brief Create a new render mode.
 * 
 * @param device FNA3D device
 * @param resolution size of this render mode
 * @param position position of this render mode's viewport (probably should be 0x0)
 * @param bgColor color for this render mode to clear to. { R, G, B, A }; values between 0.0f and 1.0f
 * @param hasRenderTarget 0 to use backbuffer, 1 to create render target
 * @return Sprender_RenderMode new render mode
 */
Sprender_RenderMode Sprender_RenderMode_Create(
    FNA3D_Device* device,
    Sprender_Int2D resolution,
    Sprender_Int2D position,
    FNA3D_Vec4 bgColor,
    char hasRenderTarget
);

/**
 * @brief Free all memory allocated by the render mode.
 * 
 * @param renderMode your render mode
 */
void Sprender_RenderMode_Destroy(Sprender_RenderMode* renderMode);
