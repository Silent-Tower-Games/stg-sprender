#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Sprender/Camera.h"
#include "Sprender/Sprender.h"
#include "Sprender/Shader.h"
#include "Sprender/Texture.h"

int main()
{
    printf("Hello, World!\n");
    
    Sprender* sprender = Sprender_Create(
        "Sprender Test",
        640, 360, // window size
        320, 180, // game resolution
        NULL,
        10000, // 10k sprite maximum
        0
    );
    
    Sprender_Shader shader = Sprender_Shader_Load(sprender->fna3d.device, "SpriteEffect", "assets/shaders/SpriteEffect.fxb");
    
    Sprender_Texture texture = Sprender_Texture_NewBlank(
        sprender->fna3d.device,
        32,
        16,
        4,
        0
    );
    
    // TODO: Make this not ugly!
    // Viewport
    FNA3D_Viewport viewport = {
        .w = 640,
        .h = 360,
        .x = 0,
        .y = 0,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    // Clear color
    FNA3D_Vec4 color = { 0, 0, 0, 1, };
    // Camera
    Sprender_Camera camera = Sprender_Camera_Create(640, 360, 1, 1);
    
    // We're gonna render this many frames
    for(int i = 0; i < 1; i++)
    {
        FNA3D_SetRenderTargets(
            sprender->fna3d.device,
            NULL,
            0,
            NULL,
            FNA3D_DEPTHFORMAT_NONE,
            0
        );
        
        FNA3D_SetViewport(sprender->fna3d.device, &viewport);
        
        FNA3D_Clear(
            sprender->fna3d.device,
            FNA3D_CLEAROPTIONS_TARGET,
            &color,
            0,
            0
        );
        
        MOJOSHADER_effectParam* shaderMatrix = Sprender_Shader_ParamGet(&shader, "MatrixTransform");
        assert(shaderMatrix != NULL);
        
        Sprender_Camera_LoadInto(&camera, shaderMatrix->value.values);
        
        MOJOSHADER_effectStateChanges stateChanges;
        memset(&stateChanges, 0, sizeof(stateChanges));
        FNA3D_ApplyEffect(
            sprender->fna3d.device,
            shader.effect,
            0,
            &stateChanges
        );
        
        // TODO: SpriteBatch add vertices
        
        // TODO: SpriteBatch end
        
        FNA3D_SwapBuffers(
            sprender->fna3d.device,
            NULL,
            NULL,
            sprender->window
        );
        
        SDL_Delay(1000);
    }
    
    Sprender_Destroy(sprender);
    
    return 0;
}
