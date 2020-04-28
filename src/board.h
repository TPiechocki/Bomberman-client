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
    SDL_Texture *outsideWallTexture;
    SDL_Texture *iceBlockTexture;
    SDL_Rect outsideWalls[4];
    SDL_Rect iceBlocks[36];
} Board;

void LoadBoard(SDL_Window *window, SDL_Renderer *renderer, Board *board);
#endif //BOMBERMAN_CLIENT_BOARD_H