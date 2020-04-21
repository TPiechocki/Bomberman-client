//
// Created by pioter on 21.04.2020.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef BOMBERMAN_CLIENT_BOARD_H
#define BOMBERMAN_CLIENT_BOARD_H

#define OUTSIDE_WALL_SPRITE_PATH "./../sprites/Blue_square.png"

typedef struct Board{
    SDL_Surface *s;
    SDL_Texture *outsideWallTexture;
    SDL_Rect outsideWalls[4];
} Board;

void LoadBoard(SDL_Window *window, SDL_Renderer *renderer, Board *board);
#endif //BOMBERMAN_CLIENT_BOARD_H
