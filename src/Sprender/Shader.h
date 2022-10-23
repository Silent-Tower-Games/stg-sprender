#pragma once

#define MOJOSHADER_NO_VERSION_INCLUDE
#define MOJOSHADER_EFFECT_SUPPORT
#include <mojoshader.h>
#define _INCL_MOJOSHADER_H_
#include <FNA3D.h>

typedef struct Sprender_Shader
{
    FNA3D_Effect* effect;
    MOJOSHADER_effect* data;
    // Optional function to run each time shader is attempted. Return 0 to not apply, 1 to apply
    char (*callable)(void*);
} Sprender_Shader;

/**
 * @brief Create a new shader from binary code.
 * 
 * @param device FNA3D device
 * @param code pointer to the shader's binary code
 * @param codeSize size of the shader's binary code
 * @param callable optional function to run each time shader is attemped. Return 0 to not apply, 1 to apply
 * @return Sprender_Shader new shader
 */
Sprender_Shader Sprender_Shader_Create(
    FNA3D_Device* device,
    uint8_t* code,
    uint32_t codeSize,
    char (*callable)(Sprender_Shader*)
);

/**
 * @brief Load a shader from a given filename.
 * 
 * @param device FNA3D device
 * @param filename filename of the shader. Probably a `.fxb` file
 * @param callable optional function to run each time shader is attemped. Return 0 to not apply, 1 to apply
 * @return Sprender_Shader newly loaded shader
 */
Sprender_Shader Sprender_Shader_Load(
    FNA3D_Device* device,
    const char* filename,
    char (*callable)(Sprender_Shader*)
);

/**
 * @brief Get a MOJOSHADER_effectParam object that represents the given shader's parameter.
 * 
 * @param shader shader to get parameter for
 * @param key name of the parameter
 * @return MOJOSHADER_effectParam* given parameter, or NULL
 */
MOJOSHADER_effectParam* Sprender_Shader_ParamGet(Sprender_Shader* shader, const char* key);

/**
 * @brief Set the value of the given shader's parameter.
 * 
 * @param shader shader to set parameter for
 * @param key name of the parameter
 * @param payload pointer to the parameter's new value
 * @param size size of the parameter's new value
 * @return char whether or not the parameter was successfully set
 */
char Sprender_Shader_ParamCopy(Sprender_Shader* shader, const char* key, void* payload, size_t size);

/**
 * @brief Free all memory allocated for the shader.
 * 
 * @param device FNA3D device
 * @param shader your shader
 */
void Sprender_Shader_Destroy(FNA3D_Device* device, Sprender_Shader* shader);
