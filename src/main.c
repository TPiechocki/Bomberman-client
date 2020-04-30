#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "conn/connection.h"
#include "player.h"
#include "board.h"
#include "timers/vtimer.h"


int main() {
    // Initialize window struct
    Window window;
    window.run = SDL_TRUE;
    // Initialize window
    if(init(&window) < 0){
        fprintf(stderr, "Error in init.");
    }
    else {
        // Initialize Board data
        Board board;
        initBoard(&board, window.gWindow);
        // Loading board data
        loadBoard(window.gWindow, window.gRenderer, &board);

        // Initialize Player data
        Player player;
        initPlayer(&player, &board);
        // Loading player data
        loadPlayer(window.gWindow, window.gRenderer, &player);

        // Initialize Velocity Timer
        VTimer vTimer;
        initVTimer(&vTimer);

        // Event handler
        SDL_Event e;
        while (window.run) {
            // Event polling queue
            while (SDL_PollEvent(&e) != 0){

                switch (e.type) {
                    /*case SDL_KEYDOWN:
                        changeMove(&player, e);
                        break;
                    case SDL_KEYUP:
                        brake(&player, e);
                        break;*/
                    // User request quit
                    case SDL_QUIT:
                        window.run = SDL_FALSE;
                        break;
                    default:
                        handlePlayerEvent(&player, &e);
                        break;
                }
            }

            // Calculate time step -> ms -> s
            double timeStep = getTicksVTimer(&vTimer) / 1000.f;

            // Moving player
            movePlayer(&player, &board, timeStep);
            //moving(&player);

            // Restart step timer / velocity timer
            startVTimer(&vTimer);

            // Clearing renderer
            SDL_RenderClear(window.gRenderer);

            // Render board
            renderBoard(window.gRenderer, &board);

            // Render player
            renderPlayer(&player, window.gRenderer);

            // Presenting data in renderer
            SDL_RenderPresent(window.gRenderer);
        }
        // Freeing resources for rendered elements
        closeBoard(&board);
        closePlayer(&player);
    }
    //test_connection();

    // Free resources and close SDL
    close_window(&window);

    return 0;
}
