#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <FNA3D.h>
#include "../vendor/stb_image.h"
#include "Texture.h"

Sprender_Texture Sprender_Texture_NewFromData(
    FNA3D_Device* device,
    int width,
    int height,
    unsigned char* pixels,
    int channels,
    char isRenderTarget
)
{
    assert(device != NULL);
    assert(pixels != NULL);
    
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

Sprender_Texture Sprender_Texture_NewBlank(
    FNA3D_Device* device,
    FNA3D_Vec4 color,
    int width,
    int height,
    int channels,
    char isRenderTarget
)
{
    assert(device != NULL);
    
    size_t size = width * height * channels;
    size_t sizeWithType = sizeof(unsigned char) * size;
    
    unsigned char* pixels = malloc(sizeWithType);
    for(int i = 0; i < size; i += 4)
    {
        pixels[i + 0] = 0xFF * color.x;
        pixels[i + 1] = 0xFF * color.y;
        pixels[i + 2] = 0xFF * color.z;
        pixels[i + 3] = 0xFF * color.w;
    }
    
    Sprender_Texture texture = Sprender_Texture_NewFromData(device, width, height, pixels, channels, isRenderTarget);
    
    free(pixels);
    
    return texture;
}

Sprender_Texture Sprender_Texture_Load(FNA3D_Device* device, const char* filename)
{
    assert(device != NULL);
    assert(filename != NULL);
    
    Sprender_Int2D size;
    int channels;
    
    unsigned char* pixels = stbi_load(filename, &size.X, &size.Y, &channels, 4);
    
    assert(pixels != NULL);
    
    Sprender_Texture texture = Sprender_Texture_NewFromData(device, size.X, size.Y, pixels, channels, 0);
    texture.size = size;
    texture.tilesize = size;
    
    stbi_image_free(pixels);
    
    return texture;
}
