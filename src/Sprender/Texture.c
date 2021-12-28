#include <stdlib.h>
#include <string.h>
#include <FNA3D.h>
#include "../vendor/stb_image.h"
#include "Texture.h"

Sprender_Texture Sprender_Texture_NewFromData(FNA3D_Device* device, int width, int height, unsigned char* pixels, int channels, char isRenderTarget)
{
    Sprender_Texture texture;
    memset(&texture, 0, sizeof(Sprender_Texture));
    
    texture.size.X = width;
    texture.size.Y = height;
    texture.tilesize = texture.size;
    
    FNA3D_Texture* asset = FNA3D_CreateTexture2D(device, FNA3D_SURFACEFORMAT_COLOR, width, height, 1, isRenderTarget);
    FNA3D_SetTextureData2D(device, asset, 0, 0, width, height, 0, pixels, width * height * channels);
    texture.asset = asset;
    
    return texture;
}

Sprender_Texture Sprender_Texture_NewBlank(FNA3D_Device* device, int width, int height, int channels, char isRenderTarget)
{
    size_t size = width * height * channels;
    size_t sizeWithType = sizeof(unsigned char) * size;
    
    unsigned char* pixels = malloc(sizeWithType);
    memset(pixels, 0xFF, sizeWithType);
    
    Sprender_Texture texture = Sprender_Texture_NewFromData(device, width, height, pixels, channels, isRenderTarget);
    
    stbi_image_free(pixels);
    
    return texture;
}
