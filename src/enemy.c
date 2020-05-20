//
// Created by pioter on 20.05.2020.
//
#include "enemy.h"


void initEnemy(Enemy *enemy, Board *board, int startX, int startY, char* name) {
    double tilesPerSecond = 3;

    //enemy->velocity = tilesPerSecond * (double)board->length / board->size; // tile per second
    //enemy->velX = 0;
    //enemy->velY = 0;

    //enemy->onBomb = 0;
    //enemy->placedBomb = 0;
    enemy->name = name;

    enemy->image.w = board->length / board->size * 7 / 10;
    enemy->image.h = enemy->image.w;

    enemy->x = startX;
    enemy->y = startY;
    enemy->nextX = -1;
    enemy->nextY = -1;
    enemy->image.x = (int)enemy->x - enemy->image.w / 2;
    enemy->image.y = (int)enemy->y - enemy->image.w / 2;
}

void loadEnemy(SDL_Window *window, SDL_Renderer *renderer, Enemy *enemy)
{
    SDL_Surface *surface = IMG_Load("./../sprites/enemy.png");
    if(surface == NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }

    enemy->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void moveEnemy(Enemy* enemy, Board* board, Bomb* bombs, double timeStep){
    // based off server messages
}



void renderEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, enemy->texture, NULL, &enemy->image);
}

void closeEnemy(Enemy *enemy) {
    if(enemy->texture != NULL)
        SDL_DestroyTexture(enemy->texture);
}
