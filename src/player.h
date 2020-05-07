//
// Created by ves on 21/04/2020.
//

#ifndef BOMBERMAN_CLIENT_PLAYER_H
#define BOMBERMAN_CLIENT_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#include "conn/connection.h"

#define PLAYER_SPRITE_PATH "./../sprites/Green_square.png"

typedef struct Player
{
    SDL_Texture *texture;
    SDL_Rect image;
    double x;
    double y;
    int current_tile;
    unsigned int counter;
    int placedBomb;
    int onBomb;
    double velocity;
    double velX;
    double velY;
} Player;

void initPlayer(Player* player, Board* board);

void loadPlayer(SDL_Window *window, SDL_Renderer *renderer, Player *p); //Loading player texture

void handlePlayerEvent(Player* player, SDL_Event* e, Connection* conn);
void movePlayer(Player* player, Board* board, double timeStep);

void placeBombPlayer(Player* player);

void renderPlayer(Player* player, SDL_Renderer* renderer);

void closePlayer(Player* player);


#endif //BOMBERMAN_CLIENT_PLAYER_H
