#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "conn/connection.h"
#include "player.h"
#include "board.h"
#include "timers/timer.h"


int main() {
    // Initialize window struct
    Window window;
    window.run = SDL_TRUE;
    // Initialize window
    if(init(&window) < 0){
        fprintf(stderr, "Error in init.");
    }
    else {
        // Initialize connection to server
        Connection conn;
        initConnection(&conn, "Kacper", "14938"); // name and port as user input
        connectServer(&conn);
        while (conn.connectionEstablished == 0); // wait for connection
        sendName(&conn);

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
        Timer vTimer;
        initTimer(&vTimer);

        Timer moveTimer;
        initTimer(&moveTimer);
        startTimer(&moveTimer);

        // Event handler
        SDL_Event e;
        while (window.run) {
            // Event polling queue
            while (SDL_PollEvent(&e) != 0){

                switch (e.type) {
                    // User request quit
                    case SDL_QUIT:
                        window.run = SDL_FALSE;
                        break;
                    default:
                        handlePlayerEvent(&player, &e, &conn);
                        break;
                }
            }

            // Calculate time step -> ms -> s
            double timeStep = getTicksTimer(&vTimer) / 1000.f;
            int stepX = (int)player.x;
            int stepY = (int)player.y;
            // Moving player
            movePlayer(&player, &board, timeStep);
            stepX -= (int)player.x;
            stepY -= (int)player.y;
            if(stepX != 0 || stepY != 0){
                // player moved, send to server
                // send not more often than 50 ms
                if(getTicksTimer(&moveTimer) >= 50.f) {
                    sendPlayerData(&conn, (int) player.x, (int) player.y, &player.counter);
                    startTimer(&moveTimer);
                }
            }

            // Restart step timer / velocity timer
            startTimer(&vTimer);

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
        closeConnection(&conn);
        closeSocket(&conn);
    }
    //test_connection();

    // Free resources and close SDL
    close_window(&window);

    return 0;
}
