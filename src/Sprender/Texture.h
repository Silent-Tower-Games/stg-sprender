#pragma once

#include <FNA3D.h>
#include "Int2D.h"

typedef struct Sprender_Texture
{
    FNA3D_Texture* asset;
    Sprender_Int2D size;
    Sprender_Int2D tilesize;
} Sprender_Texture;

/**
 * @brief Create a new texture with given data.
 * 
 * @param device FNA3D device
 * @param width width of the texture
 * @param height height of the texture
 * @param pixels pointer to where the texture data begins
 * @param channels number of channels; e.g. 4 for RGBA
 * @param isRenderTarget whether or not this texture will be used as a render target
 * @return Sprender_Texture new texture
 */
Sprender_Texture Sprender_Texture_NewFromData(
    FNA3D_Device* device,
    int width,
    int height,
    unsigned char* pixels,
    int channels,
    char isRenderTarget
);

/**
 * @brief Create a new blank texture.
 * 
 * @param device FNA3D device
 * @param color RGBA color
 * @param width width of the texture
 * @param height height of the texture
 * @param isRenderTarget whether or not this texture will be used as a render target
 * @return Sprender_Texture new blank texture
 */
Sprender_Texture Sprender_Texture_NewBlank(
    FNA3D_Device* device,
    FNA3D_Vec4 color,
    int width,
    int height,
    char isRenderTarget
);

/**
 * @brief Load a texture from a file.
 * 
 * @param device FNA3D device
 * @param filename filename of texture
 * @return Sprender_Texture newly loaded texture
 */
Sprender_Texture Sprender_Texture_Load(FNA3D_Device* device, const char* filename);

/**
 * @brief Free all memory loaded by the texture.
 * 
 * @param device FNA3D device
 * @param texture your texture
 */
void Sprender_Texture_Destroy(FNA3D_Device* device, Sprender_Texture* texture);
