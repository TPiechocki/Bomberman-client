//
// Created by pioter on 20.05.2020.
//
#include "enemy.h"

void initAllEnemies(int count){
    enemies = (Enemy**)malloc(sizeof(Enemy*)*count);
    for(int i = 0; i < 4; i++){
        enemies[i] = (Enemy*)malloc(sizeof(Enemy));
    }
}

void initEnemy(Enemy *enemy, Board *board, int startX, int startY, char* name) {

    //enemy = (Enemy*)malloc(sizeof(Enemy));
    enemy->name = name;

    enemy->image.w = board->length / board->size * 7 / 10;
    enemy->image.h = enemy->image.w;

    enemy->x = startX;
    enemy->y = startY;
    enemy->nextX = enemy->x;
    enemy->nextY = enemy->y;
    enemy->image.x = enemy->x - enemy->image.w / 2;
    enemy->image.y = enemy->y - enemy->image.w / 2;
}

void loadEnemy(SDL_Renderer *renderer, Enemy *enemy)
{
    SDL_Surface *surface = IMG_Load("./../sprites/enemy.png");
    if(surface == NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }

    enemy->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void moveEnemy(Enemy* enemy){
    // based off server messages
    // move enemy to next position given by server
    // LOCK - alternative LOCK before for() for all enemies
    enemy->x = enemy->nextX;
    enemy->y = enemy->nextY;
    enemy->image.x = enemy->x - enemy->image.w / 2;
    enemy->image.y = enemy->y - enemy->image.w / 2;
    // UNLOCK
}



void renderEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, enemy->texture, NULL, &enemy->image);
}

void closeEnemy(Enemy *enemy) {
    if(enemy->texture != NULL)
        SDL_DestroyTexture(enemy->texture);
    //free(enemy);
}

void closeAllEnemies(int count){
    for(int i = 0; i < count; i++){
        free(enemies[i]);
    }
    free(enemies);
}
