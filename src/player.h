//
// Created by ves on 21/04/2020.
//

#ifndef BOMBERMAN_CLIENT_PLAYER_H
#define BOMBERMAN_CLIENT_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PLAYER_SPRITE_PATH "./../sprites/Green_square.png"

typedef struct Player
{
    SDL_Texture *texture;
    SDL_Rect image;
} Player;

void loadPlayer(SDL_Window *window, SDL_Renderer *renderer, Player *p); //Ładowanie tekstury gracza

#endif //BOMBERMAN_CLIENT_PLAYER_H
