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
    int maxIndices;
    int verticesThisBatch;
    int indicesThisBatch;
    Sprender_Texture* texture;
    Sprender_Vertex* vertices;
    uint32_t* indices;
} Sprender_SpriteBatch;

/**
 * @brief Create a new SpriteBatch.
 * 
 * @param maxSprites maximum number of sprites
 * @return Sprender_SpriteBatch new SpriteBatch
 */
Sprender_SpriteBatch Sprender_SpriteBatch_Create(int maxSprites);

/**
 * @brief Begin staging vertices for the given texture.
 * 
 * @param spriteBatch your SpriteBatch
 * @param texture your texture
 */
void Sprender_SpriteBatch_Begin(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Texture* texture
);

/**
 * @brief End staging vertices for a given texture.
 * 
 * @param spriteBatch your SpriteBatch
 */
void Sprender_SpriteBatch_End(Sprender_SpriteBatch* spriteBatch);

/**
 * @brief Stage vertices in the given SpriteBatch.
 * 
 * @param spriteBatch your SpriteBatch
 * @param vertex0 top left position
 * @param vertex1 top right position
 * @param vertex2 bottom left position
 * @param vertex3 bottom right position
 * @return char whether or not the vertices were added successfully
 */
char Sprender_SpriteBatch_Stage(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Vertex vertex0,
    Sprender_Vertex vertex1,
    Sprender_Vertex vertex2,
    Sprender_Vertex vertex3
);

/**
 * @brief Stage a quad in the given SpriteBatch.
 * 
 * @param spriteBatch your SpriteBatch
 * @param source quad for the portion of the texture to be rendered
 * @param destination quad for where on the screen to render
 * @param depth depth value; will be turned into a 16-bit float
 * @param color color as an unsigned integer; 0xAABBGGRR
 * @return char whether or not the vertices were added successfully
 */
char Sprender_SpriteBatch_StageQuad(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Quad source,
    Sprender_Quad destination,
    float depth,
    uint32_t color
);

/**
 * @brief Stage a frame from a given texture in the given SpriteBatch, based on the texture's given tilesize.
 * 
 * @param spriteBatch your SpriteBatch
 * @param frame X & Y coordinates of a tile in the texture
 * @param position X & Y coordinates for where to draw the texture
 * @param scale size of the texture; default is 1.0fx1.0f
 * @param depth depth value; will be turned into a 16-bit float
 * @param color color as an unsigned integer; 0xAABBGGRR
 * @return char whether or not the vertices were added successfully
 */
char Sprender_SpriteBatch_StageFrame(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Int2D frame,
    Sprender_Float2D position,
    Sprender_Float2D scale,
    float depth,
    uint32_t color
);

/**
 * @brief Free all memory allocated by the SpriteBatch.
 * 
 * @param spriteBatch your SpriteBatch
 */
void Sprender_SpriteBatch_Destroy(Sprender_SpriteBatch* spriteBatch);
