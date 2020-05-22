//
// Created by pioter on 20.05.2020.
//
#include "enemy.h"

void initAllEnemies(int count){
    enemies = (Enemy**)malloc(sizeof(Enemy*)*count);
    for(int i = 0; i < count; i++){
        enemies[i] = (Enemy*)malloc(sizeof(Enemy));
    }
    pthread_mutex_init(&enemy_lock, NULL);
}

void initEnemy(Enemy *enemy, Board *board, int player_number , int startX, int startY, char* name) {

    //enemy = (Enemy*)malloc(sizeof(Enemy));
    strcpy(enemy->name, name);

    enemy->image.w = board->length / board->size * 7 / 10;
    enemy->image.h = enemy->image.w;

    if(startX == 0 || startY == 0)
    {
        switch(player_number){
            case 0:
                enemy->x = board->start_x + board->tile_length / 2;
                enemy->y = board->start_y + board->tile_length / 2;
                break;
            case 1:
                enemy->x = board->end_x - board->tile_length / 2;
                enemy->y = board->end_y - board->tile_length / 2;
                break;
            case 2:
                enemy->x = board->start_x + board->tile_length / 2;
                enemy->y = board->end_y - board->tile_length / 2;
                break;
            case 3:
                enemy->x = board->end_x - board->tile_length / 2;
                enemy->y = board->start_y + board->tile_length / 2;
                break;
        }
    }
    else
    {
        enemy->x = startX;
        enemy->y = startY;
    }
    enemy->nextX = enemy->x;
    enemy->nextY = enemy->y;
    enemy->image.x = enemy->x - enemy->image.w / 2;
    enemy->image.y = enemy->y - enemy->image.w / 2;
    enemy->stepCounter = 6;
}

void loadEnemy(SDL_Renderer *renderer, Enemy *enemy, int player_number)
{
    SDL_Surface *surface;
    switch (player_number) {
        case 0:
            surface = IMG_Load("./../sprites/enemy.png");
            break;
        case 1:
            surface = IMG_Load("./../sprites/enemy.png");
            break;
        case 2:
            surface = IMG_Load("./../sprites/enemy.png");
            break;
        default:
            break;
    }
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
    pthread_mutex_lock(&enemy_lock);
    if(enemy->stepCounter < 6)
    {
        enemy->x += enemy->stepX;
        enemy->y += enemy->stepY;
        enemy->stepCounter++;
    }

    //enemy->x = enemy->nextX;
    //enemy->y = enemy->nextY;
    enemy->image.x = enemy->x - enemy->image.w / 2;
    enemy->image.y = enemy->y - enemy->image.w / 2;
    // UNLOCK
    pthread_mutex_unlock(&enemy_lock);
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
    pthread_mutex_destroy(&enemy_lock);
}
