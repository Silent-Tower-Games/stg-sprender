#include "Sprender.h"
#include "Vertex.h"

static void Sprender_FNA3D_SetValues(Sprender_FNA3D* fna3d);

Sprender* Sprender_Create(
    char* windowTitle,
    int windowWidth,
    int windowHeight,
    int resolutionWidth,
    int resolutionHeight,
    char* driver,
    Uint32 flags
)
{
    SDL_Init(SDL_INIT_VIDEO | flags);
    
    Sprender* sprender = malloc(sizeof(Sprender));
    
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
    
    return sprender;
}

void Sprender_Destroy(Sprender* sprender)
{
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
    vertexDeclaration.vertexStride = sizeof(Vertex);
    vertexDeclaration.elements = vertexElements;
    
    fna3d->vertexBufferBinding.vertexDeclaration = vertexDeclaration;
    
    //app->renderState.vertexBufferBinding.vertexBuffer = app->renderState.vertexBuffer;
    //app->renderState.vertexBufferBinding.vertexDeclaration = vertexDeclaration;
}
