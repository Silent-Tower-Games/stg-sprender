#pragma once

#include <SDL2/SDL.h>
#include <FNA3D.h>

typedef struct Sprender_FNA3D
{
    FNA3D_Device* device;
    FNA3D_BlendState blendState;
    FNA3D_PresentationParameters presentationParameters;
    FNA3D_VertexBufferBinding vertexBufferBinding;
    FNA3D_VertexElement vertexElements[3];
} Sprender_FNA3D;

typedef struct Sprender
{
    SDL_Window* window;
} Sprender;

void Sprender_Init();

Sprender* Sprender_Create(
    char* windowTitle,
    int windowWidth,
    int windowHeight,
    int resolutionWidth,
    int resolutionHeight,
    char* driver,
    Uint32 flags
);

void Sprender_Destroy(Sprender* sprender);
