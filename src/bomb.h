//
// Created by ves on 06/05/2020.
//

#ifndef BOMBERMAN_CLIENT_BOMB_H
#define BOMBERMAN_CLIENT_BOMB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "timers/timer.h"
#include "board.h"

typedef struct bomb_struct{
  int tile;
  int exploded;
  int range;
  int placed;
  Timer* timer;
  SDL_Texture* bombTexture;
  SDL_Texture* explosionTexture;
  SDL_Rect bombRect;
  SDL_Rect explosionRect[4];
}Bomb;

void initBomb(Bomb* bomb);
void loadBomb(Bomb* bomb, SDL_Renderer* renderer, Board* board, int tile);

void explode(Bomb* bomb, Board* board);

void renderBomb(Bomb* bomb, SDL_Renderer* renderer);
void renderExplosion(Bomb* bomb, SDL_Renderer* renderer);
void closeBomb(Bomb* bomb);

#endif //BOMBERMAN_CLIENT_BOMB_H
