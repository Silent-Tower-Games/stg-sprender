#include "Sprender.h"

Sprender* Sprender_Create()
{
    Sprender* sprender = malloc(sizeof(Sprender));
    
    //sprender->window = SDL_Win
    
    return sprender;
}

void Sprender_Destroy(Sprender* sprender)
{
    free(sprender);
}
