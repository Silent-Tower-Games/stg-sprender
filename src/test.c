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
        320, 180, // game resolution
        NULL,
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
    // Camera
    Sprender_Camera camera = Sprender_Camera_Create(
        (Sprender_Int2D){ 640, 360, },
        (Sprender_Float2D){ 1.0f, 1.0f, }
    );
    // Render mode
    Sprender_RenderMode renderMode = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ 640, 360, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 1, 1, 1, 1, },
        0
    );
    Sprender_RenderMode renderModeSub = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ 24, 24, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 0, 1, 0, 1, },
        1
    );
    
    // We're gonna render this many frames
    for(int i = 0; i < 60; i++)
    {
        // Render first pass
        Sprender_RenderMode_Load(sprender, &renderModeSub);
        
        Sprender_SpriteBatch_Begin(&sprender->spriteBatch);
        
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
                .topLeft = { -8, -8, },
                .topRight = { 8, -8, },
                .bottomLeft = { -8, 8, },
                .bottomRight = { 8, 8, },
            },
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        // Render second pass
        Sprender_RenderMode_Load(sprender, &renderMode);
        
        Sprender_SpriteBatch_Begin(&sprender->spriteBatch);
        
        Sprender_SpriteBatch_DrawQuad(
            &sprender->spriteBatch,
            renderModeSub.renderTarget.texture,
            (Sprender_Quad){
                .topLeft = { 0.25f, 0.25f, },
                .topRight = { 0.75f, 0.25f, },
                .bottomLeft = { 0.25f, 0.75f, },
                .bottomRight = { 0.75f, 0.75f, },
            },
            (Sprender_Quad){
                .topLeft = { -48, -48, },
                .topRight = { -24, -48, },
                .bottomLeft = { -48, -24, },
                .bottomRight = { -24, -24, },
            },
            0xFFFFFFFF
        );
        
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
            0xFF00FFFF
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
        
        Sprender_Close(sprender);
        
        SDL_Delay(16);
    }
    
    Sprender_Destroy(sprender);
    
    return 0;
}
