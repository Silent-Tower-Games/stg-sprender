#pragma once

#include <FNA3D.h>
#define MOJOSHADER_NO_VERSION_INCLUDE
#define MOJOSHADER_EFFECT_SUPPORT
#include <mojoshader.h>
#include <mojoshader_effects.h>

typedef struct Sprender_Shader
{
    char* name;
    FNA3D_Effect* effect;
    MOJOSHADER_effect* data;
} Sprender_Shader;

Sprender_Shader Sprender_Shader_Create(FNA3D_Device* device, char* name, uint8_t* code, uint32_t codeSize);

Sprender_Shader Sprender_Shader_Load(FNA3D_Device* device, char* name, char* filename);

MOJOSHADER_effectParam* Sprender_Shader_ParamGet(Sprender_Shader* shader, const char* key);
