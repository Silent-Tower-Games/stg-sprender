# Sprender

A tiny C sprite-rendering library based on SDL & FNA3D.

# Example

A very simple example of how to use it:

```c
int main()
{
    Sprender* sprender = Sprender_Create(
        "Sprender Example",
        (Sprender_Int2D){ 960, 540, }, // window size
        (Sprender_Int2D){ 320, 180, }, // game resolution
        "SpriteEffect.fbx", // path to the SpriteEffect compiled shader
        NULL, // let FNA3D choose the graphics driver
              // you could also use e.g. "OpenGL", "Vulkan", etc.
        10000, // 10k sprite maximum
        0 // not passing in any SDL flags
    );
    
    Sprender_Texture textureLogo = Sprender_Texture_Load(
        sprender->fna3d.device,
        "image.png"
    );
    
    while(1) // make sure to break this loop
    {
        // SDL events should go here
        // e.g. break loop on SDL_QUIT
        
        // Very simple; no render targets, no shaders
        Sprender_Load(
            sprender,
            NULL,
            NULL,
            0
        );
        
        // You change textures with _Begin
        // You must call _End & draw before switching textures
        Sprender_SpriteBatch_Begin(
            &sprender->spriteBatch,
            &textureSpriteSheet
        );
        
        // Drawing tile # 0,0 from our texture
        // At position 32x64
        // At a scale of 1x1
        // At a depth of 1.0f (could be any 16-bit value over 0.0f)
        // Flipping on both the X and Y axes
        // With a white color (0xAABBGGRR, RGBA backwards)
        Sprender_SpriteBatch_StageFrame(
            &sprender->spriteBatch,
            (Sprender_Int2D){ 0, 0, },
            (Sprender_Float2D){ 32.0f, 64.0f, },
            (Sprender_Float2D){ 1.0f, 1.0f, },
            1.0f,
            SPRENDER_SPRITEBATCH_FLIP_X | SPRENDER_SPRITEBATCH_FLIP_Y,
            0xFFFFFFFF
        );
        
        // End batch with this texture
        Sprender_SpriteBatch_End(&sprender->spriteBatch);
        
        // Draw sprites to the screen
        Sprender_RenderSprites(sprender);
        
        // Finally, render everything to the screen
        // Do this once per frame, very last!
        Sprender_Close(sprender);
        
        // 16ms roughly translates to 60fps
        // Not perfect; you'll want to do better logic here
        SDL_Delay(16);
    }
    
    // Unload your texture
    Sprender_Texture_Destroy(sprender->fna3d.device, textureLogo);
    
    // Unload all data from the Sprender object
    Sprender_Destroy(sprender);
    
    return 0;
}
```

# How To Use

## Shaders

Shaders must be compiled by the `fxc.exe` tool found in the [DirectX June 2010 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=8109). Alternatively, you can use `efb.exe` from [Effect-Build](https://github.com/GlaireDaggers/Effect-Build/releases/tag/v0.1).

They should be written in HLSL, regardless of the platform you intended to support. FNA3D contains a library called MojoShader that takes care of cross-compatibility.

# Thanks

Thanks to...
- The SDL developers
- The FNA developers; especially cosmonaut, who helped with getting FNA3D working & the API design
