#pragma once

#include <FNA3D.h>
#include "Float2D.h"
#include "Int2D.h"
#include "Quad.h"
#include "Texture.h"
#include "Vertex.h"

#define SPRENDER_SPRITEBATCH_FLIP_X 1
#define SPRENDER_SPRITEBATCH_FLIP_Y 2

typedef struct Sprender_SpriteBatch
{
    char opened;
    int maxIndices;
    int verticesThisBatch;
    int indicesThisBatch;
    Sprender_Texture* texture;
    Sprender_Vertex* vertices;
    uint32_t* indices;
    FNA3D_Buffer* vertexBuffer;
    FNA3D_Buffer* indexBuffer;
    FNA3D_Device* device;
} Sprender_SpriteBatch;

/**
 * @brief Create a new SpriteBatch.
 * 
 * @param device FNA3D device
 * @param maxSprites maximum number of sprites
 * @return Sprender_SpriteBatch* pointer to new SpriteBatch
 */
Sprender_SpriteBatch* Sprender_SpriteBatch_Create(FNA3D_Device* device, int maxSprites, char useIndexBuffer);

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
 * @param flipped whether or not to flip the sprite on X and/or Y axis; bitwise
 * @param color color as an unsigned integer; 0xAABBGGRR
 * @return char whether or not the vertices were added successfully
 */
char Sprender_SpriteBatch_StageFrame(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Int2D frame,
    Sprender_Float2D position,
    Sprender_Float2D scale,
    float depth,
    char flipped,
    uint32_t color
);

/**
 * @brief Stage an arbitrary region of a given texture in the given SpriteBatch.
 * 
 * @param spriteBatch your SpriteBatch
 * @param sourcePosition X & Y coordinates where the texture's region begins
 * @param sourceSize width & height of the texture's region
 * @param destinationPosition X & Y coordinates for where to draw the texture
 * @param destinationSize width & height to render the texture at
 * @param depth depth value; will be turned into a 16-bit float
 * @param flipped whether or not to flip the sprite on X and/or Y axis; bitwise
 * @param color color as an unsigned integer; 0xAABBGGRR
 * @return char whether or not the vertices were added successfully
 */
char Sprender_SpriteBatch_StageRegion(
    Sprender_SpriteBatch* spriteBatch,
    Sprender_Int2D sourcePosition,
    Sprender_Int2D sourceSize,
    Sprender_Float2D destinationPosition,
    Sprender_Float2D destinationSize,
    float depth,
    char flipped,
    uint32_t color
);

/**
 * @brief Free all memory allocated by the SpriteBatch.
 * 
 * @param spriteBatch your SpriteBatch
 */
void Sprender_SpriteBatch_Destroy(Sprender_SpriteBatch* spriteBatch);
