#pragma once

#include <FNA3D.h>
#include "Int2D.h"

typedef struct Sprender_Texture
{
    FNA3D_Texture* asset;
    Sprender_Int2D size;
    Sprender_Int2D tilesize;
} Sprender_Texture;

Sprender_Texture Sprender_Texture_NewFromData(FNA3D_Device* device, int width, int height, unsigned char* pixels, int channels, char isRenderTarget);

Sprender_Texture Sprender_Texture_NewBlank(FNA3D_Device* device, int width, int height, int channels, char isRenderTarget);
