//
// Created by boardioter on 21.04.2020.
//
#include "board.h"

void LoadBoard(SDL_Window *window, SDL_Renderer *renderer, Board *board)
{
    board->s= IMG_Load(OUTSIDE_WALL_SPRITE_PATH);
    if(board->s == NULL) {
        printf("Blad boardrzy wczytywaniu plikow!");
        return;
    }
    board->outsideWallTexture = SDL_CreateTextureFromSurface(renderer, board->s);

    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    board->outsideWalls[0].w = windowWidth; //Gorna sciana
    board->outsideWalls[0].h = windowHeight / 10;
    board->outsideWalls[0].x = 0;
    board->outsideWalls[0].y = 0;

    board->outsideWalls[1].w = windowWidth / 10; //Lewa sciana
    board->outsideWalls[1].h = windowHeight;
    board->outsideWalls[1].x = 0;
    board->outsideWalls[1].y = 0;

    board->outsideWalls[2].w = windowWidth / 10; //Prawa sciana
    board->outsideWalls[2].h = windowHeight;
    board->outsideWalls[2].x = windowWidth * 9 / 10;
    board->outsideWalls[2].y = 0;

    board->outsideWalls[3].w = windowWidth; //Dolna sciana
    board->outsideWalls[3].h = windowHeight / 10;
    board->outsideWalls[3].x = 0;
    board->outsideWalls[3].y = windowHeight * 9 / 10;


}
