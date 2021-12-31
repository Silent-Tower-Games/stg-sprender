#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "RenderMode.h"

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
    
    Sprender_Texture renderTargetTexture;
    memset(&renderTargetTexture, 0, sizeof(renderTargetTexture));
    
    if(hasRenderTarget)
    {
        renderTargetTexture = Sprender_Texture_NewBlank(
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
        .renderTargetTexture = renderTargetTexture,
        .resolution = resolution,
        .viewport = (FNA3D_Viewport){
            .w = resolution.X,
            .h = resolution.Y,
            .x = position.X,
            .y = position.Y,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        },
        .depthRenderbuffer = FNA3D_GenDepthStencilRenderbuffer(
            device,
            resolution.X,
            resolution.Y,
            FNA3D_DEPTHFORMAT_D16,
            0
        ),
    };
    
    return renderMode;
}

void Sprender_RenderMode_Destroy(Sprender_RenderMode* renderMode)
{
    free(renderMode->renderTarget.texture);
}
