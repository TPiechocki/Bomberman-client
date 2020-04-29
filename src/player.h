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
    int verticalDirection;
    int horizontalDirection;
    int x;
    int y;
    int counter;
} Player;

void loadPlayer(SDL_Window *window, SDL_Renderer *renderer, Player *p); //Loading player texture
void moving(Player *p);
void changeMove(Player *p, SDL_Event e);//Change direction of moving
void brake(Player *player, SDL_Event e);//Stop moving

#endif //BOMBERMAN_CLIENT_PLAYER_H
