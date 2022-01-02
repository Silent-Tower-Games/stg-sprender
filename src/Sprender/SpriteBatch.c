#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <FNA3D.h>
#include "SpriteBatch.h"

Sprender_SpriteBatch Sprender_SpriteBatch_Create(int maxSprites)
{
    const int maxVertices = maxSprites * 4;
    const int maxIndices = maxSprites * 6;
    
    Sprender_SpriteBatch spriteBatch = {
        .opened = 0,
        .maxIndices = maxIndices,
        .verticesThisBatch = 0,
        .indicesThisBatch = 0,
        .texture = NULL,
        .vertices = malloc(sizeof(Sprender_Vertex) * maxVertices),
        .indices = malloc(sizeof(int) * maxIndices),
    };
    
    for(int i = 0; i < maxIndices / 6; i ++)
    {
        const int j = i * 6;
        const int x = i * 4;
        spriteBatch.indices[j + 0] = x + 0;
        spriteBatch.indices[j + 1] = x + 1;
        spriteBatch.indices[j + 2] = x + 2;
        spriteBatch.indices[j + 3] = x + 2;
        spriteBatch.indices[j + 4] = x + 3;
        spriteBatch.indices[j + 5] = x + 0;
    }
    
    return spriteBatch;
}

void Sprender_SpriteBatch_Begin(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Texture* texture
)
{
    assert(spriteBatch->opened == 0);
    assert(texture != NULL);
    spriteBatch->opened = 1;
    spriteBatch->texture = texture;
    spriteBatch->verticesThisBatch = 0;
    spriteBatch->indicesThisBatch = 0;
}

void Sprender_SpriteBatch_End(Sprender_SpriteBatch* spriteBatch)
{
    assert(spriteBatch->opened == 1);
    spriteBatch->opened = 0;
}

char Sprender_SpriteBatch_Stage(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Vertex vertex0,
    Sprender_Vertex vertex1,
    Sprender_Vertex vertex2,
    Sprender_Vertex vertex3
)
{
    assert(spriteBatch->opened == 1);
    
    // TODO: Sprite culling
    
    if(spriteBatch->indicesThisBatch + 6 > spriteBatch->maxIndices)
    {
        return 0;
    }
    
    int i = spriteBatch->verticesThisBatch;
    
    spriteBatch->vertices[i + 0] = vertex0;
    spriteBatch->vertices[i + 1] = vertex1;
    spriteBatch->vertices[i + 2] = vertex2;
    spriteBatch->vertices[i + 3] = vertex3;
    
    spriteBatch->verticesThisBatch += 4;
    spriteBatch->indicesThisBatch += 6;
    
    return 1;
}

char Sprender_SpriteBatch_StageQuad(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Quad source,
    Sprender_Quad destination,
    float depth,
    uint32_t color
)
{
    return Sprender_SpriteBatch_Stage(
        spriteBatch,
        // topLeft
        (Sprender_Vertex){
            .color = color,
            .x = destination.topLeft.X,
            .y = destination.topLeft.Y,
            .z = depth,
            .u = source.topLeft.X,
            .v = source.topLeft.Y,
        },
        // topRight
        (Sprender_Vertex){
            .color = color,
            .x = destination.topRight.X,
            .y = destination.topRight.Y,
            .z = depth,
            .u = source.topRight.X,
            .v = source.topRight.Y,
        },
        // bottomLeft
        (Sprender_Vertex){
            .color = color,
            .x = destination.bottomLeft.X,
            .y = destination.bottomLeft.Y,
            .z = depth,
            .u = source.bottomLeft.X,
            .v = source.bottomLeft.Y,
        },
        // bottomRight
        (Sprender_Vertex){
            .color = color,
            .x = destination.bottomRight.X,
            .y = destination.bottomRight.Y,
            .z = depth,
            .u = source.bottomRight.X,
            .v = source.bottomRight.Y,
        }
    );
}

char Sprender_SpriteBatch_StageFrame(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Int2D frame,
    Sprender_Float2D position,
    Sprender_Float2D scale,
    float depth,
    uint32_t color
)
{
    const int tilesizeHalfX = (spriteBatch->texture->tilesize.X / 2) * scale.X;
    const int tilesizeHalfY = (spriteBatch->texture->tilesize.Y / 2) * scale.Y;
    Sprender_Quad destination = {
        .topLeft = { position.X - (tilesizeHalfX), position.Y - (tilesizeHalfY), },
        .topRight = { position.X + (tilesizeHalfX), position.Y - (tilesizeHalfY), },
        .bottomLeft = { position.X - (tilesizeHalfX), position.Y + (tilesizeHalfY), },
        .bottomRight = { position.X + (tilesizeHalfX), position.Y + (tilesizeHalfY), },
    };
    
    const float tilesizeFloatX = 1.0f / ((float)spriteBatch->texture->size.X / (float)spriteBatch->texture->tilesize.X);
    const float tilesizeFloatY = 1.0f / ((float)spriteBatch->texture->size.Y / (float)spriteBatch->texture->tilesize.Y);
    const float frameX = frame.X * tilesizeFloatX;
    const float frameY = frame.Y * tilesizeFloatY;
    Sprender_Quad source = {
        .topLeft = { frameX, frameY, },
        .topRight = { frameX + tilesizeFloatX, frameY, },
        .bottomLeft = { frameX, frameY + tilesizeFloatY, },
        .bottomRight = { frameX + tilesizeFloatX, frameY + tilesizeFloatY, },
    };
    
    return Sprender_SpriteBatch_StageQuad(
        spriteBatch,
        source,
        destination,
        depth,
        color
    );
}

void Sprender_SpriteBatch_Destroy(Sprender_SpriteBatch* spriteBatch)
{
    free(spriteBatch->vertices);
}
