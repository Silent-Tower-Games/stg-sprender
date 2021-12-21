#pragma once

#include <SDL2/SDL.h>

typedef struct Sprender
{
    SDL_Window* window;
} Sprender;

void Sprender_Init();

Sprender* Sprender_Create();

void Sprender_Destroy(Sprender* sprender);
