#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Sprender/Camera.h"
#include "Sprender/Quad.h"
#include "Sprender/RenderMode.h"
#include "Sprender/Sprender.h"
#include "Sprender/Shader.h"
#include "Sprender/Texture.h"

int main()
{
    printf("Hello, World!\n");
    
    Sprender* sprender = Sprender_Create(
        "Sprender Test",
        640, 360, // window size
        "OpenGL",
        10000, // 10k sprite maximum
        0
    );
    
    // TODO: Test multiple shaders
    Sprender_Shader shader = Sprender_Shader_Load(sprender->fna3d.device, "SpriteEffect", "assets/shaders/SpriteEffect.fxb");
    sprender->shaderSpriteEffect = shader;
    
    Sprender_Texture texture = Sprender_Texture_NewBlank(
        sprender->fna3d.device,
        (FNA3D_Vec4){ 1, 0, 1, 0.5f, },
        32,
        16,
        4,
        0
    );
    Sprender_Texture texture2 = Sprender_Texture_NewBlank(
        sprender->fna3d.device,
        (FNA3D_Vec4){ 1, 1, 1, 1, },
        16,
        32,
        4,
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
    textureSpriteSheet.tilesize.X = 8;
    textureSpriteSheet.tilesize.Y = 8;
    
    // Render mode
    const int w = 25;
    const int h = 125;
    const int border = 1;
    Sprender_RenderMode renderModeSub = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ w * 2, h * 2, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 0, 1, 0, 1, },
        1
    );
    Sprender_RenderMode renderModeSub2 = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ h * 2, w * 2, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 1, 1, 1, 1, },
        1
    );
    Sprender_RenderMode renderModeHuge = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ 1024, 1024, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 0, 1, 1, 1, },
        1
    );
    
    Sprender_Float2D position = {
        .X = 0,
        .Y = 0,
    };
    
    // We're gonna render this many frames
    char quit = 0;
    for(int i = 0; i > -60; i++)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    quit = 1;
                } break;
            }
        }
        
        if(quit)
        {
            break;
        }
        
        // Render fourth pass
        Sprender_Load_RenderMode(sprender, NULL);
        
        Sprender_SpriteBatch_Begin(&sprender->spriteBatch);
        
        Sprender_SpriteBatch_DrawFrame(
            &sprender->spriteBatch,
            &textureLogo,
            (Sprender_Int2D){ 0, 0, },
            (Sprender_Float2D){ 0, 0, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_DrawFrame(
            &sprender->spriteBatch,
            &textureSpriteSheet,
            (Sprender_Int2D){ 0, 0, },
            (Sprender_Float2D){ -128, -80, },
            (Sprender_Float2D){ 10.0f, 10.0f, },
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        Sprender_Close(sprender);
        
        SDL_Delay(16);
    }
    
    Sprender_Destroy(sprender);
    
    return 0;
}
