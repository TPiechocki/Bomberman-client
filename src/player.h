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
    SDL_Texture *texture; // player texture
    SDL_Rect image; // bounding box of player
    double x; // x coordinate of player
    double y; // y coordinate of player
    int current_tile; // tile on which middle of player character stands
    unsigned int counter; // player action counter
    int placedBomb; // status of bomb placement by player
    double velocity; // player velocity
    double velX; // horizontal velocity
    double velY; // vertical velocity
    int bombId; // assigned bomb
    int isAlive; // status of player
} Player;

Player* player; // player structure handle
pthread_mutex_t player_lock; // player mutex

/**
 * Initializes player structure, sets starting position and
 * initializes player mutex.
 * @param board - board handle
 * @param player_number - determines starting position
 * @param x - starting coordinate x (on reconnect after restart)
 * @param y - starting coordinate y (on reconnect after restart)
 * @param bomb - id of bomb belonging to player
 */
void initPlayer(Board* board, int player_number, int x, int y, int bomb);

/**
 * Loads texture resources.
 * @param window - window handle
 * @param renderer - renderer handle
 */
void loadPlayer(SDL_Window *window, SDL_Renderer *renderer);

/**
 * Handles keyboard events linked to player actions.
 * @param e - SDL event
 * @param renderer - renderer handle
 * @param board - board handle
 */
void handlePlayerEvent(SDL_Event* e, SDL_Renderer *renderer, Board *board);
/**
 * Translates player coordinates depending on velocity and time step.
 * Checks for possible collisions and corrects player position accordingly.
 * @param board - board handle
 * @param timeStep - time passed between moves
 */
void movePlayer(Board* board, double timeStep);

/**
 * Testing function for checking all tiles player's bounding box stands on.
 * @param x - x coordinate of player
 * @param y - y coordinate of player
 */
void getAllTiles(int x, int y);

/**
 * Sets placedBomb flag and sends information about placed bomb to server.
 * @param board - board handle
 */
void placeBombPlayer(Board *board);

/**
 * Renders player sprite.
 * @param renderer
 */
void renderPlayer(SDL_Renderer* renderer);

/**
 * Frees resources and destroys player mutex.
 */
void closePlayer();


#endif //BOMBERMAN_CLIENT_PLAYER_H
