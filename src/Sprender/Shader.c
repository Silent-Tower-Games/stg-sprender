#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <FNA3D.h>
#include "Shader.h"

Sprender_Shader Sprender_Shader_Create(FNA3D_Device* device, char* name, uint8_t* code, uint32_t codeSize)
{
    Sprender_Shader shader;
    memset(&shader, 0, sizeof(shader));
    
    shader.name = name;
    
    FNA3D_CreateEffect(
        device,
        code,
        codeSize,
        &shader.effect,
        &shader.data
    );
    
    return shader;
}

Sprender_Shader Sprender_Shader_Load(FNA3D_Device* device, char* name, char* filename)
{
    FILE* effectFile = fopen(filename, "rb");
    
    assert(effectFile != NULL);
    
    fseek(effectFile, 0, SEEK_END);
    uint32_t codeSize = ftell(effectFile);
    fseek(effectFile, 0, SEEK_SET);
    uint8_t* code = malloc(codeSize);
    fread(code, 1, codeSize, effectFile);
    fclose(effectFile);
    
    return Sprender_Shader_Create(device, name, code, codeSize);
}

MOJOSHADER_effectParam* Sprender_Shader_ParamGet(Sprender_Shader* shader, const char* key)
{
    for(int i = 0; i < shader->data->param_count; i++)
    {
        if(strcmp(key, shader->data->params[i].value.name) == 0)
        {
            return &shader->data->params[i];
        }
    }
    
    return NULL;
}
