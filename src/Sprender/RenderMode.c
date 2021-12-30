#include <assert.h>
#include <string.h>
#include "RenderMode.h"
#include "Texture.h"

Sprender_RenderMode Sprender_RenderMode_Create(
    FNA3D_Device* device,
    Sprender_Int2D resolution,
    Sprender_Int2D position,
    FNA3D_Vec4 bgColor,
    char hasRenderTarget
)
{
    FNA3D_RenderTargetBinding renderTargetBinding;
    memset(&renderTargetBinding, 0, sizeof(renderTargetBinding));
    
    if(hasRenderTarget)
    {
        Sprender_Texture renderTargetTexture = Sprender_Texture_NewBlank(
            device,
            (FNA3D_Vec4){ 0, 0, 0, 1, },
            resolution.X,
            resolution.Y,
            4,
            1
        );
        
        renderTargetBinding.type = FNA3D_RENDERTARGET_TYPE_2D;
        renderTargetBinding.texture = renderTargetTexture.asset;
    }
    
    Sprender_RenderMode renderMode = {
        .bgColor = bgColor,
        .camera = Sprender_Camera_Create(resolution, (Sprender_Float2D){ 1.0f, 1.0f, }),
        .renderTarget = renderTargetBinding,
        .resolution = resolution,
        .viewport = (FNA3D_Viewport){
            .w = resolution.X,
            .h = resolution.Y,
            .x = position.X,
            .y = position.Y,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        },
    };
    
    return renderMode;
}

void Sprender_RenderMode_Load(Sprender* sprender, Sprender_RenderMode* renderMode)
{
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

void Sprender_RenderMode_Destroy(Sprender_RenderMode* renderMode)
{
    free(renderMode->renderTarget.texture);
}
