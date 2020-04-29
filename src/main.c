#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "conn/connection.h"
#include "player.h"
#include "board.h"


int main() {
    // Initialize window struct
    Window window;
    window.run = SDL_TRUE;
    // Initialize window
    if(init(&window) < 0){
        fprintf(stderr, "Error in init.");
    }
    else {
        // Player struct
        Player p;
        // Loading player data
        loadPlayer(window.gWindow, window.gRenderer, &p);
        Board board;
        LoadBoard(window.gWindow, window.gRenderer, &board);
        // Event handler
        SDL_Event e;
        while (window.run) {
            // Event polling queue
            while (SDL_PollEvent(&e) != 0){
                // User request quit
                switch (e.type) {
                    case SDL_QUIT:
                        window.run = SDL_FALSE;
                        break;
                    default:
                        break;
                }
            }

            // Rendering
            SDL_RenderClear(window.gRenderer);
            SDL_RenderCopy(window.gRenderer, p.texture, NULL, &p.image);
                for(int i = 0; i < 4; i++)
                   SDL_RenderCopy(window.gRenderer, board.outsideWallTexture, NULL, &board.outsideWalls[i]);
                for(int i = 0; i < 36; i++)
                    SDL_RenderCopy(window.gRenderer, board.iceBlockTexture, NULL, &board.iceBlocks[i]);
            SDL_RenderPresent(window.gRenderer);
        }
    }
    //test_connection();

    // Free resources and close SDL
    close_window(&window);

    return 0;
}
