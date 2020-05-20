//
// Created by pioter on 20.05.2020.
//

#ifndef BOMBERMAN_CLIENT_ENEMY_H
#define BOMBERMAN_CLIENT_ENEMY_H

#include <SDL2/SDL.h>
#include "board.h"
#include "bomb.h"

typedef struct Enemy{
    SDL_Texture *texture;
    SDL_Rect image;
    double x;
    double y;
    double velocity;
    double velX;
    double velY;
    int placedBomb;
    int onBomb;
} Enemy;

void initEnemy(Enemy* player, Board* board, int startX, int startY);

void loadEnemy(SDL_Window *window, SDL_Renderer *renderer, Enemy *p);

void moveEnemy(Enemy* player, Board* board, Bomb* bombs, double timeStep);

void renderEnemy(Enemy* player, SDL_Renderer* renderer);

void closeEnemy(Enemy* player);

#endif //BOMBERMAN_CLIENT_ENEMY_H
