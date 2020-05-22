#include <stdio.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "conn/connection.h"
#include "player.h"
#include "board.h"
#include "bomb.h"
#include "timers/timer.h"


int main(int argc, char* argv[]) {
    if(argc < 3) {
        fprintf(stderr, "Missing player name and/or port number\n");
        return -1;
    }
    // Initialize window
    if(init() < 0){
        fprintf(stderr, "Error in window init.\n");
    }
    else {
        window->run = SDL_TRUE;
        // Initialize connection to server
        initAllEnemies(3);
        initConnection(argv[1], argv[2]); // name and port as user input
        connectServer();
        while (conn->connectionEstablished == 0); // wait for connection

        // Initialize Board data
        initBoard(window->gWindow, conn->player_count);
        // Loading board data
        loadBoard(window->gWindow, window->gRenderer);

        // Initialize Player data
        //initPlayer(board);
        // Loading player data
        //loadPlayer(window->gWindow, window->gRenderer);

        initAllBombs(4);
        initBomb(bombs[0]);
        // Loading bomb data
        loadBomb(bombs[0], window->gRenderer);

        // Initialize Velocity Timer & Move Timer
        Timer vTimer;
        initTimer(&vTimer);

        Timer moveTimer;
        initTimer(&moveTimer);
        startTimer(&moveTimer);

        // Event handler
        SDL_Event e;
        while (window->run) {
            // Event polling queue
            while (SDL_PollEvent(&e) != 0){

                switch (e.type) {
                    // User request quit
                    case SDL_QUIT:
                        window->run = SDL_FALSE;
                        break;
                    default:
                        if(board->startGame == 1)
                            handlePlayerEvent(&e, window->gRenderer, board, bombs[0]);
                        break;
                }
            }

            if(board->startGame == 1) {
                // Calculate time step -> ms -> s
                double timeStep = getTicksTimer(&vTimer) / 1000.f;
                int stepX = (int) player->x;
                int stepY = (int) player->y;
                // Moving player
                movePlayer(board, bombs[0], timeStep);
                stepX -= (int) player->x;
                stepY -= (int) player->y;
                if (stepX != 0 || stepY != 0) {
                    // player moved, send to server
                    // send not more often than 50 ms
                    if (getTicksTimer(&moveTimer) >= 0.f) {
                        sendPlayerData((int) player->x, (int) player->y, &player->counter);
                        startTimer(&moveTimer);
                    }
                }
                // Restart step timer / velocity timer
                startTimer(&vTimer);

                // Move enemies
                if (conn->player_count > 1)
                    for (int i = 0; i < conn->player_count - 1; i++)
                        moveEnemy(enemies[i]);

                // Clearing renderer
                SDL_RenderClear(window->gRenderer);

                // Render board
                renderBoard(window->gRenderer);

                if (bombs[0]->placed == 1)
                    renderBomb(bombs[0], window->gRenderer);
                checkForExplosion(bombs[0], board);
                if (bombs[0]->exploded == 1)
                    renderExplosion(bombs[0], window->gRenderer);
                if (bombs[0]->exploded == 1 && getTicksTimer(bombs[0]->timer) >= 2000.f) {
                    hideBomb(bombs[0]);
                    player->placedBomb = 0;
                }
                // Render enemies
                if (conn->player_count > 1)
                    for (int i = 0; i < conn->player_count - 1; i++)
                        renderEnemy(enemies[i], window->gRenderer);

                // Render player
                renderPlayer(window->gRenderer);

                // Presenting data in renderer
                SDL_RenderPresent(window->gRenderer);
            }
            else{
                SDL_RenderClear(window->gRenderer);
                renderBoard(window->gRenderer);
                SDL_RenderPresent(window->gRenderer);
            }
        }
        // Freeing resources for rendered elements

        if(board->startGame == 1)
        {
            closePlayer(&player);
            for(int i  = 0; i < conn->player_count-1; i++){
                closeEnemy(enemies[i]);
            }
        }
        closeBomb(bombs[0]);
        closeAllBombs(4);
        closeBoard();
        closeConnection();
        closeSocket();
        closeAllEnemies(3);
    }
    // Free resources and close SDL
    close_window();

    return 0;
}
