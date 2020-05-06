//
// Created by pioter on 20.04.2020.
//
#include "player.h"

void initPlayer(Player *player, Board *board) {
    player->counter = 0;
    player->verticalDirection = player->horizontalDirection = 0;

    double tilesPerSecond = 3;

    player->velocity = tilesPerSecond * (double)board->length / board->size; // tile per second
    player->velX = 0;
    player->velY = 0;

    player->image.w = board->length / board->size * 9 / 10;
    player->image.h = player->image.w;
    // positioning in the middle of top left tile
    player->x = board->start_x + player->image.w / 2 + (board->length / board->size * 6 / 100);
    player->y = board->start_y + player->image.w / 2 + (board->length / board->size * 6 / 100);
    player->image.x = (int)player->x - player->image.w / 2;
    player->image.y = (int)player->y - player->image.w / 2;
    player->current_tile = ((int)(player->y - board->start_y) / board->tile_length) * board->size
                           + ((player->x - board->start_x) / board->tile_length);
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
            /*case SDLK_UP: player->velY -= player->velocity; break;
            case SDLK_DOWN: player->velY += player->velocity; break;
            case SDLK_LEFT: player->velX -= player->velocity; break;
            case SDLK_RIGHT: player->velX += player->velocity; break;*/
            case SDLK_w: player->velY -= player->velocity; break;
            case SDLK_s: player->velY += player->velocity; break;
            case SDLK_a: player->velX -= player->velocity; break;
            case SDLK_d: player->velX += player->velocity; break;
            case SDLK_SPACE: break;
        }
    }
    else if( e->type == SDL_KEYUP && e->key.repeat == 0){
        // Adjust velocity (stop moving)
        switch (e->key.keysym.sym) {
            /*case SDLK_UP: player->velY += player->velocity; break;
            case SDLK_DOWN: player->velY -= player->velocity; break;
            case SDLK_LEFT: player->velX += player->velocity; break;
            case SDLK_RIGHT: player->velX -= player->velocity; break;*/
            case SDLK_w: player->velY += player->velocity; break;
            case SDLK_s: player->velY -= player->velocity; break;
            case SDLK_a: player->velX += player->velocity; break;
            case SDLK_d: player->velX -= player->velocity; break;
        }
    }
}

void movePlayer(Player* player, Board* board, double timeStep){
    // Scale for diagonal movement
    double scale = 1.;
    // If the diagonal movement occurs, scale it down
    if(player->velX != 0 && player->velY != 0)
        scale = 1./sqrt(2);

    // Move left or right
    player->x += player->velX * scale * timeStep;
    // Move up or down
    player->y += player->velY * scale * timeStep;

    SDL_Rect collide = player->image;
    collide.x = (int)player->x - player->image.w / 2;
    collide.y = (int)player->y - player->image.h / 2;
    // check collisions with walls
    for(int i = 0; i < 4; i++){
        if(!SDL_HasIntersection(&collide, &board->outsideWalls[i]))
            // no collision happened with wall
            continue;
        else{
            // collision happened with outside wall
            if(i == 0) // top wall
                player->y = board->start_y + player->image.h / 2;
            else if(i == 1) // bottom wall
                player->y = board->end_y - player->image.h / 2;
            if(i == 2) // left wall
                player->x = board->start_x + player->image.w / 2;
            else if(i == 3) // right wall
                player->x = board->end_x - player->image.w / 2;
        }
    }

    // check collisions with ice walls
    for( int i  = 0; i < board->iceBlocksCount; i++){
        if(!SDL_HasIntersection(&collide, &board->iceBlocks[i]))
            // no collision happened with ice block
            continue;
        else{
            // collision happened with ice wall
            // NOT WORKING PROPERLY
            if(player->x <= board->iceBlocks[i].x + board->iceBlocks[i].w / 2 && player->velX > 0) // player left from block
                player->x = board->iceBlocks[i].x - player->image.w / 2;
            if(player->x >= board->iceBlocks[i].x + board->iceBlocks[i].w / 2 && player->velX < 0) // player right from block
                player->x = board->iceBlocks[i].x + board->iceBlocks[i].w + player->image.w / 2;
            if(player->y <= board->iceBlocks[i].y + board->iceBlocks[i].h / 2 && player->velY > 0) // player up from block
                player->y = board->iceBlocks[i].y - player->image.h / 2;
            if(player->y >= board->iceBlocks[i].y + board->iceBlocks[i].h / 2 && player->velY < 0) // player down from block
                player->y = board->iceBlocks[i].y + board->iceBlocks[i].h + player->image.h / 2;
        }
    }

    // assign new coords if they didn't go out of bounds
    player->image.x = (int)player->x - player->image.w / 2;
    player->image.y = (int)player->y - player->image.h / 2;

    // Check if it didn't go out of bounds
    /*if(player->x - player->image.h / 2 < board->start_x)
        player->x = board->start_x + player->image.w / 2; // move to the left edge if going out of bounds
    else if(player->x + player->image.w / 2 > board->end_x + 1)
        player->x = board->end_x - player->image.w / 2; // move to the right edge if going out of bounds
    else
        player->image.x = (int)player->x - player->image.w / 2; // assign new coord if they didn't

    // Check if it didn't go out of bounds
    if(player->y - player->image.h / 2 < board->start_y)
        player->y = board->start_y + player->image.h / 2; // move to the top edge if going out of bounds
    else if (player->y + player->image.h / 2 > board->end_y + 1)
        player->y = board->end_y - player->image.h / 2; // move to the bottom edge if going out of bounds
    else
        player->image.y = (int)player->y - player->image.h / 2; // assign new coord if they didn't go out of bounds
    */

    player->current_tile = ((int)(player->y - board->start_y) / board->tile_length) * board->size
                           + ((player->x - board->start_x) / board->tile_length);
}

void renderPlayer(Player *player, SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, player->texture, NULL, &player->image);
}

void closePlayer(Player *player) {
    if(player->texture != NULL)
        SDL_DestroyTexture(player->texture);
}
