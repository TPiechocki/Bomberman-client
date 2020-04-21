#include <stdio.h>
#include "player.h"
#include "board.h"
#include "conn/connection.h"



int main() {
    SDL_Window *window;                    // Declare a pointer
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
            "Bomberman",                  // window title
            SDL_WINDOWPOS_UNDEFINED,           // initial x position
            SDL_WINDOWPOS_UNDEFINED,           // initial y position
            640,                               // width, in pixels
            640,                               // height, in pixels
            SDL_WINDOW_OPENGL                  // flags - see below
    );

    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    Board board;
    Player p;
    LoadBoard(window, renderer, &board);
    LoadPlayer(renderer, &p);
    while (1)
    {
        // Get the next event
        SDL_Event event;

        if (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
            {
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, p.texture, NULL, &p.image);
        for(int i = 0; i < 4; i++)
            SDL_RenderCopy(renderer, board.outsideWallTexture, NULL, &board.outsideWalls[i]);
        SDL_RenderPresent(renderer);
    }



    // Close and destroy the window
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();

    //test_connection();

    return 0;
}
