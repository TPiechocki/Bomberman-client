//
// Created by pioter on 20.05.2020.
//

#ifndef BOMBERMAN_CLIENT_ENEMY_H
#define BOMBERMAN_CLIENT_ENEMY_H

#include <SDL2/SDL.h>
#include "board.h"
#include "bomb.h"
#include <pthread.h>

typedef struct Enemy{
    SDL_Texture *texture;
    SDL_Rect image;
    int x;
    int y;
    int nextX;
    int nextY;
    int stepCounter;
    int stepX;
    int stepY;
    char name[100];
} Enemy;

Enemy** enemies;
pthread_mutex_t enemy_lock;

void initAllEnemies(int count);
void initEnemy(Enemy* enemy, Board* board, int player_number, int startX, int startY, char* name);

void loadEnemy( SDL_Renderer *renderer, Enemy *p, int player_number);

void moveEnemy(Enemy* enemy);

void renderEnemy(Enemy* enemy, SDL_Renderer* renderer);

void closeEnemy(Enemy* enemy);
void closeAllEnemies(int count);

#endif //BOMBERMAN_CLIENT_ENEMY_H
