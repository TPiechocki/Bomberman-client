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

void close(Bomb *bomb) {
    SDL_DestroyTexture(bomb->bombTexture);
    SDL_DestroyTexture(bomb->explosionTexture);
    free(bomb->timer);
}
