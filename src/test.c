#include <stdio.h>
#include <SDL2/SDL.h>
#include "Sprender/Sprender.h"

int main()
{
    printf("Hello, World!\n");
    
    Sprender* sprender = Sprender_Create(
        "Sprender Test",
        640, 360, // window size
        320, 180, // game resolution
        "Vulkan",
        0
    );
    
    SDL_Delay(1000);
    
    Sprender_Destroy(sprender);
    
    return 0;
}
