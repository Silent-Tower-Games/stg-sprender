#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Sprender/Camera.h"
#include "Sprender/Quad.h"
#include "Sprender/RenderMode.h"
#include "Sprender/Sprender.h"
#include "Sprender/Shader.h"
#include "Sprender/Texture.h"

// TODO: Index buffer
// TODO: Resizing
// TODO: Fullscreen
// TODO: NULL-safety
// TODO: Comments

int main()
{
    printf("Hello, World! %d\n", sizeof(float));
    
    Sprender* sprender = Sprender_Create(
        "Sprender Test",
        (Sprender_Int2D){ 960, 540, }, // window size
        (Sprender_Int2D){ 320, 180, }, // game resolution
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
    const int w = 6;
    const int h = 32;
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
                
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_SPACE:
                        {
                            printf("ok\n");
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
        Sprender_Load_RenderMode(sprender, &renderModeSub2);
        
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
            (i / 2) % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        // Render to backbuffer
        Sprender_Load_RenderMode(sprender, NULL);
        
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &textureSpriteSheet
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 0, },
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
            i % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 1, 0, },
            (Sprender_Float2D){ -160, 16, },
            (Sprender_Float2D){ 8.0f, 8.0f, },
            i % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 1, 0, },
            (Sprender_Float2D){ -160, -90, },
            (Sprender_Float2D){ 8.0f, 8.0f, },
            i % 2 ? 0.5f : 1.0f,
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &renderModeSub2.renderTargetTexture
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
            &texture2
        );
        
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 0, },
            (Sprender_Float2D){ 0, 0, },
            (Sprender_Float2D){ 1000.0f, 1000.0f, },
            0.0f,
            0xFF999999
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        Sprender_Close(sprender);
        
        SDL_Delay(16);
    }
    
    Sprender_Destroy(sprender);
    
    return 0;
}
