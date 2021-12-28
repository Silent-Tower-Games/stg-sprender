#include <stdlib.h>
#include <FNA3D.h>
#include "SpriteBatch.h"

Sprender_SpriteBatch Sprender_SpriteBatch_Create(int maxVertices)
{
    Sprender_SpriteBatch spriteBatch = {
        .maxVertices = maxVertices,
        .verticesThisBatch = 0,
        .textures = malloc(sizeof(FNA3D_Texture*) * maxVertices),
        .vertices = malloc(sizeof(Sprender_Vertex) * maxVertices),
    };
    
    return spriteBatch;
}

char Sprender_SpriteBatch_Draw(
    Sprender_SpriteBatch* spriteBatch,
    FNA3D_Texture* texture,
    Sprender_Vertex vertex0,
    Sprender_Vertex vertex1,
    Sprender_Vertex vertex2,
    Sprender_Vertex vertex3,
    Sprender_Vertex vertex4,
    Sprender_Vertex vertex5
)
{
    int i = spriteBatch->verticesThisBatch;
    
    spriteBatch->textures[i + 0] = texture;
    spriteBatch->textures[i + 1] = texture;
    spriteBatch->textures[i + 2] = texture;
    spriteBatch->textures[i + 3] = texture;
    spriteBatch->textures[i + 4] = texture;
    spriteBatch->textures[i + 5] = texture;
    
    spriteBatch->vertices[i + 0] = vertex0;
    spriteBatch->vertices[i + 1] = vertex1;
    spriteBatch->vertices[i + 2] = vertex2;
    spriteBatch->vertices[i + 3] = vertex3;
    spriteBatch->vertices[i + 4] = vertex4;
    spriteBatch->vertices[i + 5] = vertex5;
    
    spriteBatch->verticesThisBatch += 6;
}

void Sprender_SpriteBatch_Destroy(Sprender_SpriteBatch* spriteBatch)
{
    free(spriteBatch->vertices);
}
