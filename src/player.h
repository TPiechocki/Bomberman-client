//
// Created by ves on 21/04/2020.
//

#ifndef BOMBERMAN_CLIENT_PLAYER_H
#define BOMBERMAN_CLIENT_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"

#define PLAYER_SPRITE_PATH "./../sprites/Green_square.png"

typedef struct Player
{
    SDL_Texture *texture;
    SDL_Rect image;
    double x;
    double y;
    int counter;
    int verticalDirection;
    int horizontalDirection;
    double velocity;
    double velX;
    double velY;
} Player;

void initPlayer(Player* player, Board* board);

void loadPlayer(SDL_Window *window, SDL_Renderer *renderer, Player *p); //Loading player texture

void handlePlayerEvent(Player* player, SDL_Event* e);
void movePlayer(Player* player, Board* board, double timeStep);

void renderPlayer(Player* player, SDL_Renderer* renderer);

void moving(Player *p);
void changeMove(Player *p, SDL_Event e);//Change direction of moving
void brake(Player *player, SDL_Event e);//Stop moving

#endif //BOMBERMAN_CLIENT_PLAYER_H
