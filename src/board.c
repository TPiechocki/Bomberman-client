//
// Created by pioter on 21.04.2020.
//
#include "board.h"

void initBoard(SDL_Window* window, int enemy_count){
    board = (Board*)malloc(sizeof(Board));

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
    board->tile_length = board->length / board->size;
    board->iceBlocksCount = 25;
    board->breakableIceBlocksCount = 121;
    board->enemy_count = enemy_count;

    for(int i = 0; i < board->breakableIceBlocksCount; i++){
        board->breakableIceBlocks[i] = NULL;
    }


    board->startGame = 0;
    pthread_mutex_init(&board_lock, NULL);
}

void loadBoard(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_Surface *outsideWallSurface= IMG_Load(OUTSIDE_WALL_SPRITE_PATH);
    SDL_Surface *iceBlockSurface= IMG_Load(ICE_WALL_SPRITE_PATH);
    SDL_Surface  *breakableIceBlockSurface = IMG_Load(BREAKABLE_ICE_BLOCK_SPRITE_PATH);
    if(outsideWallSurface == NULL || iceBlockSurface == NULL || breakableIceBlockSurface == NULL){
        printf("Blad przy wczytywaniu plikow!");
        return;
    }
    board->outsideWallTexture = SDL_CreateTextureFromSurface(renderer, outsideWallSurface);
    SDL_FreeSurface(outsideWallSurface);
    board->iceBlockTexture = SDL_CreateTextureFromSurface(renderer, iceBlockSurface);
    SDL_FreeSurface(iceBlockSurface);
    board->breakableIceBlockTexture = SDL_CreateTextureFromSurface(renderer, breakableIceBlockSurface);
    SDL_FreeSurface(breakableIceBlockSurface);

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
    board->outsideWalls[1].w = windowWidth;
    board->outsideWalls[1].h = board->start_y;
    board->outsideWalls[1].x = 0;
    board->outsideWalls[1].y = board->end_y;

    //Lewa sciana
    board->outsideWalls[2].w = board->start_x;
    board->outsideWalls[2].h = windowHeight;
    board->outsideWalls[2].x = 0;
    board->outsideWalls[2].y = 0;

    //Prawa sciana
    board->outsideWalls[3].w = board->start_x;
    board->outsideWalls[3].h = windowHeight;
    board->outsideWalls[3].x = board->end_x;
    board->outsideWalls[3].y = 0;

    // Ice block init
    int tile_length = board->length / board->size;
    int index = 0;
    int ind = 0;
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

void loadBreakable(char* status){
    pthread_mutex_lock(&board_lock);
    for(int i = 0; i < 121; i++){
        if(((i % board->size) % 2) == 1 && ((i / board->size) %2) == 1)
            continue;
        if(status[i] && board->breakableIceBlocks[i] == NULL) {
            board->breakableIceBlocks[i] = (SDL_Rect *) malloc(sizeof(SDL_Rect));

            board->breakableIceBlocks[i]->w = 0.8 * board->tile_length;
            board->breakableIceBlocks[i]->h = 0.8 * board->tile_length;
            board->breakableIceBlocks[i]->x = (i % board->size) * board->tile_length + board->start_x + 0.1 * board->tile_length;
            board->breakableIceBlocks[i]->y = (i / board->size) * board->tile_length + board->start_y + 0.1 * board->tile_length;
        }
    }
    pthread_mutex_unlock(&board_lock);
}

void renderOutsideWalls(SDL_Renderer *renderer) {
    for(int i = 0; i < 4; i++)
        SDL_RenderCopy(renderer, board->outsideWallTexture, NULL, &board->outsideWalls[i]);
}

void renderChessBoard(SDL_Renderer* renderer){
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

void renderIceBlocks(SDL_Renderer *renderer) {
    for(int i = 0; i < 25; i++)
        SDL_RenderCopy(renderer, board->iceBlockTexture, NULL, &board->iceBlocks[i]);
    for(int i = 0; i < 121; i++)
        if(board->breakableIceBlocks[i] != NULL)
            SDL_RenderCopy(renderer, board->breakableIceBlockTexture, NULL, board->breakableIceBlocks[i]);
}

void renderBoard(SDL_Renderer *renderer) {
    pthread_mutex_lock(&board_lock);
    // Render arena outside walls / colour outside of arena
    renderOutsideWalls(renderer);
    // Render Chessboard pattern
    renderChessBoard(renderer);
    // Render ice blocks
    renderIceBlocks(renderer);
    pthread_mutex_unlock(&board_lock);
}

void destroyBreakableIceBlock(int index){
    free(board->breakableIceBlocks[index]);
    board->breakableIceBlocks[index] = NULL;
}

void closeBoard() {
    if(board->iceBlockTexture != NULL)
        SDL_DestroyTexture(board->iceBlockTexture);
    if(board->outsideWallTexture != NULL)
        SDL_DestroyTexture(board->outsideWallTexture);
    if(board->breakableIceBlockTexture != NULL)
        SDL_DestroyTexture(board->breakableIceBlockTexture);
    for(int i = 0; i < board->breakableIceBlocksCount; i++)
        if(board->breakableIceBlocks[i] != NULL)
            free(board->breakableIceBlocks[i]);
    free(board);
    pthread_mutex_destroy(&board_lock);
}

