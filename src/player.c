//
// Created by pioter on 20.04.2020.
//
#include "player.h"

void loadPlayer(SDL_Window *window, SDL_Renderer *renderer, Player *p)
{
    SDL_Surface *surface = IMG_Load(PLAYER_SPRITE_PATH);
    if(surface == NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }
    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    p->counter = 0;
    p->verticalDirection = p->horizontalDirection = 0;
    p->x = windowWidth / 10;
    p->y = windowHeight / 10;
    p->texture = SDL_CreateTextureFromSurface(renderer, surface);
    p->image.w = windowWidth / 20;
    p->image.h = windowHeight / 20;
    p->image.x = p->x;
    p->image.y = p->y;
    SDL_FreeSurface(surface);
}

void changeMove(Player *player, SDL_Event event)
{
    switch(event.key.keysym.sym) //Choosing direction of move
    {
        case SDLK_DOWN:
            player->verticalDirection = 1;
            break;
        case SDLK_UP:
            player->verticalDirection = -1;
            break;
        case SDLK_RIGHT:
            player->horizontalDirection = 1;
            break;
        case SDLK_LEFT:
            player->horizontalDirection = -1;
            break;
    }
}

void moving(Player *player)
{
    if (player->counter == 10) //Moving one per 10 times (more often is too fast)
    {
        if(player->horizontalDirection != 0)
        {
            player->x += player->horizontalDirection;
            player->image.x = player->x;
        }
        if(player->verticalDirection!= 0)
        {
            player->y += player->verticalDirection;
            player->image.y = player->y;
        }
        player->counter = 0;
    }
    player->counter++;
}

void brake(Player *player, SDL_Event event)
{
    switch (event.key.keysym.sym)
    {
        case SDLK_DOWN:
            if(player->verticalDirection == 1)
                player->verticalDirection = 0;
            break;
        case SDLK_UP:
            if(player->verticalDirection == -1)
                player->verticalDirection = 0;
            break;
        case SDLK_LEFT:
            if(player->horizontalDirection == -1)
                player->horizontalDirection = 0;
            break;
        case SDLK_RIGHT:
            if(player->horizontalDirection == 1)
                player->horizontalDirection = 0;
            break;
    }
}