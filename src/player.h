//
// Created by ves on 21/04/2020.
//

#ifndef BOMBERMAN_CLIENT_PLAYER_H
#define BOMBERMAN_CLIENT_PLAYER_H
#define PLAYER_SPRITE_PATH "./../sprites/Green_square.png"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Player
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect image;
} Player;

void LoadPlayer(SDL_Window *window, SDL_Renderer *renderer, Player *p); //Ładowanie tekstury gracza
#endif //BOMBERMAN_CLIENT_PLAYER_H
