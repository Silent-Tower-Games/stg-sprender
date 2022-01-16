#include <assert.h>
#include <FNA3D.h>
#include <math.h>
#include "Int2D.h"
#include "Sprender.h"
#include "Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"

static void Sprender_FNA3D_SetValues(Sprender_FNA3D* fna3d);

Sprender* Sprender_Create(
    char* windowTitle,
    Sprender_Int2D windowSize,
    Sprender_Int2D resolution,
    char* spriteEffectShaderFilename,
    char* driver,
    int maxSprites,
    char vsync,
    Uint32 flags
)
{
    assert(windowTitle != NULL);

    SDL_Init(SDL_INIT_VIDEO | flags);
    
    Sprender* sprender = calloc(1, sizeof(Sprender));
    sprender->maxSprites = maxSprites;
    sprender->resolution = resolution;
    
    if(driver != NULL)
    {
        SDL_SetHint("FNA3D_FORCE_DRIVER", driver);
    }

    int FNA3D_Window_Attributes = FNA3D_PrepareWindowAttributes();
    assert(FNA3D_Window_Attributes != 0);
    
    sprender->window = SDL_CreateWindow(
        windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowSize.X,
        windowSize.Y,
        FNA3D_Window_Attributes
    );
    
    Sprender_SetPresentation(sprender, windowSize, 0, vsync);
    
    sprender->fna3d.indexBuffer = FNA3D_GenIndexBuffer(
        sprender->fna3d.device,
        1,
        FNA3D_BUFFERUSAGE_WRITEONLY,
        sprender->maxSprites * 6 * sizeof(int)
    );
    sprender->fna3d.vertexBufferBinding.vertexBuffer = FNA3D_GenVertexBuffer(
        sprender->fna3d.device,
        1,
        FNA3D_BUFFERUSAGE_WRITEONLY,
        sprender->maxSprites * 4 * sizeof(Sprender_Vertex) // Maximum number of sprites * 6 vertices per sprite * bytes per vertex
    );
    
    // Initialize FNA3D
    Sprender_FNA3D_SetValues(&sprender->fna3d);
    
    // Create SpriteBatch
    sprender->spriteBatch = Sprender_SpriteBatch_Create(sprender->maxSprites);
    
    // Load SpriteEffect shader
    sprender->shaderSpriteEffect = Sprender_Shader_Load(sprender->fna3d.device, spriteEffectShaderFilename, NULL);
    
    return sprender;
}

void Sprender_SetPresentation(Sprender* sprender, Sprender_Int2D windowSize, char fullscreen, char vsync)
{
    assert(sprender != NULL);
    
    sprender->vsync = vsync;
    
    // FNA3D create device
    FNA3D_PresentationParameters presentationParameters;
    memset(&presentationParameters, 0, sizeof(presentationParameters));
    presentationParameters.backBufferWidth = windowSize.X;
    presentationParameters.backBufferHeight = windowSize.Y;
    presentationParameters.deviceWindowHandle = sprender->window;
    presentationParameters.backBufferFormat = FNA3D_SURFACEFORMAT_COLOR;
    presentationParameters.presentationInterval = vsync ? FNA3D_PRESENTINTERVAL_ONE : FNA3D_PRESENTINTERVAL_IMMEDIATE;
    presentationParameters.depthStencilFormat = FNA3D_DEPTHFORMAT_D16;
    sprender->fna3d.presentationParameters = presentationParameters;
    
    SDL_SetWindowFullscreen(
        sprender->window,
        fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
    );
    
    if(sprender->fna3d.device == NULL)
    {
        sprender->fna3d.device = FNA3D_CreateDevice(&sprender->fna3d.presentationParameters, 0);
    }
    else
    {
        FNA3D_ResetBackbuffer(sprender->fna3d.device, &presentationParameters);
        
        SDL_SetWindowSize(sprender->window, windowSize.X, windowSize.Y);
        
        SDL_SetWindowPosition(sprender->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
    
    // Create default render mode
    Sprender_RenderMode_Destroy(sprender->fna3d.device, &sprender->defaultRenderMode);
    const Sprender_Int2D resolutionOffBy = {
        .X = windowSize.X % sprender->resolution.X,
        .Y = windowSize.Y % sprender->resolution.Y,
    };
    sprender->defaultRenderMode = Sprender_RenderMode_Create(
        sprender->fna3d.device,
        (Sprender_Int2D){ windowSize.X - resolutionOffBy.X, windowSize.Y - resolutionOffBy.Y, },
        (Sprender_Int2D){ resolutionOffBy.X / 2, resolutionOffBy.Y / 2, },
        (FNA3D_Vec4){ 0, 0, 0, 1, },
        0
    );
    sprender->defaultRenderMode.camera.zoom.X = fmin(((float)windowSize.X / sprender->resolution.X), ((float)windowSize.Y / sprender->resolution.Y));
    sprender->defaultRenderMode.camera.zoom.Y = sprender->defaultRenderMode.camera.zoom.X;
}

void Sprender_Load(
    Sprender* sprender,
    Sprender_RenderMode* renderMode,
    Sprender_Shader* shaders,
    int shadersCount
)
{
    assert(sprender != NULL);
    assert(shadersCount <= 0 || shaders != NULL);
    
    if(renderMode == NULL)
    {
        renderMode = &sprender->defaultRenderMode;
    }
    
    if(renderMode->renderTarget.texture == NULL)
    {
        FNA3D_SetRenderTargets(
            sprender->fna3d.device,
            NULL,
            0,
            renderMode->depthRenderbuffer,
            FNA3D_DEPTHFORMAT_D16,
            0
        );
    }
    else
    {
        FNA3D_SetRenderTargets(
            sprender->fna3d.device,
            &renderMode->renderTarget,
            1,
            renderMode->depthRenderbuffer,
            FNA3D_DEPTHFORMAT_D16,
            0
        );
    }
    
    FNA3D_SetViewport(sprender->fna3d.device, &renderMode->viewport);
    
    FNA3D_Clear(
        sprender->fna3d.device,
        FNA3D_CLEAROPTIONS_TARGET,
        &renderMode->bgColor,
        0,
        0
    );
    FNA3D_Clear(
        sprender->fna3d.device,
        FNA3D_CLEAROPTIONS_DEPTHBUFFER,
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
    
    for(int i = 0; i < shadersCount; i++)
    {
        if(shaders[i].callable != NULL && !shaders[i].callable(&shaders[i]))
        {
            continue;
        }
        
        FNA3D_ApplyEffect(
            sprender->fna3d.device,
            shaders[i].effect,
            0,
            &stateChanges
        );
    }
}

void Sprender_RenderSprites(Sprender* sprender)
{
    assert(sprender != NULL);
    
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
    FNA3D_SetIndexBufferData(
        sprender->fna3d.device,
        sprender->fna3d.indexBuffer,
        0,
        sprender->spriteBatch.indices,
        sizeof(uint32_t) * sprender->spriteBatch.indicesThisBatch,
        FNA3D_SETDATAOPTIONS_DISCARD
    );
    FNA3D_ApplyVertexBufferBindings(
        sprender->fna3d.device,
        &sprender->fna3d.vertexBufferBinding,
        1,
        1,
        0
    );
    
    FNA3D_VerifySampler(
        sprender->fna3d.device,
        0,
        sprender->spriteBatch.texture->asset,
        &sprender->fna3d.samplerState
    );
    
    FNA3D_DrawIndexedPrimitives(
        sprender->fna3d.device,
        FNA3D_PRIMITIVETYPE_TRIANGLELIST,
        0,
        0,
        sprender->spriteBatch.verticesThisBatch,
        0,
        sprender->spriteBatch.verticesThisBatch / 2,
        sprender->fna3d.indexBuffer,
        FNA3D_INDEXELEMENTSIZE_32BIT
    );
}

void Sprender_Close(Sprender* sprender)
{
    assert(sprender != NULL);
    
    FNA3D_SwapBuffers(
        sprender->fna3d.device,
        NULL,
        NULL,
        sprender->window
    );
}

void Sprender_Destroy(Sprender* sprender)
{
    assert(sprender != NULL);
    
    Sprender_RenderMode_Destroy(sprender->fna3d.device, &sprender->defaultRenderMode);
    
    FNA3D_AddDisposeEffect(sprender->fna3d.device, sprender->shaderSpriteEffect.effect);
    FNA3D_AddDisposeIndexBuffer(sprender->fna3d.device, sprender->fna3d.indexBuffer);
    FNA3D_AddDisposeVertexBuffer(sprender->fna3d.device, sprender->fna3d.vertexBufferBinding.vertexBuffer);
    FNA3D_DestroyDevice(sprender->fna3d.device);
    
    Sprender_SpriteBatch_Destroy(&sprender->spriteBatch);
    
    SDL_DestroyWindow(sprender->window);
    
    free(sprender);
    
    SDL_Quit();
}

static void Sprender_FNA3D_SetValues(Sprender_FNA3D* fna3d)
{
    assert(fna3d != NULL);
    
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
    vertexElements[0].vertexElementFormat = FNA3D_VERTEXELEMENTFORMAT_VECTOR3;
    vertexElements[0].vertexElementUsage = FNA3D_VERTEXELEMENTUSAGE_POSITION;
    vertexElements[1].offset = sizeof(float) * 3;
    vertexElements[1].usageIndex = 0;
    vertexElements[1].vertexElementFormat = FNA3D_VERTEXELEMENTFORMAT_VECTOR2;
    vertexElements[1].vertexElementUsage = FNA3D_VERTEXELEMENTUSAGE_TEXTURECOORDINATE;
    vertexElements[2].offset = sizeof(float) * 5;
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
    depthStencilState.depthBufferEnable = 1;
    depthStencilState.depthBufferFunction = FNA3D_COMPAREFUNCTION_GREATEREQUAL;
    depthStencilState.stencilEnable = 1;
    depthStencilState.depthBufferWriteEnable = 1;
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
