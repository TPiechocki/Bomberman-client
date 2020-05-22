//
// Created by Kacper on 28/04/2020.
//

#ifndef BOMBERMAN_CLIENT_WINDOW_H
#define BOMBERMAN_CLIENT_WINDOW_H

#include <SDL2/SDL.h>
#include <pthread.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef struct SDLWindowStruct {
    SDL_Window *gWindow;
    SDL_Surface *gScreenSurface;
    SDL_Renderer *gRenderer;
    int run;
}Window;

Window* window;
pthread_mutex_t renderer_lock;

/**
 * Initializes window struct and SDL window.
 * @return 0 on success, -1 on error
 */
int init();

/**
 * Clears resources and closes SDL window.
 */
void close_window();

#endif //BOMBERMAN_CLIENT_WINDOW_H
