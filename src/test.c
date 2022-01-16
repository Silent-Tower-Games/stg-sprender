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
        (Sprender_Int2D){ 320, 180, }, // game resolution
        "assets/shaders/SpriteEffect.fxb", // SpriteEffect filename
        NULL, // let FNA3D choose the graphics driver
        10000, // 10k sprite maximum
        1, // not vsync
        0 // not passing in any SDL flags
    );
    
    // YellowShader.fx
    Sprender_Shader shaderYellow = Sprender_Shader_Load(sprender->fna3d.device, "assets/shaders/YellowShader.fxb", yellowShaderStep);
    
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
        "assets/images/8x8.png"
    );
    // Set the tile size of the sprite sheet
    textureSpriteSheet.tilesize.X = 8;
    textureSpriteSheet.tilesize.Y = 8;
    
    // Render mode
    const int w = 6;
    const int h = 32;
    Sprender_RenderMode renderModeTest = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ h * 2, w * 2, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 1, 1, 1, 1, },
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
        
        // Render to RT
        Sprender_Load(
            sprender,
            &renderModeTest,
            NULL,
            0
        );
        
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &textureSpriteSheet
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 0, },
            (Sprender_Float2D){ i / 10.0f, 0, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            0.75f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 1, },
            (Sprender_Float2D){ 16, 0, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            (i / 30) % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        // Render to backbuffer
        Sprender_Load(
            sprender,
            NULL,
            &shaderYellow,
            1
        );
        
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &textureSpriteSheet
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ (i / 30) % 2, 0, },
            (Sprender_Float2D){ i, 0, },
            (Sprender_Float2D){ 8.0f, 8.0f, },
            0.75f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 1, 1, },
            (Sprender_Float2D){ 0, 0, },
            (Sprender_Float2D){ 8.0f, 8.0f, },
            (i / 30) % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 1, 0, },
            (Sprender_Float2D){ -160, 16, },
            (Sprender_Float2D){ 8.0f, 8.0f, },
            (i / 30) % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 1, 0, },
            (Sprender_Float2D){ -160, -90, },
            (Sprender_Float2D){ 8.0f, 8.0f, },
            (i / 30) % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &renderModeTest.renderTargetTexture
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 0, },
            (Sprender_Float2D){ -160, -40, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
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
            0xFF990000
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
    Sprender_RenderMode_Destroy(sprender->fna3d.device, &renderModeTest);
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
