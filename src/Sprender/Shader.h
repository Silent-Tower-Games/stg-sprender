#pragma once

#include <FNA3D.h>

typedef struct Sprender_Shader
{
    char* name;
    FNA3D_Effect* effect;
    MOJOSHADER_effect* data;
} Sprender_Shader;

Sprender_Shader Sprender_Shader_Create(FNA3D_Device* device, char* name, uint8_t* code, uint32_t codeSize);

Sprender_Shader Sprender_Shader_Load(FNA3D_Device* device, char* name, char* filename);
