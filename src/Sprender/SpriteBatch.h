#pragma once

#include <FNA3D.h>
#include "Vertex.h"

typedef struct Sprender_SpriteBatch
{
    int maxVertices;
    int verticesThisBatch;
    FNA3D_Texture** textures;
    Sprender_Vertex* vertices;
} Sprender_SpriteBatch;

Sprender_SpriteBatch Sprender_SpriteBatch_Create(int maxVertices);

//

void Sprender_SpriteBatch_Destroy(Sprender_SpriteBatch* spriteBatch);
