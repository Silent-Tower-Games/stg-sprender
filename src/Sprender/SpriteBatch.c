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
        spriteBatch.indices[j + 5] = x + 1;
    }
    
    return spriteBatch;
}

void Sprender_SpriteBatch_Begin(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Texture* texture
)
{
    assert(spriteBatch != NULL);
    assert(spriteBatch->opened == 0);
    assert(texture != NULL);
    
    spriteBatch->opened = 1;
    spriteBatch->texture = texture;
    spriteBatch->verticesThisBatch = 0;
    spriteBatch->indicesThisBatch = 0;
}

void Sprender_SpriteBatch_End(Sprender_SpriteBatch* spriteBatch)
{
    assert(spriteBatch != NULL);
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
    assert(spriteBatch != NULL);
    assert(spriteBatch->opened == 1);
    
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
            .x = destination.left,
            .y = destination.top,
            .z = depth,
            .u = source.left,
            .v = source.top,
        },
        // topRight
        (Sprender_Vertex){
            .color = color,
            .x = destination.right,
            .y = destination.top,
            .z = depth,
            .u = source.right,
            .v = source.top,
        },
        // bottomLeft
        (Sprender_Vertex){
            .color = color,
            .x = destination.left,
            .y = destination.bottom,
            .z = depth,
            .u = source.left,
            .v = source.bottom,
        },
        // bottomRight
        (Sprender_Vertex){
            .color = color,
            .x = destination.right,
            .y = destination.bottom,
            .z = depth,
            .u = source.right,
            .v = source.bottom,
        }
    );
}

char Sprender_SpriteBatch_StageFrame(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Int2D frame,
    Sprender_Float2D position,
    Sprender_Float2D scale,
    float depth,
    char flipped,
    uint32_t color
)
{
    const Sprender_Float2D tilesize ={
        .X = spriteBatch->texture->tilesize.X * 1.0f,
        .Y = spriteBatch->texture->tilesize.Y * 1.0f,
    };
    const float tilesizeHalfX = (tilesize.X * scale.X) / 2;
    const float tilesizeHalfY = (tilesize.Y * scale.Y) / 2;
    Sprender_Quad destination = {
        .left = position.X - tilesizeHalfX,
        .right = position.X + tilesizeHalfX,
        .top = position.Y - tilesizeHalfY,
        .bottom = position.Y + tilesizeHalfY,
    };
    
    Sprender_Float2D frameUse = {
        .X = spriteBatch->texture->border.X + (frame.X * (tilesize.X + spriteBatch->texture->padding.X)),
        .Y = spriteBatch->texture->border.Y + (frame.Y * (tilesize.Y + spriteBatch->texture->padding.Y)),
    };
    Sprender_Quad source = {
        .left = 1.0f / ((float)spriteBatch->texture->size.X / frameUse.X),
        .right = 1.0f / ((float)spriteBatch->texture->size.X / (frameUse.X + tilesize.X)),
        .top = 1.0f / ((float)spriteBatch->texture->size.Y / frameUse.Y),
        .bottom = 1.0f / ((float)spriteBatch->texture->size.Y / (frameUse.Y + tilesize.Y)),
    };
    
    // If flipped
    if(flipped != 0)
    {
        float hold;
        
        // If flipped X
        if(flipped & SPRENDER_SPRITEBATCH_FLIP_X)
        {
            hold = source.right;
            source.right = source.left;
            source.left = hold;
        }
        
        // If flipped Y
        if(flipped & SPRENDER_SPRITEBATCH_FLIP_Y)
        {
            hold = source.bottom;
            source.bottom = source.top;
            source.top = hold;
        }
    }
    
    return Sprender_SpriteBatch_StageQuad(
        spriteBatch,
        source,
        destination,
        depth,
        color
    );
}

char Sprender_SpriteBatch_StageRegion(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Int2D sourcePosition,
    Sprender_Int2D sourceSize,
    Sprender_Float2D destinationPosition,
    Sprender_Float2D destinationSize,
    float depth,
    char flipped,
    uint32_t color
)
{
    Sprender_Int2D sizeHalf = {
        .X = destinationSize.X / 2,
        .Y = destinationSize.Y / 2,
    };
    Sprender_Quad destination = {
        .left = destinationPosition.X - sizeHalf.X,
        .right = destinationPosition.X + sizeHalf.X,
        .top = destinationPosition.Y - sizeHalf.Y,
        .bottom = destinationPosition.Y + sizeHalf.Y,
    };
    
    Sprender_Quad source = {
        .left = 1.0f / ((float)spriteBatch->texture->size.X / sourcePosition.X),
        .right = 1.0f / ((float)spriteBatch->texture->size.X / (sourcePosition.X + sourceSize.X)),
        .top = 1.0f / ((float)spriteBatch->texture->size.Y / sourcePosition.Y),
        .bottom = 1.0f / ((float)spriteBatch->texture->size.Y / (sourcePosition.Y + sourceSize.Y)),
    };
    
     // If flipped
    if(flipped != 0)
    {
        float hold;
        
        // If flipped X
        if(flipped & 1)
        {
            hold = source.right;
            source.right = source.left;
            source.left = hold;
        }
        
        // If flipped Y
        if(flipped & 2)
        {
            hold = source.bottom;
            source.bottom = source.top;
            source.top = hold;
        }
    }
    
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
    assert(spriteBatch != NULL);
    
    free(spriteBatch->vertices);
    free(spriteBatch->indices);
}
