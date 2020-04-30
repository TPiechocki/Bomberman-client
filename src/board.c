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
    SDL_FreeSurface(outsideWallSurface);
    board->iceBlockTexture = SDL_CreateTextureFromSurface(renderer, iceBlockSurface);
    SDL_FreeSurface(iceBlockSurface);

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

    // Ice block init
    int tile_length = board->length / board->size;
    int index = 0;

    for(int i = 0; i < board->size; i++)
    {
        for (int j = 0; j < board->size; j++)
        {
            if(i % 2 == 1 && j % 2 == 1) {
                board->iceBlocks[index].w = tile_length;
                board->iceBlocks[index].h = tile_length;
                board->iceBlocks[index].x = j * tile_length + board->start_x;
                board->iceBlocks[index].y = i * tile_length + board->start_y;
                index++;
            }
        }
    }
}

void renderOutsideWalls(Board *board, SDL_Renderer *renderer) {
    for(int i = 0; i < 4; i++)
        SDL_RenderCopy(renderer, board->outsideWallTexture, NULL, &board->outsideWalls[i]);
}

void renderChessBoard(SDL_Renderer* renderer, Board* board){
    int tile_size = (board->length) / board->size; // 11 tiles per arena

    SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0x66, 0xff); // dark gray
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
    SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0x77, 0xff); // base bg colour
}

void renderIceBlocks(SDL_Renderer *renderer, Board *board) {
    for(int i = 0; i < 25; i++)
        SDL_RenderCopy(renderer, board->iceBlockTexture, NULL, &board->iceBlocks[i]);
}

void renderBoard(SDL_Renderer *renderer, Board *board) {
    // Render arena outside walls / colour outside of arena
    renderOutsideWalls(board, renderer);
    // Render Chessboard pattern
    renderChessBoard(renderer, board);
    // Render ice blocks
    renderIceBlocks(renderer, board);
}

void closeBoard(Board *board) {
    if(board->iceBlockTexture != NULL)
        SDL_DestroyTexture(board->iceBlockTexture);
    if(board->outsideWallTexture != NULL)
        SDL_DestroyTexture(board->outsideWallTexture);
}

