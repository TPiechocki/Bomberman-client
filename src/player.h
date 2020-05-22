//
// Created by ves on 21/04/2020.
//

#ifndef BOMBERMAN_CLIENT_PLAYER_H
#define BOMBERMAN_CLIENT_PLAYER_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#include "conn/connection.h"
#include "bomb.h"

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

Player* player;
pthread_mutex_t player_lock;

void initPlayer(Board* board, int player_number, int x, int y);

void loadPlayer(SDL_Window *window, SDL_Renderer *renderer); //Loading player texture

void handlePlayerEvent(SDL_Event* e, SDL_Renderer *renderer, Board *board, Bomb* bomb);
void movePlayer(Board* board, Bomb* bombs, double timeStep);

void placeBombPlayer(Board *board, Bomb* bomb);

void renderPlayer(SDL_Renderer* renderer);

void closePlayer();


#endif //BOMBERMAN_CLIENT_PLAYER_H
