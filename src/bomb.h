//
// Created by ves on 06/05/2020.
//

#ifndef BOMBERMAN_CLIENT_BOMB_H
#define BOMBERMAN_CLIENT_BOMB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct bomb_struct{
  int tile;
  int range;
  SDL_Texture* texture;
  SDL_Rect image;
}Bomb;



#endif //BOMBERMAN_CLIENT_BOMB_H
