#pragma once

#include <FNA3D.h>
#define MOJOSHADER_NO_VERSION_INCLUDE
#define MOJOSHADER_EFFECT_SUPPORT
#include <mojoshader.h>
#include <mojoshader_effects.h>

typedef struct Sprender_Shader
{
    FNA3D_Effect* effect;
    MOJOSHADER_effect* data;
    char (*callable)(void*);
} Sprender_Shader;

Sprender_Shader Sprender_Shader_Create(
    FNA3D_Device* device,
    uint8_t* code,
    uint32_t codeSize,
    char (*callable)()
);

Sprender_Shader Sprender_Shader_Load(
    FNA3D_Device* device,
    char* filename,
    char (*callable)(Sprender_Shader*)
);

MOJOSHADER_effectParam* Sprender_Shader_ParamGet(Sprender_Shader* shader, const char* key);

char Sprender_Shader_ParamCopy(Sprender_Shader* shader, const char* key, void* payload, size_t size);

void Sprender_Shader_Free(FNA3D_Device* device, Sprender_Shader* shader);
