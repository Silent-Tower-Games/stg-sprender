#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <FNA3D.h>
#include "SpriteBatch.h"

Sprender_SpriteBatch Sprender_SpriteBatch_Create(int maxVertices)
{
    Sprender_SpriteBatch spriteBatch = {
        .opened = 0,
        .maxVertices = maxVertices,
        .verticesThisBatch = 0,
        .textures = malloc(sizeof(FNA3D_Texture*) * maxVertices),
        .vertices = malloc(sizeof(Sprender_Vertex) * maxVertices),
    };
    
    return spriteBatch;
}

void Sprender_SpriteBatch_Begin(Sprender_SpriteBatch* spriteBatch)
{
    assert(spriteBatch->opened == 0);
    spriteBatch->opened = 1;
    spriteBatch->verticesThisBatch = 0;
}

void Sprender_SpriteBatch_End(Sprender_SpriteBatch* spriteBatch)
{
    assert(spriteBatch->opened == 1);
    spriteBatch->opened = 0;
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
    assert(spriteBatch->opened == 1);
    
    if(spriteBatch->verticesThisBatch + 6 > spriteBatch->maxVertices)
    {
        return 0;
    }
    
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
    
    return 1;
}

char Sprender_SpriteBatch_DrawQuad(
    Sprender_SpriteBatch* spriteBatch,
    FNA3D_Texture* texture,
    Sprender_Quad source,
    Sprender_Quad destination,
    uint32_t color
)
{
    return Sprender_SpriteBatch_Draw(
        spriteBatch,
        texture,
        // topLeft
        (Sprender_Vertex){
            .color = color,
            .x = destination.topLeft.X,
            .y = destination.topLeft.Y,
            .u = source.topLeft.X,
            .v = source.topLeft.Y,
        },
        // topRight
        (Sprender_Vertex){
            .color = color,
            .x = destination.topRight.X,
            .y = destination.topRight.Y,
            .u = source.topRight.X,
            .v = source.topRight.Y,
        },
        // bottomRight
        (Sprender_Vertex){
            .color = color,
            .x = destination.bottomRight.X,
            .y = destination.bottomRight.Y,
            .u = source.bottomRight.X,
            .v = source.bottomRight.Y,
        },
        // bottomRight
        (Sprender_Vertex){
            .color = color,
            .x = destination.bottomRight.X,
            .y = destination.bottomRight.Y,
            .u = source.bottomRight.X,
            .v = source.bottomRight.Y,
        },
        // bottomLeft
        (Sprender_Vertex){
            .color = color,
            .x = destination.bottomLeft.X,
            .y = destination.bottomLeft.Y,
            .u = source.bottomLeft.X,
            .v = source.bottomLeft.Y,
        },
        // topLeft
        (Sprender_Vertex){
            .color = color,
            .x = destination.topLeft.X,
            .y = destination.topLeft.Y,
            .u = source.topLeft.X,
            .v = source.topLeft.Y,
        }
    );
}

void Sprender_SpriteBatch_Destroy(Sprender_SpriteBatch* spriteBatch)
{
    free(spriteBatch->vertices);
}
