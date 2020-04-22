//
// Created by pioter on 20.04.2020.
//
#include "player.h"

void LoadPlayer(SDL_Window *window, SDL_Renderer *renderer, Player *p) //Ładowanie tekstury gracza
{
    p->surface = IMG_Load(PLAYER_SPRITE_PATH);
    if(p->surface == NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }
    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    p->texture = SDL_CreateTextureFromSurface(renderer, p->surface);
    p->image.w = windowWidth / 20;//Rozmiar dla testów, do zmiany
    p->image.h = windowHeight / 20;
    p->image.x = windowWidth / 10;
    p->image.y = windowHeight / 10;
    SDL_FreeSurface(p->surface);
}
