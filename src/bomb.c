//
// Created by ves on 06/05/2020.
//

#include "bomb.h"

void initBomb(Bomb *bomb) {
    bomb->tile = -1;
    bomb->placed = 0;
    bomb->exploded = 0;
    bomb->range = 2; // 2 tiles from bomb in each direction are affected
    bomb->timer = (Timer*)malloc(sizeof(Timer));
    initTimer(bomb->timer);
}

void loadBomb(Bomb *bomb, SDL_Renderer* renderer, Board* board, int tile) {
    SDL_Surface* bombSurface = IMG_Load("./../sprites/Black_square.png");
    SDL_Surface* exploSurface = IMG_Load("./../sprites/Explosion.jpg");
    if(bombSurface == NULL || exploSurface == NULL){
        fprintf(stderr, "Failed to load bomb sprites.\n");
        return;
    }
    bomb->bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    if(bomb->bombTexture == NULL)
        fprintf(stderr,"%s", SDL_GetError());
    SDL_FreeSurface(bombSurface);
    bomb->explosionTexture = SDL_CreateTextureFromSurface(renderer, exploSurface);
    SDL_FreeSurface(exploSurface);

    bomb->placed = 1;
    bomb->tile = tile;

    bomb->bombRect.w = board->length/ board->size / 2;
    bomb->bombRect.h = bomb->bombRect.w;
    bomb->bombRect.x = (tile % board->size) * board->tile_length + board->start_x + board->tile_length / 4;
    bomb->bombRect.y = (tile / board->size) * board->tile_length + board->start_y + board->tile_length / 4;
    startTimer(bomb->timer);
}

void explode(Bomb* bomb, Board* board){
    // Rects will go more in each side till they collide or till range
    SDL_Rect right;
    SDL_Rect left;
    SDL_Rect up;
    SDL_Rect down;
    int collision = 0;
    right.x = (bomb->tile % board->size) * board->tile_length + board->start_x;
    right.y = (bomb->tile / board->size) * board->tile_length + board->start_y;
    right.w = board->tile_length;
    right.h = board->tile_length;

    down.x = (bomb->tile % board->size) * board->tile_length + board->start_x;
    down.y = (bomb->tile / board->size) * board->tile_length + board->start_y;
    down.w = board->tile_length;
    down.h = board->tile_length;

    left.x = (bomb->tile % board->size) * board->tile_length + board->start_x;
    left.y = (bomb->tile / board->size) * board->tile_length + board->start_y;
    left.w = board->tile_length;
    left.h = board->tile_length;

    up.x = (bomb->tile % board->size) * board->tile_length + board->start_x;
    up.y = (bomb->tile / board->size) * board->tile_length + board->start_y;
    up.w = board->tile_length;
    up.h = board->tile_length;


    for(int i = 0; i < bomb->range; i++){
        right.w += board->tile_length;
        //right side collide with outside wall
        for(int j = 0; j < 4; j++){
            if(SDL_HasIntersection(&right, &board->outsideWalls[j])){
                right.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        //right side collide with ice block
        for(int j = 0; j < board->iceBlocksCount; j++){
            if(SDL_HasIntersection(&right, &board->iceBlocks[j])){
                right.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
    }
    collision = 0;

    for(int i = 0; i < bomb->range; i++){
        down.h += board->tile_length;
        //down side collide with outside wall
        for(int j = 0; j < 4; j++){
            if(SDL_HasIntersection(&down, &board->outsideWalls[j])){
                down.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        //down side collide with ice block
        for(int j = 0; j < board->iceBlocksCount; j++){
            if(SDL_HasIntersection(&down, &board->iceBlocks[j])){
                down.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
    }
    collision = 0;

    for(int i = 0; i < bomb->range; i++){
        up.y -= board->tile_length;
        up.h += board->tile_length;
        //upper side collide with outside wall
        for(int j = 0; j < 4; j++){
            if(SDL_HasIntersection(&up, &board->outsideWalls[j])){
                up.y += board->tile_length;
                up.h -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        //upper side collide with ice block
        for(int j = 0; j < board->iceBlocksCount; j++){
            if(SDL_HasIntersection(&up, &board->iceBlocks[j])){
                up.y += board->tile_length;
                up.h -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
    }

    collision = 0;
    for(int i = 0; i < bomb->range; i++){
        left.x -= board->tile_length;
        left.w += board->tile_length;
        //left side collide with outside wall
        for(int j = 0; j < 4; j++){
            if(SDL_HasIntersection(&left, &board->outsideWalls[j])){
                left.x += board->tile_length;
                left.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        //left side collide with ice block
        for(int j = 0; j < board->iceBlocksCount; j++){
            if(SDL_HasIntersection(&left, &board->iceBlocks[j])){
                left.x += board->tile_length;
                left.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
    }

    bomb->explosionRect[0] = right;
    bomb->explosionRect[1] = left;
    bomb->explosionRect[2] = up;
    bomb->explosionRect[3] = down;
    bomb->exploded = 1;
}

void renderBomb(Bomb* bomb, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, bomb->bombTexture, NULL, &bomb->bombRect);
}

void renderExplosion(Bomb* bomb, SDL_Renderer* renderer)
{
    for(int i = 0; i < 4; i++)
        SDL_RenderCopy(renderer, bomb->explosionTexture, NULL, &bomb->explosionRect[i]);
}

void closeBomb(Bomb *bomb) {
    SDL_DestroyTexture(bomb->bombTexture);
    SDL_DestroyTexture(bomb->explosionTexture);
    bomb->exploded = 0;
    free(bomb->timer);
}