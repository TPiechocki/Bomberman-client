//
// Created by Kacper on 28/04/2020.
//

#ifndef BOMBERMAN_CLIENT_WINDOW_H
#define BOMBERMAN_CLIENT_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef struct SDLWindowStruct {
    SDL_Window *gWindow; // SDL window handle
    SDL_Surface *gScreenSurface; //  SDL window surface handle
    SDL_Renderer *gRenderer; // SDL renderer handle
    int run; // status of app
}Window;

Window* window; // window struct handle
pthread_mutex_t renderer_lock; // renderer mutex

/**
 * Initializes window struct and SDL window.
 * Renderer mutex is initialised here.
 * @return 0 on success, -1 on error
 */
int init();

/**
 * Renders "Waiting for players..." message.
 */
void renderWaitingMessage();
/**
 * Renders "You Died" message.
 */
void renderKillMessage();
/**
 * Renders "You won!" message.
 */
void renderWinMessage();
/**
 * Renders name above given coordinates to avoid overlap with player character.
 * @param name - name to render
 * @param x - starting coordinate x
 * @param y - starting coordinate y
 */
void renderName(char* name, int x, int y);

/**
 * Clears resources and closes SDL window.
 * Destroys renderer mutex.
 */
void close_window();

#endif //BOMBERMAN_CLIENT_WINDOW_H
