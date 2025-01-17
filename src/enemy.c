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

void initEnemy(Enemy *enemy, Board *board, int player_number , int startX, int startY, char* name, int bombId) {

    //enemy = (Enemy*)malloc(sizeof(Enemy));
    strcpy(enemy->name, name);

    enemy->image.w = board->length / board->size * 7 / 10;
    enemy->image.h = enemy->image.w;

    enemy->bombId = bombId;

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
    enemy->isAlive = 1;
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
    pthread_mutex_lock(&enemy_lock); // LOCK
    enemy->texture = SDL_CreateTextureFromSurface(renderer, surface);
    pthread_mutex_unlock(&enemy_lock); // UNLOCK
    SDL_FreeSurface(surface);
}

void moveEnemy(Enemy* enemy){
    // based off server messages
    // move enemy to next position given by server
    pthread_mutex_lock(&enemy_lock); // LOCK
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

    pthread_mutex_unlock(&enemy_lock); // UNLOCK
}

int checkEnemyLives(){
    for(int i = 0; i < conn->player_count - 1; i++)
        if(enemies[i]->isAlive)
            return 1;
    return 0;
}


void renderEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    pthread_mutex_lock(&enemy_lock); // LOCK
    SDL_RenderCopy(renderer, enemy->texture, NULL, &enemy->image);
    pthread_mutex_unlock(&enemy_lock); // UNLOCK
}

void closeEnemy(Enemy *enemy) {
    if(enemy->texture != NULL)
        SDL_DestroyTexture(enemy->texture);
}

void closeAllEnemies(int count){
    // deallocate each enemy
    for(int i = 0; i < count; i++){
        free(enemies[i]);
    }
    // deallocate handle
    free(enemies);
    pthread_mutex_destroy(&enemy_lock);
}
