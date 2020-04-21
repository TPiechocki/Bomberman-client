//
// Created by pioter on 20.04.2020.
//
#include "player.h"

void LoadPlayer(SDL_Renderer *renderer, Player *p) //Ładowanie tekstury gracza
{
    p->surface = IMG_Load(PLAYER_SPRITE_PATH);
    if(p->surface == NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }
    p->texture = SDL_CreateTextureFromSurface(renderer, p->surface);
    p->image.w = 20;//Rozmiar dla testów, do zmiany
    p->image.h = 20;
    p->image.x = 100;
    p->image.y = 100;
    SDL_FreeSurface(p->surface);
}