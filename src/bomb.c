//
// Created by ves on 06/05/2020.
//

#include "bomb.h"

void initBomb(Bomb *bomb) {
    bomb->tile = -1;
    bomb->placed = 0;
    bomb->range = 2; // 2 tiles from bomb in each direction are affected
    bomb->timer = (Timer*)malloc(sizeof(Timer));
    initTimer(bomb->timer);
}

void loadBomb(Bomb *bomb, SDL_Renderer* renderer) {
    SDL_Surface* bombSurface = IMG_Load("./../sprites/Black_square.png");
    SDL_Surface* exploSurface = IMG_Load("./../sprites/Explosion.png");
    if(bombSurface == NULL || exploSurface == NULL){
        fprintf(stderr, "Failed to load bomb sprites.\n");
        return;
    }
    bomb->bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    SDL_FreeSurface(bombSurface);
    bomb->explosionTexture = SDL_CreateTextureFromSurface(renderer, exploSurface);
    SDL_FreeSurface(exploSurface);
}

void closeBomb(Bomb *bomb) {
    SDL_DestroyTexture(bomb->bombTexture);
    SDL_DestroyTexture(bomb->explosionTexture);
    free(bomb->timer);
}