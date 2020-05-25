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
    SDL_Texture *texture; // enemie's texture
    SDL_Rect image; // bounding box of enemy
    int x; // current coordinate x
    int y; // current coordinate y
    int nextX; // next coordinate x given by server
    int nextY; // next coordinate y given by server
    int stepCounter; // step counter for smoothing
    int stepX; // next step coordinate x
    int stepY; //  next step coordinate y
    int bombId; // assigned bomb
    char name[100]; // name of enemy
    int isAlive; // status of enemy
} Enemy;

Enemy** enemies; // handle to array of enemies
pthread_mutex_t enemy_lock; // enemy mutex

/**
 * Allocates memory for enemies and initialises enemy mutex.
 * @param count - amount of enemies to allocate for
 */
void initAllEnemies(int count);
/**
 * Initialises enemy structure, sets starting position and assigns bomb.
 * @param enemy - enemy to initialise
 * @param board - board handle
 * @param player_number - determines starting pos
 * @param startX - starting coordinate x (on reconnect after restart)
 * @param startY - starting coordinate y (on reconnect after restart)
 * @param name - name of enemy
 * @param bombId - number of assigned bomb
 */
void initEnemy(Enemy* enemy, Board* board, int player_number, int startX, int startY, char* name, int bombId);

/**
 * Loads texture resource.
 * @param renderer - renderer handle
 * @param enemy - enemy to load textures for
 * @param player_number - dictates look change (only 1 look implemented now)
 */
void loadEnemy(SDL_Renderer *renderer, Enemy *enemy, int player_number);

/**
 * Move enemy depending on step coordinates.
 * @param enemy - enemy to move
 */
void moveEnemy(Enemy* enemy);

/**
 * Checks if any of the enemies are alive.
 * @return 0 - all enemies dead; 1 - at least 1 enemy alive
 */
int checkEnemyLives();

/**
 * Renders enemy sprite.
 * @param enemy - enemy to render
 * @param renderer - renderer handle
 */
void renderEnemy(Enemy* enemy, SDL_Renderer* renderer);

/**
 * Frees resources for given enemy.
 * @param enemy - enemy to clear
 */
void closeEnemy(Enemy* enemy);
/**
 * Frees memory allocated for enemies and destroys enemy mutex.
 * @param count - amount of enemies
 */
void closeAllEnemies(int count);

#endif //BOMBERMAN_CLIENT_ENEMY_H
