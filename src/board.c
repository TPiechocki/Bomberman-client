//
// Created by pioter on 21.04.2020.
//
#include "board.h"

void initBoard(Board* board, SDL_Window* window){
    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int board_length = windowHeight * 9 / 10;
    board->size = 11; // Board 11x11
    // converting the size to number dividable by size of board
    int arena_length = (int)(board_length / board->size) * board->size;
    int pixel_difference = board_length - arena_length;

    board->start_x = windowWidth / 2 - windowHeight * 9 / 20 + pixel_difference / 2;
    board->start_y = windowHeight / 20 + pixel_difference / 2;
    board->end_x = windowWidth / 2 + windowHeight * 9 / 20 - pixel_difference / 2;
    board->end_y = windowHeight * 19 / 20 - pixel_difference / 2;
    board->length = board->end_x - board->start_x;
}

void loadBoard(SDL_Window *window, SDL_Renderer *renderer, Board *board)
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
    // Górna i dolna ściana na 1/20 wysokości ekranu
    // Prawa i lewa tak żeby arena była kwadratowa
    //Gorna sciana
    board->outsideWalls[0].w = windowWidth;
    board->outsideWalls[0].h = board->start_y;
    board->outsideWalls[0].x = 0;
    board->outsideWalls[0].y = 0;

    //Dolna sciana
    board->outsideWalls[3].w = windowWidth;
    board->outsideWalls[3].h = board->start_y;
    board->outsideWalls[3].x = 0;
    board->outsideWalls[3].y = board->end_y;

    //Lewa sciana
    board->outsideWalls[1].w = board->start_x;
    board->outsideWalls[1].h = windowHeight;
    board->outsideWalls[1].x = 0;
    board->outsideWalls[1].y = 0;

    //Prawa sciana
    board->outsideWalls[2].w = board->start_x;
    board->outsideWalls[2].h = windowHeight;
    board->outsideWalls[2].x = board->end_x;
    board->outsideWalls[2].y = 0;

    SDL_FreeSurface(outsideWallSurface);

    //Ustawianie blokow na planszy
    board->iceBlockTexture = SDL_CreateTextureFromSurface(renderer, iceBlockSurface);

    for(int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            board->iceBlocks[6 * i + j].w = windowWidth * 4 / 5 / 13;
            board->iceBlocks[6 * i + j].h = windowHeight * 4 / 5 / 13;
            board->iceBlocks[6 * i + j].x = windowWidth / 10 + (2 * j + 1) * windowWidth * 4 / 5 / 13;
            board->iceBlocks[6 * i + j].y = windowHeight / 10 + (2 * i + 1) * windowHeight * 4 / 5 / 13;
        }
    }

    SDL_FreeSurface(iceBlockSurface);
}

void renderOutsideWalls(Board *board, SDL_Renderer *renderer) {
    for(int i = 0; i < 4; i++)
        SDL_RenderCopy(renderer, board->outsideWallTexture, NULL, &board->outsideWalls[i]);
}

void renderChessBoard(SDL_Renderer* renderer, Board* board){
    int tile_size = (board->length) / board->size; // 11 tiles per arena

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); // dark gray
    for(int i = 0; i < board->size; i++){
        for(int j = 0; j < board->size; j++){
            if((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)){
                SDL_Rect tile;
                tile.w = tile_size;
                tile.h = tile_size;
                tile.x = board->start_x + j*tile_size;
                tile.y = board->start_y + i*tile_size;
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 0xcc, 0xcc, 0xcc, 0xff); // base bg colour
}

