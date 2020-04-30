//
// Created by pioter on 21.04.2020.
//

#ifndef BOMBERMAN_CLIENT_BOARD_H
#define BOMBERMAN_CLIENT_BOARD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define OUTSIDE_WALL_SPRITE_PATH "./../sprites/Wall.png"
#define ICE_WALL_SPRITE_PATH "./../sprites/Ice_block.png"

typedef struct Board{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int size;
    int length;
    SDL_Texture *outsideWallTexture;
    SDL_Texture *iceBlockTexture;
    SDL_Rect outsideWalls[4];
    SDL_Rect iceBlocks[25];
} Board;

void initBoard(Board* board, SDL_Window* window);

void loadBoard(SDL_Window *window, SDL_Renderer *renderer, Board *board);

void renderBoard(SDL_Renderer* renderer, Board* board);

void renderOutsideWalls(Board* board, SDL_Renderer* renderer);
void renderChessBoard(SDL_Renderer* renderer, Board* board);
void renderIceBlocks(SDL_Renderer* renderer, Board* board);

void closeBoard(Board* board);

#endif //BOMBERMAN_CLIENT_BOARD_H
