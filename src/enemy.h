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
    int nextX;
    int nextY;
    char* name;
} Enemy;

Enemy* enemies[3];

void initEnemy(Enemy* enemy, Board* board, int startX, int startY, char* name);

void loadEnemy(SDL_Window *window, SDL_Renderer *renderer, Enemy *p);

void moveEnemy(Enemy* enemy, Board* board);

void renderEnemy(Enemy* enemy, SDL_Renderer* renderer);

void closeEnemy(Enemy* enemy);

#endif //BOMBERMAN_CLIENT_ENEMY_H
