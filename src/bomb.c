//
// Created by ves on 06/05/2020.
//

#include "bomb.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
void initAllBombs(int count){
    bombs = (Bomb**)malloc(sizeof(Bomb*)*count);
    for(int i = 0; i < count; i++){
        bombs[i] = (Bomb*)malloc(sizeof(Bomb));
    }
    pthread_mutex_init(&bombs_lock, NULL);
}

void initBomb(Bomb *bomb) {
    //bomb = (Bomb*)malloc(sizeof(Bomb));
    
    bomb->underPlayer = 0;
    bomb->explodeTick = -1;
    bomb->tile = -1;
    bomb->placed = 0;
    bomb->exploded = 0;
    bomb->range = 2; // 2 tiles from bomb in each direction are affected
}

void loadBomb(Bomb *bomb, SDL_Renderer* renderer) {
    SDL_Surface* bombSurface = IMG_Load("./../sprites/Black_square.png");
    SDL_Surface* exploHorSurface = IMG_Load("./../sprites/beam_hor.png");
    SDL_Surface* exploVerSurface = IMG_Load("./../sprites/beam_ver.png");
    SDL_Surface* centerSurface = IMG_Load("./../sprites/beam_center.png");
    if(bombSurface == NULL || exploHorSurface == NULL || exploVerSurface == NULL || centerSurface == NULL){
        fprintf(stderr, "Failed to load bomb sprites.\n");
        return;
    }
    bomb->bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    if(bomb->bombTexture == NULL)
        fprintf(stderr,"%s", SDL_GetError());
    SDL_FreeSurface(bombSurface);
    bomb->explHorTexture = SDL_CreateTextureFromSurface(renderer, exploHorSurface);
    if(bomb->explHorTexture == NULL)
        fprintf(stderr,"%s", SDL_GetError());
    SDL_FreeSurface(exploHorSurface);
    bomb->explVerTexture = SDL_CreateTextureFromSurface(renderer, exploVerSurface);
    if(bomb->explVerTexture == NULL)
        fprintf(stderr,"%s", SDL_GetError());
    SDL_FreeSurface(exploVerSurface);
    bomb->centerTexture = SDL_CreateTextureFromSurface(renderer, centerSurface);
    if(bomb->centerTexture == NULL)
        fprintf(stderr,"%s", SDL_GetError());
    SDL_FreeSurface(centerSurface);
}

void placeBomb(Bomb *bomb, Board* board, int tile, int explodeTick, int endOfExplosionTick) {
    bomb->placed = 1;
    bomb->tile = tile;

    bomb->explodeTick = explodeTick;
    bomb->endOfExplosionTick = endOfExplosionTick;
    bomb->bombRect.w = board->length/ board->size / 2;
    bomb->bombRect.h = bomb->bombRect.w;
    bomb->bombRect.x = (tile % board->size) * board->tile_length + board->start_x + board->tile_length / 4;
    bomb->bombRect.y = (tile / board->size) * board->tile_length + board->start_y + board->tile_length / 4;


    if(SDL_HasIntersection(&bomb->bombRect, &player->image)){
        bomb->underPlayer = 1;
    }
}

void hideBomb(Bomb *bomb) {
    bomb->placed = 0;
    bomb->exploded = 0;
    bomb->tile = -1;
    bomb->bombRect.w = 0;
    bomb->bombRect.h = 0;
}

void checkForExplosion(Bomb *bomb, Board* board) {
    pthread_mutex_lock(&tick_lock);
    if(bomb->placed == 1 && actualTick>= bomb->explodeTick && bomb->exploded == 0){
        bomb->exploded = 1;
        explode(bomb, board);
    }
    pthread_mutex_unlock(&tick_lock);
}

void explode(Bomb* bomb, Board* board){
    // Rects will go more in each side till they collide or till range
    SDL_Rect right;
    SDL_Rect left;
    SDL_Rect up;
    SDL_Rect down;
    int collision = 0;
    right.x = (bomb->tile % board->size) * board->tile_length + board->start_x + board->tile_length;
    right.y = (bomb->tile / board->size) * board->tile_length + board->start_y;
    right.w = board->tile_length;
    right.h = board->tile_length;

    down.x = (bomb->tile % board->size) * board->tile_length + board->start_x;
    down.y = (bomb->tile / board->size) * board->tile_length + board->start_y + board->tile_length;
    down.w = board->tile_length;
    down.h = board->tile_length;

    left.x = (bomb->tile % board->size) * board->tile_length + board->start_x - board->tile_length;
    left.y = (bomb->tile / board->size) * board->tile_length + board->start_y;
    left.w = board->tile_length;
    left.h = board->tile_length;

    up.x = (bomb->tile % board->size) * board->tile_length + board->start_x;
    up.y = (bomb->tile / board->size) * board->tile_length + board->start_y - board->tile_length;
    up.w = board->tile_length;
    up.h = board->tile_length;


    for(int i = 0; i < bomb->range; i++){
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
        //right side collide with breakable ice block - destroying it
        for(int j = 0; j < board->breakableIceBlocksCount; j++){
            if(SDL_HasIntersection(&right, board->breakableIceBlocks[j])){
                right.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        if(right.w < board->tile_length * bomb->range)
            right.w += board->tile_length;
    }

    collision = 0;
    for(int i = 0; i < bomb->range; i++){
        //down side collide with outside wall
        for(int j = 0; j < 4; j++){
            if(SDL_HasIntersection(&down, &board->outsideWalls[j])){
                down.h -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        //down side collide with ice block
        for(int j = 0; j < board->iceBlocksCount; j++){
            if(SDL_HasIntersection(&down, &board->iceBlocks[j])){
                down.h -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        //down side collide with breakable ice block - destroying it
        for(int j = 0; j < board->breakableIceBlocksCount; j++){
            if(SDL_HasIntersection(&down, board->breakableIceBlocks[j])){
                down.h -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        if(down.h < board->tile_length * bomb->range)
            down.h += board->tile_length;
    }

    collision = 0;
    for(int i = 0; i < bomb->range; i++){
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
        //upper side collide with breakable ice block - destroying it
        for(int j = 0; j < board->breakableIceBlocksCount; j++){
            if(SDL_HasIntersection(&up, board->breakableIceBlocks[j])){
                up.y += board->tile_length;
                up.h -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        if(up.h < board->tile_length * bomb->range){
            up.y -= board->tile_length;
            up.h += board->tile_length;
        }
    }

    collision = 0;
    for(int i = 0; i < bomb->range; i++){
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
        //right side collide with breakable ice block - destroying it
        for(int j = 0; j < board->breakableIceBlocksCount; j++){
            if(SDL_HasIntersection(&left, board->breakableIceBlocks[j])){
                left.x += board->tile_length;
                left.w -= board->tile_length;
                collision = 1;
                break;
            }
        }
        if (collision)
            break;
        if(left.w < board->tile_length * bomb->range){
            left.x -= board->tile_length;
            left.w += board->tile_length;
        }
    }

    // clockwise
    bomb->explosionRect[0] = up;
    bomb->explosionRect[1] = right;
    bomb->explosionRect[2] = down;
    bomb->explosionRect[3] = left;

    bomb->centerRect.x = (bomb->tile % board->size) * board->tile_length + board->start_x;
    bomb->centerRect.y = (bomb->tile / board->size) * board->tile_length + board->start_y;
    bomb->centerRect.w = board->tile_length;
    bomb->centerRect.h = board->tile_length;
}

void renderBomb(Bomb* bomb, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, bomb->bombTexture, NULL, &bomb->bombRect);
}

void renderExplosion(Bomb* bomb, SDL_Renderer* renderer)
{
    for(int i = 0; i < 4; i++) {
        if(i % 2 == 0) // vertical
            SDL_RenderCopy(renderer, bomb->explVerTexture, NULL, &bomb->explosionRect[i]);
        else // horizontal
            SDL_RenderCopy(renderer, bomb->explHorTexture, NULL, &bomb->explosionRect[i]);
    }
    SDL_RenderCopy(renderer, bomb->centerTexture, NULL, &bomb->centerRect);
}

void closeBomb(Bomb *bomb) {
    SDL_DestroyTexture(bomb->bombTexture);
    SDL_DestroyTexture(bomb->explHorTexture);
    SDL_DestroyTexture(bomb->explVerTexture);
    SDL_DestroyTexture(bomb->centerTexture);
    //free(bomb);
}

void closeAllBombs(int count){
    for(int i = 0; i < count; i++){
        free(bombs[i]);
    }
    free(bombs);
    pthread_mutex_destroy(&bombs_lock);
}
#pragma clang diagnostic pop