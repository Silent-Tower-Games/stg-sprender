#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <FNA3D.h>
#include "Shader.h"

Sprender_Shader Sprender_Shader_Create(
    FNA3D_Device* device,
    uint8_t* code,
    uint32_t codeSize,
    char (*callable)()
)
{
    assert(device != NULL);
    assert(code != NULL);
    
    Sprender_Shader shader;
    memset(&shader, 0, sizeof(shader));
    
    shader.callable = callable;
    
    FNA3D_CreateEffect(
        device,
        code,
        codeSize,
        &shader.effect,
        &shader.data
    );
    
    return shader;
}

Sprender_Shader Sprender_Shader_Load(
    FNA3D_Device* device,
    char* filename,
    char (*callable)(Sprender_Shader*)
)
{
    assert(device != NULL);
    assert(filename != NULL);
    
    FILE* effectFile = fopen(filename, "rb");
    
    assert(effectFile != NULL);
    
    fseek(effectFile, 0, SEEK_END);
    uint32_t codeSize = ftell(effectFile);
    fseek(effectFile, 0, SEEK_SET);
    uint8_t* code = malloc(codeSize);
    fread(code, 1, codeSize, effectFile);
    fclose(effectFile);
    
    return Sprender_Shader_Create(device, code, codeSize, callable);
}

MOJOSHADER_effectParam* Sprender_Shader_ParamGet(Sprender_Shader* shader, const char* key)
{
    assert(shader != NULL);
    assert(key != NULL);
    
    for(int i = 0; i < shader->data->param_count; i++)
    {
        if(strcmp(key, shader->data->params[i].value.name) == 0)
        {
            return &shader->data->params[i];
        }
    }
    
    return NULL;
}

char Sprender_Shader_ParamCopy(Sprender_Shader* shader, const char* key, void* payload, size_t size)
{
    assert(shader != NULL);
    assert(key != NULL);
    assert(payload != NULL);
    
    MOJOSHADER_effectParam* param = Sprender_Shader_ParamGet(shader, key);
    
    if(param == NULL)
    {
        return 0;
    }
    
    memcpy(param->value.values, payload, size);
    
    return 1;
}

void Sprender_Shader_Free(FNA3D_Device* device, Sprender_Shader* shader)
{
    assert(device != NULL);
    assert(shader != NULL);
    
    FNA3D_AddDisposeEffect(device, shader->effect);
}
