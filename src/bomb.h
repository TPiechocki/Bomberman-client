//
// Created by ves on 06/05/2020.
//

#ifndef BOMBERMAN_CLIENT_BOMB_H
#define BOMBERMAN_CLIENT_BOMB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "timers/timer.h"
#include "board.h"
#include <pthread.h>

typedef struct bomb_struct{
  int tile;
  int range;
  int placed;
  int exploded;
  int explodeTick;
  Timer* timer;
  SDL_Texture* bombTexture;
  SDL_Texture* explVerTexture;
  SDL_Texture* explHorTexture;
  SDL_Texture* centerTexture;
  SDL_Rect bombRect;
  SDL_Rect explosionRect[4];
  SDL_Rect centerRect;
}Bomb;

Bomb** bombs;
pthread_mutex_t bombs_lock;

void initAllBombs(int count);
void initBomb(Bomb* bomb);
void loadBomb(Bomb* bomb, SDL_Renderer* renderer);
void placeBomb(Bomb* bomb, Board* board, int tile);
void hideBomb(Bomb* bomb);

void checkForExplosion(Bomb* bomb, Board* board);
void explode(Bomb* bomb, Board* board);

void renderBomb(Bomb* bomb, SDL_Renderer* renderer);
void renderExplosion(Bomb* bomb, SDL_Renderer* renderer);

void closeBomb(Bomb* bomb);
void closeAllBombs(int count);

#endif //BOMBERMAN_CLIENT_BOMB_H
