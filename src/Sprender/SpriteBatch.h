#pragma once

#include <FNA3D.h>
#include "Float2D.h"
#include "Int2D.h"
#include "Quad.h"
#include "Texture.h"
#include "Vertex.h"

typedef struct Sprender_SpriteBatch
{
    char opened;
    int maxVertices;
    int verticesThisBatch;
    Sprender_Texture* texture;
    Sprender_Vertex* vertices;
} Sprender_SpriteBatch;

Sprender_SpriteBatch Sprender_SpriteBatch_Create(int maxSprites);

void Sprender_SpriteBatch_Begin(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Texture* texture
);

void Sprender_SpriteBatch_End(Sprender_SpriteBatch* spriteBatch);

char Sprender_SpriteBatch_Stage(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Vertex vertex0,
    Sprender_Vertex vertex1,
    Sprender_Vertex vertex2,
    Sprender_Vertex vertex3,
    Sprender_Vertex vertex4,
    Sprender_Vertex vertex5
);

char Sprender_SpriteBatch_StageQuad(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Quad source,
    Sprender_Quad destination,
    float depth,
    uint32_t color
);

char Sprender_SpriteBatch_StageFrame(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Int2D frame,
    Sprender_Float2D position,
    Sprender_Float2D scale,
    float depth,
    uint32_t color
);

void Sprender_SpriteBatch_Destroy(Sprender_SpriteBatch* spriteBatch);
