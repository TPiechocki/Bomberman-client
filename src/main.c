#include <stdio.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "conn/connection.h"
#include "player.h"
#include "board.h"
#include "bomb.h"
#include "timers/timer.h"


int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Missing player name\n");
        return -1;
    }
    // Initialize window
    if(init() < 0){
        fprintf(stderr, "Error in window init.\n");
    }
    else {
        window->run = SDL_TRUE;
        // allocate memory for bombs and enemies
        initAllEnemies(3);
        initAllBombs(4);
        // Initialize connection to server
        initConnection(argv[1]/*, argv[2]*/); // name and port as user input
        connectServer();
        // while (conn->connectionEstablished == 0); // wait for connection

        // Initialize Board data
        initBoard(window->gWindow);
        // Loading board data
        pthread_mutex_lock(&renderer_lock);
        loadBoard(window->gWindow, window->gRenderer);
        pthread_mutex_unlock(&renderer_lock);

        // Initialize Velocity Timer & Move Timer
        Timer vTimer;
        initTimer(&vTimer);

        Timer moveTimer;
        initTimer(&moveTimer);
        startTimer(&moveTimer);

        // Event handler
        SDL_Event e;

        // Main event loop
        while (window->run) {
            // Event polling queue
            while (SDL_PollEvent(&e) != 0){

                switch (e.type) {
                    // User request quit
                    case SDL_QUIT:
                        window->run = SDL_FALSE;
                        break;
                    default:
                        if(board->startGame == 1 && player->isAlive) {
                            pthread_mutex_lock(&renderer_lock);
                            handlePlayerEvent(&e, window->gRenderer, board);
                            pthread_mutex_unlock(&renderer_lock);
                        }
                        break;
                }
            }

            if(board->startGame == 1) {

                if(player->isAlive) {
                    // Calculate time step -> ms -> s
                    double timeStep = getTicksTimer(&vTimer) / 1000.f;
                    int stepX = (int) player->x;
                    int stepY = (int) player->y;
                    // Moving player
                    movePlayer(board, timeStep);
                    stepX -= (int) player->x;
                    stepY -= (int) player->y;
                    if (stepX != 0 || stepY != 0) {
                        // player moved, send to server
                        // send not more often than 0 ms
                        if (getTicksTimer(&moveTimer) >= 0.f) {
                            sendPlayerData((int) player->x, (int) player->y, &player->counter);
                            startTimer(&moveTimer);
                        }
                    }
                    // Restart step timer / velocity timer
                    startTimer(&vTimer);
                }

                // Move enemies
                if (conn->player_count > 1)
                    for (int i = 0; i < conn->player_count - 1; i++)
                        if(enemies[i]->isAlive)
                            moveEnemy(enemies[i]);

                //getAllTiles(player->x, player->y);

                pthread_mutex_lock(&renderer_lock);
                // Clearing renderer
                SDL_RenderClear(window->gRenderer);

                // Render board
                renderBoard(window->gRenderer);

                // bomb rendering and explosions
                pthread_mutex_lock(&bombs_lock);
                for(int i = 0; i < 4; i++){
                    if (bombs[i]->placed == 1)
                        renderBomb(bombs[i], window->gRenderer);
                    checkForExplosion(bombs[i], board);
                    if (bombs[i]->exploded == 1)
                        renderExplosion(bombs[i], window->gRenderer);
                    pthread_mutex_lock(&tick_lock);
                    if (bombs[i]->exploded == 1 && bombs[i]->endOfExplosionTick < actualTick) {
                        hideBomb(bombs[i]);
                        if(player->bombId == i)
                            player->placedBomb = 0;
                    }
                    pthread_mutex_unlock(&tick_lock);
                }
                pthread_mutex_unlock(&bombs_lock);


                // Render enemies
                if (conn->player_count > 1)
                    for (int i = 0; i < conn->player_count - 1; i++){
                        if(enemies[i]->isAlive) {
                            renderEnemy(enemies[i], window->gRenderer);
                            renderName(enemies[i]->name, enemies[i]->image.x, enemies[i]->image.y);
                        }
                    }

                // render loose message on death
                if(player->isAlive == 0)
                    renderKillMessage();
                else {
                    // Render player
                    renderPlayer(window->gRenderer);
                    renderName(conn->name, player->image.x, player->image.y);
                }

                if(player->isAlive && !checkEnemyLives() && conn->player_count > 1)
                    renderWinMessage();

                // Presenting data in renderer
                SDL_RenderPresent(window->gRenderer);
                pthread_mutex_unlock(&renderer_lock);
            }
            else{
                pthread_mutex_lock(&renderer_lock);
                SDL_RenderClear(window->gRenderer);
                renderBoard(window->gRenderer);
                renderWaitingMessage();
                SDL_RenderPresent(window->gRenderer);
                pthread_mutex_unlock(&renderer_lock);
            }
        }
        // Freeing resources for rendered elements
        closeConnection();
        if(board->startGame == 1)
        {
            closePlayer(&player);
            for(int i  = 0; i < conn->player_count-1; i++)
                closeEnemy(enemies[i]);
            for(int i = 0; i < conn->player_count; i++)
                closeBomb(bombs[i]);
        }
        closeConnStruct();
        closeBoard();
        closeAllBombs(4);
        closeAllEnemies(3);
    }
    // Free resources and close SDL
    close_window();

    return 0;
}
