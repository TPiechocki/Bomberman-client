//
// Created by pioter on 21.04.2020.
//
#include "board.h"

void LoadBoard(SDL_Window *window, SDL_Renderer *renderer, Board *board)
{
    SDL_Surface *outsideWallSurface= IMG_Load(OUTSIDE_WALL_SPRITE_PATH);
    SDL_Surface *iceBlockSurface= IMG_Load(ICE_WALL_SPRITE_PATH);
    if(outsideWallSurface == NULL || iceBlockSurface== NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }
    board->outsideWallTexture = SDL_CreateTextureFromSurface(renderer, outsideWallSurface);

    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    //Ustawianie zewnetrznych scian
    //Kazda szeroka na 1/10 wymiaru ekranu
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

    SDL_FreeSurface(outsideWallSurface);

    //Ustawianie blokow na planszy
    board->iceBlockTexture = SDL_CreateTextureFromSurface(renderer, iceBlockSurface);

    for(int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            board->iceBlocks[6 * i + j].w = windowWidth * 4 / 5 / 13;
            board->iceBlocks[6 * i + j].h = windowHeight * 4 / 5 / 13;
            board->iceBlocks[6 * i + j].x = windowWidth / 10 + (2 * j + 1) * windowHeight * 4 / 5 / 13;
            board->iceBlocks[6 * i + j].y = windowHeight / 10 + (2 * i + 1) * windowHeight * 4 / 5 / 13;
        }
    }

    SDL_FreeSurface(iceBlockSurface);
}
