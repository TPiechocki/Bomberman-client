//
// Created by pioter on 21.04.2020.
//

#ifndef BOMBERMAN_CLIENT_BOARD_H
#define BOMBERMAN_CLIENT_BOARD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>

#define OUTSIDE_WALL_SPRITE_PATH "./../sprites/Wall.png"
#define ICE_WALL_SPRITE_PATH "./../sprites/Ice_block.png"
#define BREAKABLE_ICE_BLOCK_SPRITE_PATH "./../sprites/Breakable_ice_block.jpg"


typedef struct board_struct{
    int start_x; // starting x pixel of arena
    int start_y; // starting y pixel of arena
    int end_x; // ending x pixel of arena
    int end_y; // ending y pixel of arena
    int size; // size of arena in tiles
    int length; // length of whole arena in pixels
    int tile_length; // length of single tile in pixel
    SDL_Texture *outsideWallTexture; // texture of outside wall
    SDL_Texture *iceBlockTexture; // texture of unbreakable walls
    SDL_Texture *breakableIceBlockTexture; // texture of breakable walls
    SDL_Rect outsideWalls[4]; // bounding boxes of outside walls
    SDL_Rect iceBlocks[25]; // bounding boxes of unbreakable walls
    SDL_Rect *breakableIceBlocks[121]; // bounding boxes of breakable walls
    int iceBlocksCount; // amount of unbreakable walls
    int breakableIceBlocksCount; // amount of breakable walls
    int startGame; // signal to start the game
} Board;

Board* board; // board instance handle
pthread_mutex_t board_lock; // board mutex

/**
 * Initializes board structure and points it at the handle.
 * Mutex initialization inside.
 * @param window - window handle
 */
void initBoard(SDL_Window* window);

/**
 * Loads textures and resources for board rendering.
 * @param window - window handle
 * @param renderer - renderer handle
 */
void loadBoard(SDL_Window *window, SDL_Renderer *renderer);
/**
 * Sets bounding boxes of breakable walls to specified tiles in start array.
 * @param start - array of all tiles; 0 - no wall on tile; 1 - wall on tile
 */
void loadBreakable(char* start);

/**
 * Renders board and all blocks.
 * @param renderer - renderer handle
 */
void renderBoard(SDL_Renderer* renderer);

/**
 * Renders outside walls.
 * @param renderer - renderer handle
 */
void renderOutsideWalls(SDL_Renderer* renderer);
/**
 * Renders chessboard pattern on arena.
 * @param renderer - renderer handle
 */
void renderChessBoard(SDL_Renderer* renderer);
/**
 * Renders breakable and unbreakable walls.
 * @param renderer - renderer handle
 */
void renderIceBlocks(SDL_Renderer* renderer);

/**
 * Destroys breakable wall at given tile and frees linked resources.
 * @param index - tile number of block to be destroyed
 */
void destroyBreakableIceBlock(int index);

/**
 * Frees resources and destroys board mutex.
 */
void closeBoard();

#endif //BOMBERMAN_CLIENT_BOARD_H
