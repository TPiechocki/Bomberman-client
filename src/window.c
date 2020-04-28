//
// Created by ves on 28/04/2020.
//

#include "window.h"

int init(Window* window){

    window->gWindow = NULL;
    window->gScreenSurface = NULL;
    window->gRenderer = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }

    // Create window
    window->gWindow = SDL_CreateWindow(
            "Bomberman",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN // | SDL_WINDOW_RESIZABLE
            );
    if(window->gWindow == NULL){
        fprintf(stderr, "SDL could not create window! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }

    // Create window renderer
    window->gRenderer = SDL_CreateRenderer(window->gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(window->gRenderer == NULL){
        fprintf(stderr, "SDL could not create renderer! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }

    // Set default renderer draw colour
    SDL_SetRenderDrawColor(window->gRenderer, 0xcc, 0xcc, 0xcc, 0xff);

    // Get window surface
    window->gScreenSurface = SDL_GetWindowSurface(window->gWindow);
    return 0;
}

void close_window(Window* window){
    // Destroy window
    SDL_DestroyWindow(window->gWindow);
    window->gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}