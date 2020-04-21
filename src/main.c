#include <stdio.h>
#include "player.c"
#include "conn/connection.h"



int main() {
    SDL_Window *window;                    // Declare a pointer
    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
            "Bomberman",                  // window title
            SDL_WINDOWPOS_UNDEFINED,           // initial x position
            SDL_WINDOWPOS_UNDEFINED,           // initial y position
            640,                               // width, in pixels
            480,                               // height, in pixels
            SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }


    Player p;
    LoadPlayer(renderer, p, "..//sprites//kwadrat.png");
    while (1)
    {
        // Get the next event
        SDL_Event event;
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }
    }



    // Close and destroy the window
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();

    //test_connection();

    return 0;
}
