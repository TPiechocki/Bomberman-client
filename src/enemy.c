//
// Created by pioter on 20.05.2020.
//
#include "enemy.h"


void initEnemy(Enemy *enemy, Board *board, int startX, int startY) {
    double tilesPerSecond = 3;

    enemy->velocity = tilesPerSecond * (double)board->length / board->size; // tile per second
    enemy->velX = 0;
    enemy->velY = 0;

    enemy->onBomb = 0;
    enemy->placedBomb = 0;

    enemy->image.w = board->length / board->size * 7 / 10;
    enemy->image.h = enemy->image.w;

    enemy->x = startX;
    enemy->y = startY;
    enemy->image.x = (int)enemy->x - enemy->image.w / 2;
    enemy->image.y = (int)enemy->y - enemy->image.w / 2;
}

void loadEnemy(SDL_Window *window, SDL_Renderer *renderer, Enemy *enemy)
{
    SDL_Surface *surface = IMG_Load("./../sprites/enemy.png");
    if(surface == NULL) {
        printf("Blad przy wczytywaniu plikow!");
        return;
    }

    enemy->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void moveEnemy(Enemy* enemy, Board* board, Bomb* bombs, double timeStep){
    // Scale for diagonal movement
    double scale = 1.;
    // If the diagonal movement occurs, scale it down
    if(enemy->velX != 0 && enemy->velY != 0)
        scale = 1./sqrt(2);

    // Move left or right
    enemy->x += enemy->velX * scale * timeStep;
    // Move up or down
    enemy->y += enemy->velY * scale * timeStep;

    SDL_Rect collide = enemy->image;
    collide.x = (int)enemy->x - enemy->image.w / 2;
    collide.y = (int)enemy->y - enemy->image.h / 2;
    // check collisions with walls
    for(int i = 0; i < 4; i++){
        if(!SDL_HasIntersection(&collide, &board->outsideWalls[i]))
            // no collision happened with wall
            continue;
        else{
            // collision happened with outside wall
            if(i == 0) // top wall
                enemy->y = board->start_y + enemy->image.h / 2;
            else if(i == 1) // bottom wall
                enemy->y = board->end_y - enemy->image.h / 2;
            if(i == 2) // left wall
                enemy->x = board->start_x + enemy->image.w / 2;
            else if(i == 3) // right wall
                enemy->x = board->end_x - enemy->image.w / 2;
        }
    }

    // check collisions with ice walls
    for( int i  = 0; i < board->iceBlocksCount; i++){
        if(!SDL_HasIntersection(&collide, &board->iceBlocks[i]))
            // no collision happened with ice block
            continue;
        else{
            double temp[2][2];
            for (int j = 0; j < 2; ++j) {
                temp[j][0] = 0.0f;
            }

            // player coordinates point to the center, but coordinates of block point to upper left corner

            // player is on the left side of the block
            if(enemy->x >= board->iceBlocks[i].x - enemy->image.w / 2 && enemy->velX > 0.0f) {
                temp[0][0] = fabs(enemy->x - (board->iceBlocks[i].x - enemy->image.w / 2));
                temp[0][1] = board->iceBlocks[i].x - enemy->image.w / 2;
            }
            // player is on the right side of the block
            if(enemy->x <= board->iceBlocks[i].x + board->iceBlocks[i].w + enemy->image.w / 2 && enemy->velX < 0.0f) {
                temp[0][0] = fabs(enemy->x - (board->iceBlocks[i].x + board->iceBlocks[i].w + enemy->image.w / 2));
                temp[0][1] = board->iceBlocks[i].x + board->iceBlocks[i].w + enemy->image.w / 2;
            }
            // player above the block
            if(enemy->y >= board->iceBlocks[i].y - enemy->image.h / 2 && enemy->velY > 0.0f) {
                temp[1][0] = fabs(enemy->y - (board->iceBlocks[i].y - enemy->image.h / 2));
                temp[1][1] = board->iceBlocks[i].y - enemy->image.h / 2;
            }
            // player below the block
            if(enemy->y <= board->iceBlocks[i].y + board->iceBlocks[i].w + enemy->image.w / 2 && enemy->velY < 0.0f) {
                temp[1][0] = fabs(enemy->y - (board->iceBlocks[i].y + board->iceBlocks[i].w + enemy->image.w / 2));
                temp[1][1] = board->iceBlocks[i].y + board->iceBlocks[i].h + enemy->image.h / 2;
            }

            // choose collision with smaller delta and use proper callback
            if (temp[1][0] == 0 || (temp[0][0] != 0 && temp[0][0] <= temp[1][0]))
                enemy->x = temp[0][1];
            else
                enemy->y = temp[1][1];
        }
    }

    // check collisions with breakable ice walls
    for( int i  = 0; i < board->breakableIceBlocksCount; i++){
        if(!SDL_HasIntersection(&collide, board->breakableIceBlocks[i]))
            // no collision happened with ice block
            continue;
        else{
            // collision happened with ice wall
            double temp[2][2];
            for (int j = 0; j < 2; ++j) {
                temp[j][0] = 0.0f;
            }

            // player coordinates point to the center, but coordinates of block point to upper left corner

            // player is on the left side of the block
            if(enemy->x >= board->breakableIceBlocks[i]->x - enemy->image.w / 2 && enemy->velX > 0.0f) {
                temp[0][0] = fabs(enemy->x - (board->breakableIceBlocks[i]->x - enemy->image.w / 2));
                temp[0][1] = board->breakableIceBlocks[i]->x - enemy->image.w / 2;
            }
            // player is on the right side of the block
            if(enemy->x <= board->breakableIceBlocks[i]->x + board->breakableIceBlocks[i]->w + enemy->image.w / 2 && enemy->velX < 0.0f) {
                temp[0][0] = fabs(enemy->x - (board->breakableIceBlocks[i]->x + board->breakableIceBlocks[i]->w + enemy->image.w / 2));
                temp[0][1] = board->breakableIceBlocks[i]->x + board->breakableIceBlocks[i]->w + enemy->image.w / 2;
            }
            // player above the block
            if(enemy->y >= board->breakableIceBlocks[i]->y - enemy->image.h / 2 && enemy->velY > 0.0f) {
                temp[1][0] = fabs(enemy->y - (board->breakableIceBlocks[i]->y - enemy->image.h / 2));
                temp[1][1] = board->breakableIceBlocks[i]->y - enemy->image.h / 2;
            }
            // player below the block
            if(enemy->y <= board->breakableIceBlocks[i]->y + board->breakableIceBlocks[i]->w + enemy->image.w / 2 && enemy->velY < 0.0f) {
                temp[1][0] = fabs(enemy->y - (board->breakableIceBlocks[i]->y + board->breakableIceBlocks[i]->w + enemy->image.w / 2));
                temp[1][1] = board->breakableIceBlocks[i]->y + board->breakableIceBlocks[i]->h + enemy->image.h / 2;
            }

            // choose collision with smaller delta and use proper callback
            if (temp[1][0] == 0 || (temp[0][0] != 0 && temp[0][0] <= temp[1][0]))
                enemy->x = temp[0][1];
            else
                enemy->y = temp[1][1];
        }
    }

    // check if player went out of bomb
    if(enemy->placedBomb && !SDL_HasIntersection(&collide, &bombs->bombRect))
        enemy->onBomb = 0;

    // collision with bomb
    if(enemy->placedBomb && !enemy->onBomb && SDL_HasIntersection(&collide, &bombs->bombRect)){
        double temp[2][2];
        for (int j = 0; j < 2; ++j) {
            temp[j][0] = 0.0f;
        }

        // player is on the left side of the bomb
        if(enemy->x >= bombs->bombRect.x - enemy->image.w / 2 && enemy->velX > 0.0f) {
            temp[0][0] = fabs(enemy->x - (bombs->bombRect.x - enemy->image.w / 2));
            temp[0][1] = bombs->bombRect.x - enemy->image.w / 2;
        }
        // player is on the right side of the bomb
        if(enemy->x <= bombs->bombRect.x + bombs->bombRect.w + enemy->image.w / 2 && enemy->velX < 0.0f) {
            temp[0][0] = fabs(enemy->x - (bombs->bombRect.x + bombs->bombRect.w + enemy->image.w / 2));
            temp[0][1] = bombs->bombRect.x + bombs->bombRect.w + enemy->image.w / 2;
        }
        // player above the bomb
        if(enemy->y >= bombs->bombRect.y - enemy->image.h / 2 && enemy->velY > 0.0f) {
            temp[1][0] = fabs(enemy->y - (bombs->bombRect.y - enemy->image.h / 2));
            temp[1][1] = bombs->bombRect.y- enemy->image.h / 2;
        }
        // player below the bomb
        if(enemy->y <= bombs->bombRect.y + bombs->bombRect.w + enemy->image.w / 2 && enemy->velY < 0.0f) {
            temp[1][0] = fabs(enemy->y - (bombs->bombRect.y+ bombs->bombRect.w + enemy->image.w / 2));
            temp[1][1] = bombs->bombRect.y + bombs->bombRect.h + enemy->image.h / 2;
        }

        // choose collision with smaller delta and use proper callback
        if (temp[1][0] == 0 || (temp[0][0] != 0 && temp[0][0] <= temp[1][0]))
            enemy->x = temp[0][1];
        else
            enemy->y = temp[1][1];
    }
    // assign new coords if they didn't go out of bounds
    enemy->image.x = (int)enemy->x - enemy->image.w / 2;
    enemy->image.y = (int)enemy->y - enemy->image.h / 2;

}



void renderEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, enemy->texture, NULL, &enemy->image);
}

void closeEnemy(Enemy *enemy) {
    if(enemy->texture != NULL)
        SDL_DestroyTexture(enemy->texture);
}
