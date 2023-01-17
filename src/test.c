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
        1, // not vsync
        0 // not passing in any SDL flags
    );
    Sprender_SpriteBatch* spriteBatch = Sprender_SpriteBatch_Create(
        sprender->fna3d.device,
        10000, // 10k
        SPRENDER_SPRITEBATCH_INDEXBUFFER_USE // use an index buffer, don't pre-build it
    );
    
    // YellowShader.fx
    Sprender_Shader shaderYellow = Sprender_Shader_Load(sprender->fna3d.device, "assets/shaders/YellowShader.fxb", yellowShaderStep);
    // DepthShader.fx
    Sprender_Shader shaderDepth = Sprender_Shader_Load(sprender->fna3d.device, "assets/shaders/DepthShader.fxb", NULL);
    
    Sprender_Texture textureBlankWhite = Sprender_Texture_NewBlank(
        sprender->fna3d.device,
        (FNA3D_Vec4){ 1, 1, 1, 1, },
        1,
        1,
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
        
        // Render to backbuffer
        Sprender_Load(
            sprender,
            NULL,
            NULL,
            0
        );
        
        // Rainbow triangle
        Sprender_SpriteBatch_Begin(
            spriteBatch,
            &textureBlankWhite
        );
        const float depth = -0.75f;
        Sprender_SpriteBatch_StageTriangleVerts(
            spriteBatch,
            (Sprender_Vertex){
                .x = 0,
                .y = -16,
                .u = 0,
                .v = 0,
                .z = depth,
                .color = 0xFF0000FF,
            },
            (Sprender_Vertex){
                .x = -16,
                .y = 16,
                .u = 1,
                .v = 0,
                .z = depth,
                .color = 0xFF00FF00,
            },
            (Sprender_Vertex){
                .x = 16,
                .y = 16,
                .u = 0,
                .v = 1,
                .z = depth,
                .color = 0xFFFF0000,
            }
        );
        Sprender_SpriteBatch_End(spriteBatch);
        Sprender_RenderSprites(sprender, spriteBatch);
        
        Sprender_Close(sprender);
        
        if(!sprender->vsync)
        {
            SDL_Delay(16);
        }
    }
    
    Sprender_SpriteBatch_Destroy(spriteBatch);
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
