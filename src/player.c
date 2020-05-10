//
// Created by pioter on 20.04.2020.
//
#include "player.h"

void initPlayer(Player *player, Board *board) {
    double tilesPerSecond = 3;

    player->velocity = tilesPerSecond * (double)board->length / board->size; // tile per second
    player->velX = 0;
    player->velY = 0;

    player->counter = 0;
    player->onBomb = 0;
    player->placedBomb = 0;
    player->bomb = NULL;

    player->image.w = board->length / board->size * 7 / 10;
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

    p->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void handlePlayerEvent(Player *player, SDL_Event *e, SDL_Renderer *renderer, Board *board, Connection* conn) {
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
            case SDLK_SPACE: placeBombPlayer(player, renderer, board); break;
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
        sendPlayerData(conn, (int)player->x, (int)player->y, &player->counter);
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
            /*
            TEMP[i][j] - j:             0                                   1
                    i = 0:      x collision delta                   x callback position
                    i = 1:      y collision delta                   y callback position
                    
            collision delta means how deep is the collision
            it is needed to ignore the unwanted dimension when user try to move in both dimensions
            
            callback position is the position to use if this collision should revert the movement
            */
            double temp[2][2];
            for (int j = 0; j < 2; ++j) {
                temp[j][0] = 0.0f;
            }
            
            // player coordinates point to the center, but coordinates of block point to upper left corner
      
            // player is on the left side of the block
            if(player->x >= board->iceBlocks[i].x - player->image.w / 2 && player->velX > 0.0f) {
                temp[0][0] = fabs(player->x - (board->iceBlocks[i].x - player->image.w / 2));
                temp[0][1] = board->iceBlocks[i].x - player->image.w / 2;
            }
            // player is on the right side of the block
            if(player->x <= board->iceBlocks[i].x + board->iceBlocks[i].w + player->image.w / 2 && player->velX < 0.0f) { 
                temp[0][0] = fabs(player->x - (board->iceBlocks[i].x + board->iceBlocks[i].w + player->image.w / 2));
                temp[0][1] = board->iceBlocks[i].x + board->iceBlocks[i].w + player->image.w / 2;
            }
            // player above the block
            if(player->y >= board->iceBlocks[i].y - player->image.h / 2 && player->velY > 0.0f) {
                temp[1][0] = fabs(player->y - (board->iceBlocks[i].y - player->image.h / 2));
                temp[1][1] = board->iceBlocks[i].y - player->image.h / 2;
            }
            // player below the block
            if(player->y <= board->iceBlocks[i].y + board->iceBlocks[i].w + player->image.w / 2 && player->velY < 0.0f) {
                temp[1][0] = fabs(player->y - (board->iceBlocks[i].y + board->iceBlocks[i].w + player->image.w / 2));
                temp[1][1] = board->iceBlocks[i].y + board->iceBlocks[i].h + player->image.h / 2;
            }
            
            // choose collision with smaller delta and use proper callback
            if (temp[1][0] == 0 || (temp[0][0] != 0 && temp[0][0] <= temp[1][0]))
                player->x = temp[0][1];
            else
                player->y = temp[1][1];
        }
    }

    // check collisions with breakable ice walls
    for( int i  = 0; i < board->breakableIceBlocksCount; i++){
        if(!SDL_HasIntersection(&collide, board->breakableIceBlocks[i]))
            // no collision happened with ice block
            continue;
        else{
            // collision happened with ice wall
            /*
            TEMP[i][j] - j:             0                                   1
                    i = 0:      x collision delta                   x callback position
                    i = 1:      y collision delta                   y callback position

            collision delta means how deep is the collision
            it is needed to ignore the unwanted dimension when user try to move in both dimensions

            callback position is the position to use if this collision should revert the movement
            */
            double temp[2][2];
            for (int j = 0; j < 2; ++j) {
                temp[j][0] = 0.0f;
            }

            // player coordinates point to the center, but coordinates of block point to upper left corner

            // player is on the left side of the block
            if(player->x >= board->breakableIceBlocks[i]->x - player->image.w / 2 && player->velX > 0.0f) {
                temp[0][0] = fabs(player->x - (board->breakableIceBlocks[i]->x - player->image.w / 2));
                temp[0][1] = board->breakableIceBlocks[i]->x - player->image.w / 2;
            }
            // player is on the right side of the block
            if(player->x <= board->breakableIceBlocks[i]->x + board->breakableIceBlocks[i]->w + player->image.w / 2 && player->velX < 0.0f) {
                temp[0][0] = fabs(player->x - (board->breakableIceBlocks[i]->x + board->breakableIceBlocks[i]->w + player->image.w / 2));
                temp[0][1] = board->breakableIceBlocks[i]->x + board->breakableIceBlocks[i]->w + player->image.w / 2;
            }
            // player above the block
            if(player->y >= board->breakableIceBlocks[i]->y - player->image.h / 2 && player->velY > 0.0f) {
                temp[1][0] = fabs(player->y - (board->breakableIceBlocks[i]->y - player->image.h / 2));
                temp[1][1] = board->breakableIceBlocks[i]->y - player->image.h / 2;
            }
            // player below the block
            if(player->y <= board->breakableIceBlocks[i]->y + board->breakableIceBlocks[i]->w + player->image.w / 2 && player->velY < 0.0f) {
                temp[1][0] = fabs(player->y - (board->breakableIceBlocks[i]->y + board->breakableIceBlocks[i]->w + player->image.w / 2));
                temp[1][1] = board->breakableIceBlocks[i]->y + board->breakableIceBlocks[i]->h + player->image.h / 2;
            }

            // choose collision with smaller delta and use proper callback
            if (temp[1][0] == 0 || (temp[0][0] != 0 && temp[0][0] <= temp[1][0]))
                player->x = temp[0][1];
            else
                player->y = temp[1][1];
        }
    }

    // check if player went out of bomb
    if(player->placedBomb && !SDL_HasIntersection(&collide, &player->bomb->bombRect))
        player->onBomb = 0;

    // collision with bomb
    if(player->placedBomb && !player->onBomb && SDL_HasIntersection(&collide, &player->bomb->bombRect)){
        double temp[2][2];
        for (int j = 0; j < 2; ++j) {
            temp[j][0] = 0.0f;
        }

        // player is on the left side of the bomb
        if(player->x >= player->bomb->bombRect.x - player->image.w / 2 && player->velX > 0.0f) {
            temp[0][0] = fabs(player->x - (player->bomb->bombRect.x - player->image.w / 2));
            temp[0][1] = player->bomb->bombRect.x - player->image.w / 2;
        }
        // player is on the right side of the bomb
        if(player->x <= player->bomb->bombRect.x + player->bomb->bombRect.w + player->image.w / 2 && player->velX < 0.0f) {
            temp[0][0] = fabs(player->x - (player->bomb->bombRect.x + player->bomb->bombRect.w + player->image.w / 2));
            temp[0][1] = player->bomb->bombRect.x + player->bomb->bombRect.w + player->image.w / 2;
        }
        // player above the bomb
        if(player->y >= player->bomb->bombRect.y - player->image.h / 2 && player->velY > 0.0f) {
            temp[1][0] = fabs(player->y - (player->bomb->bombRect.y - player->image.h / 2));
            temp[1][1] = player->bomb->bombRect.y- player->image.h / 2;
        }
        // player below the bomb
        if(player->y <= player->bomb->bombRect.y + player->bomb->bombRect.w + player->image.w / 2 && player->velY < 0.0f) {
            temp[1][0] = fabs(player->y - (player->bomb->bombRect.y+ player->bomb->bombRect.w + player->image.w / 2));
            temp[1][1] = player->bomb->bombRect.y + player->bomb->bombRect.h + player->image.h / 2;
        }

        // choose collision with smaller delta and use proper callback
        if (temp[1][0] == 0 || (temp[0][0] != 0 && temp[0][0] <= temp[1][0]))
            player->x = temp[0][1];
        else
            player->y = temp[1][1];
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
    //render player and bomb/ explosion if it exists
    SDL_RenderCopy(renderer, player->texture, NULL, &player->image);
    if(player->placedBomb)
        renderBomb(player->bomb, renderer);
    if(player->bomb != NULL && player->bomb->exploded)
        renderExplosion(player->bomb, renderer);
}

void placeBombPlayer(Player* player, SDL_Renderer *renderer, Board *board){
    //place bomb in middle of current tile
    if (!player->placedBomb){
        player->placedBomb = 1;
        player->onBomb = 1;
        player->bomb = (Bomb *) malloc(sizeof(Bomb));
        initBomb(player->bomb);
        loadBomb(player->bomb, renderer, board, player->current_tile);
    }
}

void checkBombs(Player* player, Board* board){
    // check if bomb should explode or be destroyed after explosion
    if(player->bomb != NULL){
        //Explode
        if(getTicksTimer(player->bomb->timer) >= 2000){
            if(!player->bomb->exploded)
                explode(player->bomb, board);
        }
        //Destroy bomb
        if(getTicksTimer(player->bomb->timer) >= 3000){
            player->onBomb = 0;
            player->placedBomb = 0;
            closeBomb(player->bomb);
            free(player->bomb);
            player->bomb = NULL;
        }
    }
}

void closePlayer(Player *player) {
    if(player->texture != NULL)
        SDL_DestroyTexture(player->texture);
}