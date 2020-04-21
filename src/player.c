//
// Created by pioter on 20.04.2020.
//
#include "player.h"

void LoadPlayer(SDL_Renderer *renderer, Player p, const char *fileName) //Ładowanie tekstury gracza
{
    p.loadedImage = IMG_Load(fileName);
    if(p.loadedImage == NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }
    p.texture = SDL_CreateTextureFromSurface(renderer, p.loadedImage);
    SDL_FreeSurface(p.loadedImage);
    SDL_RenderCopy(renderer, p.texture, NULL, NULL);
}