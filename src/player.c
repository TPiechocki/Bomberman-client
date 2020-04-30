//
// Created by pioter on 20.04.2020.
//
#include "player.h"

void initPlayer(Player *player, Board *board) {
    player->x = board->start_x;
    player->y = board->start_y;
    player->counter = 0;
    player->verticalDirection = player->horizontalDirection = 0;

    double tilesPerSecond = 3;

    player->velocity = tilesPerSecond * (double)board->length / board->size; // tile per second
    player->velX = 0;
    player->velY = 0;

    player->image.w = board->length / board->size * 9 / 10;
    player->image.h = player->image.w;
    player->image.x = (int)player->x;
    player->image.y = (int)player->y;
}

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

    p->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void handlePlayerEvent(Player *player, SDL_Event *e) {
    // If key was pressed
    if( e->type == SDL_KEYDOWN && e->key.repeat == 0){
        // Adjust velocity (start moving)
        switch (e->key.keysym.sym) {
            case SDLK_UP: player->velY -= player->velocity; break;
            case SDLK_DOWN: player->velY += player->velocity; break;
            case SDLK_LEFT: player->velX -= player->velocity; break;
            case SDLK_RIGHT: player->velX += player->velocity; break;
        }
    }
    else if( e->type == SDL_KEYUP && e->key.repeat == 0){
        // Adjust velocity (stop moving)
        switch (e->key.keysym.sym) {
            case SDLK_UP: player->velY += player->velocity; break;
            case SDLK_DOWN: player->velY -= player->velocity; break;
            case SDLK_LEFT: player->velX += player->velocity; break;
            case SDLK_RIGHT: player->velX -= player->velocity; break;
        }
    }
}

void movePlayer(Player* player, Board* board, double timeStep){
    // Scale for diagonal movement
    double scale = 1;
    // If the diagonal movement occurs, scale it down
    if(player->velX != 0 && player->velY != 0)
        scale = 1/sqrt(2);
    // Move left or right
    player->x += player->velX * scale * timeStep;
    // Check if it didn't go out of bounds
    if(player->x < board->start_x)
        player->x = board->start_x; // move to the left edge if going out of bounds
    else if(player->x + player->image.w > board->end_x + 1)
        player->x = board->end_x - player->image.w; // move to the right edge if going out of bounds
    else
        player->image.x = (int)player->x; // assign new coord if they didn't

    // Move up or down
    player->y += player->velY * scale * timeStep;
    // Check if it didn't go out of bounds
    if(player->y < board->start_y)
        player->y = board->start_y; // move to the top edge if going out of bounds
    else if (player->y + player->image.h > board->end_y + 1)
        player->y = board->end_y - player->image.h; // move to the bottom edge if going out of bounds
    else
        player->image.y = (int)player->y; // assign new coord if they didn't go out of bounds
}

void renderPlayer(Player *player, SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, player->texture, NULL, &player->image);
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
