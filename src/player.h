//
// Created by ves on 21/04/2020.
//

#ifndef BOMBERMAN_CLIENT_PLAYER_H
#define BOMBERMAN_CLIENT_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Player
{
    SDL_Surface *loadedImage;
    SDL_Texture *texture;
} Player;

void LoadPlayer(SDL_Renderer *renderer, Player p, const char *fileName); //Ładowanie tekstury gracza

#endif //BOMBERMAN_CLIENT_PLAYER_H
