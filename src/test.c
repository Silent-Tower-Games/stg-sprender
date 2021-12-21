#include <stdio.h>
#include <SDL2/SDL.h>
#include "Sprender/Sprender.h"

int main()
{
    printf("Hello, World!\n");
    
    SDL_Init(SDL_INIT_VIDEO);
    
    Sprender* sprender = Sprender_Create();
    
    Sprender_Destroy(sprender);
    
    return 0;
}
