#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Sprender/Camera.h"
#include "Sprender/Quad.h"
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
        (FNA3D_Vec4){ 1, 1, 1, 0.5f, },
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
    FNA3D_Vec4 color = { 0, 1, 0, 1, };
    // Camera
    // TODO: Try moving the camera & make sure that works
    // TODO: Try zooming the camera
    Sprender_Camera camera = Sprender_Camera_Create(640, 360, 1, 1);
    
    // We're gonna render this many frames
    for(int i = 0; i < 60; i++)
    {
        FNA3D_SetViewport(sprender->fna3d.device, &viewport);
        
        FNA3D_SetRenderTargets(
            sprender->fna3d.device,
            NULL,
            0,
            NULL,
            FNA3D_DEPTHFORMAT_NONE,
            0
        );
        
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
        
        Sprender_SpriteBatch_Begin(&sprender->spriteBatch);
        
        Sprender_SpriteBatch_DrawQuad(
            &sprender->spriteBatch,
            texture.asset,
            (Sprender_Quad){
                .topLeft = { 0.25f, 0.25f, },
                .topRight = { 0.75f, 0.25f, },
                .bottomLeft = { 0.25f, 0.75f, },
                .bottomRight = { 0.75f, 0.75f, },
            },
            (Sprender_Quad){
                .topLeft = { 32, 33, },
                .topRight = { 64, 33, },
                .bottomLeft = { 32, 65, },
                .bottomRight = { 64, 65, },
            },
            0xFFFFFFFF
        );
        //*
        Sprender_SpriteBatch_DrawQuad(
            &sprender->spriteBatch,
            texture2.asset,
            (Sprender_Quad){
                .topLeft = { 0, 0, },
                .topRight = { 1, 0, },
                .bottomLeft = { 0, 1, },
                .bottomRight = { 1, 1, },
            },
            (Sprender_Quad){
                .topLeft = { 0, 0, },
                .topRight = { 8, 0, },
                .bottomLeft = { 0, 8, },
                .bottomRight = { 8, 8, },
            },
            0xFFFFFFFF
        );
        //*/
        //*
        Sprender_SpriteBatch_DrawQuad(
            &sprender->spriteBatch,
            textureLogo.asset,
            (Sprender_Quad){
                .topLeft = { 0, 0, },
                .topRight = { 1, 0, },
                .bottomLeft = { 0, 1, },
                .bottomRight = { 1, 1, },
            },
            (Sprender_Quad){
                .topLeft = { 64, 64, },
                .topRight = { 256, 64, },
                .bottomLeft = { 64, 128, },
                .bottomRight = { 256, 128, },
            },
            0xFFFFFFFF
        );
        //*/
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        Sprender_Close();
        
        SDL_Delay(16);
    }
    
    Sprender_Destroy(sprender);
    
    return 0;
}
