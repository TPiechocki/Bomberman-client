//
// Created by pioter on 21.04.2020.
//

#ifndef BOMBERMAN_CLIENT_BOARD_H
#define BOMBERMAN_CLIENT_BOARD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define OUTSIDE_WALL_SPRITE_PATH "./../sprites/Wall.png"
#define ICE_WALL_SPRITE_PATH "./../sprites/Ice_block.png"
#define BREAKABLE_ICE_BLOCK_SPRITE_PATH "./../sprites/Breakable_ice_block.jpg"

typedef struct board_struct{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int size;
    int length;
    int tile_length;
    SDL_Texture *outsideWallTexture;
    SDL_Texture *iceBlockTexture;
    SDL_Texture *breakableIceBlockTexture;
    SDL_Rect outsideWalls[4];
    SDL_Rect iceBlocks[25];
    SDL_Rect *breakableIceBlocks[90];
    int iceBlocksCount;
    int breakableIceBlocksCount;
    int enemy_count;
    int* bombs_placed;
} Board;

Board* board;

void initBoard(SDL_Window* window, int enemy_count);

void loadBoard(SDL_Window *window, SDL_Renderer *renderer);

void renderBoard(SDL_Renderer* renderer);

void renderOutsideWalls(SDL_Renderer* renderer);
void renderChessBoard(SDL_Renderer* renderer);
void renderIceBlocks(SDL_Renderer* renderer);

void destroyBreakableIceBlock(int index);

void closeBoard();

#endif //BOMBERMAN_CLIENT_BOARD_H
