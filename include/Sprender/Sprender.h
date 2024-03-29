#pragma once

#include <SDL2/SDL.h>
#include <FNA3D.h>
#include <Sprender/RenderMode.h>
#include <Sprender/Shader.h>
#include <Sprender/SpriteBatch.h>

typedef struct Sprender_FNA3D
{
    FNA3D_Device* device;
    FNA3D_BlendState blendState;
    FNA3D_Buffer* indexBuffer;
    FNA3D_PresentationParameters presentationParameters;
    FNA3D_SamplerState samplerState;
    FNA3D_VertexBufferBinding vertexBufferBinding;
    FNA3D_VertexElement vertexElements[3];
} Sprender_FNA3D;

typedef struct Sprender
{
    Sprender_FNA3D fna3d;
    Sprender_Shader shaderSpriteEffect;
    SDL_Window* window;
    Sprender_RenderMode defaultRenderMode;
    Sprender_Int2D resolution;
    char vsync;
} Sprender;

/**
 * @brief Create a new Sprender object.
 * 
 * @param windowTitle the window's title string, to appear in the title bar
 * @param windowSize width & height of the window
 * @param resolution the game's resolution
 * @param spriteEffectShaderFilename location of the SpriteEffect.fxb shader file
 * @param driver optionally give a preferred graphics driver; "OpenGL", "Vulkan", "DirectX", or "Metal"
 * @param vsync whether or not to use vsync
 * @param SDLflags additional to pass to SDL_Init. `SDL_INIT_VIDEO` is already being used
 * @return Sprender* new Sprender object
 */
Sprender* Sprender_Create(
    char* windowTitle,
    Sprender_Int2D windowSize,
    Sprender_Int2D resolution,
    char* spriteEffectShaderFilename,
    char* driver,
    char vsync,
    Uint32 SDLflags
);

/**
 * @brief Set window size, fullscreen, and vsync.
 * 
 * @param sprender your sprender object
 * @param windowSize the new window size
 * @param vsync whether or not to use vsync
 * @param fullscreen whether or not to go fullscreen
 */
void Sprender_SetPresentation(Sprender* sprender, Sprender_Int2D windowSize, char fullscreen, char vsync);

/**
 * @brief Render all sprites staged by the SpriteBatch.
 * 
 * @param sprender your sprender object
 */
void Sprender_RenderSprites(Sprender* sprender, Sprender_SpriteBatch* spriteBatch);

/**
 * @brief Load a new RenderMode & set of shaders.
 * 
 * @param sprender your sprender object
 * @param renderMode your new render mode
 * @param shaders pointer to a list of shader objects
 * @param shadersCount number of shader objects
 */
void Sprender_Load(
    Sprender* sprender,
    Sprender_RenderMode* renderMode,
    Sprender_Shader* shaders,
    int shadersCount
);

/**
 * @brief End the current frame & swap buffers.
 * 
 * @param sprender your sprender object
 */
void Sprender_Close(Sprender* sprender);

/**
 * @brief Free all memory allocated by the Sprender object.
 * 
 * @param sprender your sprender object
 */
void Sprender_Destroy(Sprender* sprender);
