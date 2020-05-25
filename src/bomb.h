//
// Created by ves on 06/05/2020.
//

#ifndef BOMBERMAN_CLIENT_BOMB_H
#define BOMBERMAN_CLIENT_BOMB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "timers/timer.h"
#include "board.h"
#include "player.h"
#include <pthread.h>

typedef struct bomb_struct{
  int tile; // current tile where bomb is placed
  int range; // range of explosion
  int placed; // status of bomb; if bomb is placed
  int exploded; // status of bomb; if bomb exploded
  int underPlayer; // status of bomb; if bomb was placed under player
  int explodeTick; // expected server tick for bomb to explode on
  int endOfExplosionTick; // expected server tick for bomb to stop exploding
  SDL_Texture* bombTexture; // bomb texture
  SDL_Texture* explVerTexture; // vertical explosion texture
  SDL_Texture* explHorTexture; // horizontal explosion texture
  SDL_Texture* centerTexture; // center of explosion texture
  SDL_Rect bombRect; // bounding box of bomb
  SDL_Rect explosionRect[4]; // bounding box of side explosions
  SDL_Rect centerRect; // bounding box of center explosion
}Bomb;

Bomb** bombs; // array of bombs
pthread_mutex_t bombs_lock; // bombs mutex

/**
 * Allocates memory for bombs and initialises bombs mutex.
 * @param count - number of bombs
 */
void initAllBombs(int count);
/**
 * Initialises bomb.
 * @param bomb - bomb to initialise
 */
void initBomb(Bomb* bomb);

/**
 * Loads textures for bomb.
 * @param bomb - bomb to load textures for
 * @param renderer - renderer handle
 */
void loadBomb(Bomb* bomb, SDL_Renderer* renderer);

/**
 * Places bomb in the center of given tile and sets expected explosion ticks.
 * If placed under player it sets proper status.
 * @param bomb - bomb to place
 * @param board - board handle
 * @param tile - tile to place bomb on
 * @param explodeTick - tick when bomb explodes
 * @param endOfExplosionTick - tick when bomb's explosion vanishes
 */
void placeBomb(Bomb* bomb, Board* board, int tile, int explodeTick, int endOfExplosionTick);
/**
 * Removes bomb from arena.
 * @param bomb - bomb to be removed
 */
void hideBomb(Bomb* bomb);

/**
 * Checks if bomb should explode based on tick given from server.
 * @param bomb - bomb to check
 * @param board - board handle
 */
void checkForExplosion(Bomb* bomb, Board* board);
/**
 * Sets bounding boxes of explosions accordingly to map.
 * Explosion travelling up to specified range destroying up to 1 wall.
 * @param bomb - bomb to explode
 * @param board - board handle
 */
void explode(Bomb* bomb, Board* board);

/**
 * Renders bomb.
 * @param bomb - bomb to render
 * @param renderer - renderer handle
 */
void renderBomb(Bomb* bomb, SDL_Renderer* renderer);
/**
 * Renders explosion of given bomb.
 * @param bomb - explosion of given bomb
 * @param renderer - renderer handle
 */
void renderExplosion(Bomb* bomb, SDL_Renderer* renderer);

/**
 * Frees resources of bomb.
 * @param bomb - bomb to clear
 */
void closeBomb(Bomb* bomb);
/**
 * Frees memory of bombs and destroys bombs mutex.
 * @param count - number of bombs to free memory for
 */
void closeAllBombs(int count);

#endif //BOMBERMAN_CLIENT_BOMB_H
