#include <assert.h>
#include <math.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Sprender/Camera.h"
#include "Sprender/Quad.h"
#include "Sprender/RenderMode.h"
#include "Sprender/Sprender.h"
#include "Sprender/Shader.h"
#include "Sprender/Texture.h"

// TODO: Destroy functions for everything; valgrind, etc

/**
 * @brief Example of a shader callable function. Fades the screen into yellow & back to normal.
 * 
 * @param shader the shader that this function is applied to
 * @return char returns 1 to use shader this frame, 0 to skip it
 */
char yellowShaderStep(Sprender_Shader* shader);

int main(int argc, char** argv)
{
    printf("Hello, World!\n");
    
    Sprender* sprender = Sprender_Create(
        "Sprender Example",
        (Sprender_Int2D){ 960, 540, }, // window size
        (Sprender_Int2D){ 80, 45, }, // game resolution
        "assets/shaders/SpriteEffect.fxb", // SpriteEffect filename
        NULL, // let FNA3D choose the graphics driver
        10000, // 10k sprite maximum
        1, // not vsync
        0 // not passing in any SDL flags
    );
    
    // YellowShader.fx
    Sprender_Shader shaderYellow = Sprender_Shader_Load(sprender->fna3d.device, "assets/shaders/YellowShader.fxb", yellowShaderStep);
    // DepthShader.fx
    Sprender_Shader shaderDepth = Sprender_Shader_Load(sprender->fna3d.device, "assets/shaders/DepthShader.fxb", NULL);
    
    Sprender_Texture textureBlankWhite = Sprender_Texture_NewBlank(
        sprender->fna3d.device,
        (FNA3D_Vec4){ 1, 1, 1, 1, },
        16,
        32,
        0
    );
    Sprender_Texture textureLogo = Sprender_Texture_Load(
        sprender->fna3d.device,
        "assets/images/logo.png"
    );
    Sprender_Texture textureSpriteSheet = Sprender_Texture_Load(
        sprender->fna3d.device,
        "assets/images/8x8-padding.png"
    );
    // Set the tile size of the sprite sheet
    textureSpriteSheet.tilesize.X = 8;
    textureSpriteSheet.tilesize.Y = 8;
    textureSpriteSheet.padding.X = 1;
    textureSpriteSheet.padding.Y = 1;
    textureSpriteSheet.border.X = 1;
    textureSpriteSheet.border.Y = 1;
    
    Sprender_RenderMode myRenderTarget = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ 32, 32 },
        (Sprender_Int2D){ 0, 0 },
        (FNA3D_Vec4){ 0, 0, 0.5f, 1 },
        1
    );
    
    char quit = 0;
    char isFullscreen = 0;
    int i = 0;
    while(1)
    {
        i++;
        
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    quit = 1;
                } break;
                
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_SPACE:
                        {
                            Sprender_SetPresentation(sprender, (Sprender_Int2D){
                                isFullscreen ? 640 : 1920,
                                isFullscreen ? 360 : 1080,
                            }, !isFullscreen, sprender->vsync);
                            
                            isFullscreen = !isFullscreen;
                        } break;
                        
                        case SDLK_v:
                        {
                            Sprender_SetPresentation(sprender, (Sprender_Int2D){
                                !isFullscreen ? 640 : 1920,
                                !isFullscreen ? 360 : 1080,
                            }, isFullscreen, !sprender->vsync);
                        } break;
                    }
                } break;
            }
        }
        
        if(quit)
        {
            break;
        }
        
        // Render to target
        Sprender_Load(
            sprender,
            &myRenderTarget,
            // This shader applies to the sprite sheet, not the final image
            &shaderDepth,
            1
        );
        
        // Blue background
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &textureBlankWhite
        );
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 0, },
            (Sprender_Float2D){ 0, 0, },
            (Sprender_Float2D){ 1000.0f, 1000.0f, },
            0.0f,
            0,
            0xFFFFFFFF
        );
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        Sprender_RenderSprites(sprender);
        
        // Sprites
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &textureSpriteSheet
        );
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 1, 0, },
            (Sprender_Float2D){ -8, 0, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            0.75f,
            0,
            0xFFFFFFFF
        );
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 6, 3, },
            (Sprender_Float2D){ -8, 0, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            (i / 30) % 2 ? 0.5f : 1.0f,
            0,
            0xFFFFFFFF
        );
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 6, 3, },
            (Sprender_Float2D){ 10, 0, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            (i / 30) % 2 ? 1.0f : 0.5f,
            0,
            0xFFFFFFFF
        );
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 1, 0, },
            (Sprender_Float2D){ 12, 0, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            0.75f,
            0,
            0xFFFFFFFF
        );
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        Sprender_RenderSprites(sprender);
        
        // Render to backbuffer
        Sprender_Load(
            sprender,
            NULL,
            // This shader applies to the render target as a whole, since that's the texture we're rendering
            &shaderYellow,
            1
        );
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &myRenderTarget.renderTargetTexture
        );
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 0 },
            (Sprender_Float2D){ 0, 0 },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            0,
            0,
            0xFFFFFFFF
        );
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        Sprender_RenderSprites(sprender);
        
        Sprender_Close(sprender);
        
        if(!sprender->vsync)
        {
            SDL_Delay(16);
        }
    }
    
    Sprender_Shader_Destroy(sprender->fna3d.device, &shaderYellow);
    Sprender_Texture_Destroy(sprender->fna3d.device, &textureBlankWhite);
    Sprender_Texture_Destroy(sprender->fna3d.device, &textureSpriteSheet);
    Sprender_Texture_Destroy(sprender->fna3d.device, &textureLogo);
    Sprender_Destroy(sprender);
    
    return 0;
}

int yellowShaderStep_Ticker = 0;
char yellowShaderStep(Sprender_Shader* shader)
{
    float magnitude = fabs(sin((float)(++yellowShaderStep_Ticker) / 100));
    
    Sprender_Shader_ParamCopy(shader, "magnitude", &magnitude, sizeof(float));
    
    return 1;
}
