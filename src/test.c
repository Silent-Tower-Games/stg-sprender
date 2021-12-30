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
    
    // Render mode
    // TODO: Sprender should own the window RenderMode
    Sprender_RenderMode renderMode = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ 640, 360, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 1, 0, 1, 1, },
        0
    );
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
        (FNA3D_Vec4){ 0, 1, 1, 1, },
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
                .topLeft = { -w + border, -h + border, },
                .topRight = { w - border, -h + border, },
                .bottomLeft = { -w + border, h - border, },
                .bottomRight = { w - border, h - border, },
            },
            0xFFFF0000
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        // Render second pass
        Sprender_RenderMode_Load(sprender, &renderModeSub2);
        
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
                .topLeft = { -h + border, -w + border, },
                .topRight = { h - border, -w + border, },
                .bottomLeft = { -h + border, w - border, },
                .bottomRight = { h - border, w - border, },
            },
            0xFFFF0000
        );
        
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        Sprender_RenderSprites(sprender);
        
        // Render third pass
        Sprender_RenderMode_Load(sprender, &renderMode);
        
        Sprender_SpriteBatch_Begin(&sprender->spriteBatch);
        
        Sprender_SpriteBatch_DrawQuad(
            &sprender->spriteBatch,
            renderModeSub.renderTarget.texture,
            (Sprender_Quad){
                .topLeft = { 0, 0, },
                .topRight = { 1, 0, },
                .bottomLeft = { 0, 1, },
                .bottomRight = { 1, 1, },
            },
            (Sprender_Quad){
                .topLeft = { -w, -h, },
                .topRight = { w, -h, },
                .bottomLeft = { -w, h, },
                .bottomRight = { w, h, },
            },
            0xFFFFFFFF
        );
        
        Sprender_SpriteBatch_DrawQuad(
            &sprender->spriteBatch,
            renderModeSub2.renderTarget.texture,
            (Sprender_Quad){
                .topLeft = { 0, 0, },
                .topRight = { 1, 0, },
                .bottomLeft = { 0, 1, },
                .bottomRight = { 1, 1, },
            },
            (Sprender_Quad){
                .topLeft = { -h - 64, -w, },
                .topRight = { h - 64, -w, },
                .bottomLeft = { -h - 64, w, },
                .bottomRight = { h - 64, w, },
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
            0xFFFF0000
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
            0xFFFFFF00
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
