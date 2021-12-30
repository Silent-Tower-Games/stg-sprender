#include <FNA3D.h>
#include "Sprender.h"
#include "Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"

static void Sprender_FNA3D_SetValues(Sprender_FNA3D* fna3d);

Sprender* Sprender_Create(
    char* windowTitle,
    int windowWidth,
    int windowHeight,
    char* driver,
    int maxSprites,
    Uint32 flags
)
{
    // SDL init & create window
    SDL_Init(SDL_INIT_VIDEO | flags);
    
    Sprender* sprender = calloc(1, sizeof(Sprender));
    sprender->maxSprites = maxSprites;
    
    if(driver != NULL)
    {
        SDL_SetHint("FNA3D_FORCE_DRIVER", driver);
    }
    
    sprender->window = SDL_CreateWindow(
        windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowWidth,
        windowHeight,
        FNA3D_PrepareWindowAttributes()
    );
    
    // FNA3D create device
    // TODO: Resizing window
    // TODO: Fullscreen
    FNA3D_PresentationParameters presentationParameters;
    memset(&presentationParameters, 0, sizeof(presentationParameters));
    presentationParameters.backBufferWidth = windowWidth;
    presentationParameters.backBufferHeight = windowHeight;
    presentationParameters.deviceWindowHandle = sprender->window;
    presentationParameters.backBufferFormat = FNA3D_SURFACEFORMAT_COLOR;
    presentationParameters.presentationInterval = FNA3D_PRESENTINTERVAL_IMMEDIATE; // vsync is _DEFAULT, not _IMMEDIATE
    sprender->fna3d.presentationParameters = presentationParameters;
    
    sprender->fna3d.device = FNA3D_CreateDevice(&sprender->fna3d.presentationParameters, 0);
    
    sprender->fna3d.vertexBufferBinding.vertexBuffer = FNA3D_GenVertexBuffer(
        sprender->fna3d.device,
        1,
        FNA3D_BUFFERUSAGE_WRITEONLY,
        sprender->maxSprites * 6 * sizeof(Sprender_Vertex) // Maximum number of sprites * 6 vertices per sprite * bytes per vertex
    );
    
    // Initialize FNA3D
    Sprender_FNA3D_SetValues(&sprender->fna3d);
    
    // Create SpriteBatch
    sprender->spriteBatch = Sprender_SpriteBatch_Create(sprender->maxSprites);
    
    // Create default render mode
    sprender->defaultRenderMode = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ windowWidth, windowHeight, },
        (Sprender_Int2D){ 0, 0, },
        (FNA3D_Vec4){ 0, 0, 0, 1, },
        0
    );
    
    return sprender;
}

void Sprender_Open(Sprender* sprender)
{
    assert(0);
}

void Sprender_Load_RenderMode(Sprender* sprender, Sprender_RenderMode* renderMode)
{
    if(renderMode == NULL)
    {
        renderMode = &sprender->defaultRenderMode;
    }
    
    if(renderMode->renderTarget.texture != NULL && sprender->renderedToWindow)
    {
        renderMode->viewport.y = sprender->fna3d.presentationParameters.backBufferHeight - renderMode->resolution.Y;
        sprender->renderedToWindow = 0;
    }
    else if(renderMode->renderTarget.texture == NULL)
    {
        sprender->renderedToWindow = 1;
    }
    
    FNA3D_SetViewport(sprender->fna3d.device, &renderMode->viewport);
    
    if(renderMode->renderTarget.texture == NULL)
    {
        FNA3D_SetRenderTargets(
            sprender->fna3d.device,
            NULL,
            0,
            NULL,
            FNA3D_DEPTHFORMAT_NONE,
            0
        );
    }
    else
    {
        FNA3D_SetRenderTargets(
            sprender->fna3d.device,
            &renderMode->renderTarget,
            1,
            NULL,
            FNA3D_DEPTHFORMAT_NONE,
            0
        );
    }
    
    FNA3D_Clear(
        sprender->fna3d.device,
        FNA3D_CLEAROPTIONS_TARGET,
        &renderMode->bgColor,
        0,
        0
    );
    
    MOJOSHADER_effectParam* shaderMatrix = Sprender_Shader_ParamGet(&sprender->shaderSpriteEffect, "MatrixTransform");
    assert(shaderMatrix != NULL);
    Sprender_Camera_LoadInto(&renderMode->camera, shaderMatrix->value.values);
    
    MOJOSHADER_effectStateChanges stateChanges;
    memset(&stateChanges, 0, sizeof(stateChanges));
    FNA3D_ApplyEffect(
        sprender->fna3d.device,
        sprender->shaderSpriteEffect.effect,
        0,
        &stateChanges
    );
}

void Sprender_RenderSprites(Sprender* sprender)
{
    FNA3D_SetVertexBufferData(
        sprender->fna3d.device,
        sprender->fna3d.vertexBufferBinding.vertexBuffer,
        0,
        sprender->spriteBatch.vertices,
        sizeof(Sprender_Vertex) * sprender->spriteBatch.verticesThisBatch,
        1,
        1,
        FNA3D_SETDATAOPTIONS_DISCARD
    );
    FNA3D_ApplyVertexBufferBindings(
        sprender->fna3d.device,
        &sprender->fna3d.vertexBufferBinding,
        1,
        0,
        0
    );
    
    FNA3D_Texture* thisTexture = NULL;
    int thisTextureStartsAt = 0;
    
    // FIXME: O(n^2)! Is there any solution?
    // Maybe textures shouldn't have a record each row, but just for each time it changes?
    // It would still be worst-case O(n^2) but at least it wouldn't be O(n^2) every time.
    // Can't set vertex buffer until we know how many vertices there are this frame, so it
    // must be O(n^2) worst-case.
    // FIXME: i += 6? i += 3?
    int t;
    for(int i = 0; i < sprender->spriteBatch.verticesThisBatch; i += 6)
    {
        t = i / 6;
        
        if(thisTexture == NULL || thisTexture != sprender->spriteBatch.textures[t])
        {
            // Do this only if we've already got vertices to draw
            if(i > 0)
            {
                FNA3D_DrawPrimitives(
                    sprender->fna3d.device,
                    FNA3D_PRIMITIVETYPE_TRIANGLELIST,
                    thisTextureStartsAt,
                    (i - thisTextureStartsAt) / 3
                );
                
                thisTextureStartsAt = i;
            }
            
            thisTexture = sprender->spriteBatch.textures[t];
            
            FNA3D_VerifySampler(
                sprender->fna3d.device,
                0,
                thisTexture,
                &sprender->fna3d.samplerState
            );
        }
    }
    
    FNA3D_DrawPrimitives(
        sprender->fna3d.device,
        FNA3D_PRIMITIVETYPE_TRIANGLELIST,
        thisTextureStartsAt,
        (sprender->spriteBatch.verticesThisBatch - thisTextureStartsAt) / 3
    );
}

void Sprender_Close(Sprender* sprender)
{
    FNA3D_SwapBuffers(
        sprender->fna3d.device,
        NULL,
        NULL,
        sprender->window
    );
}

void Sprender_Destroy(Sprender* sprender)
{
    Sprender_RenderMode_Destroy(&sprender->defaultRenderMode);
    
    // FNA3D Destroy
    FNA3D_AddDisposeVertexBuffer(sprender->fna3d.device, sprender->fna3d.vertexBufferBinding.vertexBuffer);
    FNA3D_DestroyDevice(sprender->fna3d.device);
    
    Sprender_SpriteBatch_Destroy(&sprender->spriteBatch);
    
    SDL_DestroyWindow(sprender->window);
    
    free(sprender);
}

static void Sprender_FNA3D_SetValues(Sprender_FNA3D* fna3d)
{
    // BlendState
    memset(&fna3d->blendState, 0, sizeof(fna3d->blendState));
    fna3d->blendState.alphaBlendFunction = FNA3D_BLENDFUNCTION_ADD;
    fna3d->blendState.alphaDestinationBlend = FNA3D_BLEND_INVERSESOURCEALPHA;
    fna3d->blendState.alphaSourceBlend = FNA3D_BLEND_ONE;
    FNA3D_Color blendFactor = { 0xff, 0xff, 0xff, 0xff };
    fna3d->blendState.blendFactor = blendFactor;
    fna3d->blendState.colorBlendFunction = FNA3D_BLENDFUNCTION_ADD;
    fna3d->blendState.colorDestinationBlend = FNA3D_BLEND_INVERSESOURCEALPHA;
    fna3d->blendState.colorSourceBlend = FNA3D_BLEND_SOURCEALPHA;
    fna3d->blendState.colorWriteEnable = FNA3D_COLORWRITECHANNELS_ALL;
    fna3d->blendState.colorWriteEnable1 = FNA3D_COLORWRITECHANNELS_ALL;
    fna3d->blendState.colorWriteEnable2 = FNA3D_COLORWRITECHANNELS_ALL;
    fna3d->blendState.colorWriteEnable3 = FNA3D_COLORWRITECHANNELS_ALL;
    fna3d->blendState.multiSampleMask = -1;
    FNA3D_SetBlendState(fna3d->device, &fna3d->blendState);
    
    // VertexElements
    FNA3D_VertexElement* vertexElements = fna3d->vertexElements;
    vertexElements[0].offset = 0;
    vertexElements[0].usageIndex = 0;
    vertexElements[0].vertexElementFormat = FNA3D_VERTEXELEMENTFORMAT_VECTOR2;
    vertexElements[0].vertexElementUsage = FNA3D_VERTEXELEMENTUSAGE_POSITION;
    vertexElements[1].offset = sizeof(float) * 2;
    vertexElements[1].usageIndex = 0;
    vertexElements[1].vertexElementFormat = FNA3D_VERTEXELEMENTFORMAT_VECTOR2;
    vertexElements[1].vertexElementUsage = FNA3D_VERTEXELEMENTUSAGE_TEXTURECOORDINATE;
    vertexElements[2].offset = sizeof(float) * 4;
    vertexElements[2].usageIndex = 0;
    vertexElements[2].vertexElementFormat = FNA3D_VERTEXELEMENTFORMAT_COLOR;
    vertexElements[2].vertexElementUsage = FNA3D_VERTEXELEMENTUSAGE_COLOR;
    
    FNA3D_VertexDeclaration vertexDeclaration;
    memset(&vertexDeclaration, 0, sizeof(vertexDeclaration));
    vertexDeclaration.elementCount = 3;
    vertexDeclaration.vertexStride = sizeof(Sprender_Vertex);
    vertexDeclaration.elements = vertexElements;
    fna3d->vertexBufferBinding.vertexDeclaration = vertexDeclaration;
    
    FNA3D_SamplerState samplerState;
    memset(&samplerState, 0, sizeof(samplerState));
    samplerState.addressU = FNA3D_TEXTUREADDRESSMODE_CLAMP;
    samplerState.addressV = FNA3D_TEXTUREADDRESSMODE_CLAMP;
    samplerState.addressW = FNA3D_TEXTUREADDRESSMODE_WRAP;
    samplerState.filter = FNA3D_TEXTUREFILTER_POINT;
    samplerState.maxAnisotropy = 4;
    samplerState.maxMipLevel = 0;
    samplerState.mipMapLevelOfDetailBias = 0;
    fna3d->samplerState = samplerState;
    
    FNA3D_DepthStencilState depthStencilState;
    memset(&depthStencilState, 0, sizeof(depthStencilState));
    depthStencilState.depthBufferEnable = 0;
    depthStencilState.stencilEnable = 0;
    FNA3D_SetDepthStencilState(fna3d->device, &depthStencilState);

    FNA3D_RasterizerState rasterizerState;
    rasterizerState.cullMode = FNA3D_CULLMODE_NONE;
    rasterizerState.fillMode = FNA3D_FILLMODE_SOLID;
    rasterizerState.depthBias = 0;
    rasterizerState.multiSampleAntiAlias = 1;
    rasterizerState.scissorTestEnable = 0;
    rasterizerState.slopeScaleDepthBias = 0;
    FNA3D_ApplyRasterizerState(fna3d->device, &rasterizerState);
}
