#pragma once

#include <SDL2/SDL.h>
#include <FNA3D.h>
#include "RenderMode.h"
#include "Shader.h"
#include "SpriteBatch.h"

typedef struct Sprender_FNA3D
{
    FNA3D_Device* device;
    FNA3D_BlendState blendState;
    FNA3D_PresentationParameters presentationParameters;
    FNA3D_SamplerState samplerState;
    FNA3D_VertexBufferBinding vertexBufferBinding;
    FNA3D_VertexElement vertexElements[3];
} Sprender_FNA3D;

typedef struct Sprender
{
    Sprender_FNA3D fna3d;
    int maxSprites;
    Sprender_Shader shaderSpriteEffect;
    Sprender_SpriteBatch spriteBatch;
    SDL_Window* window;
    char renderedToWindow;
    Sprender_RenderMode defaultRenderMode;
} Sprender;

Sprender* Sprender_Create(
    char* windowTitle,
    Sprender_Int2D windowSize,
    Sprender_Int2D resolution,
    char* driver,
    int maxSprites,
    Uint32 flags
);

void Sprender_RenderSprites(Sprender* sprender);

void Sprender_Load_RenderMode(Sprender* sprender, Sprender_RenderMode* renderMode);

void Sprender_Close(Sprender* sprender);

void Sprender_Destroy(Sprender* sprender);
