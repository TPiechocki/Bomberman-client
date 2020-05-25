//
// Created by ves on 28/04/2020.
//

#include "window.h"

int init(){
    window = (Window*)malloc(sizeof(Window));

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
    SDL_SetRenderDrawColor(window->gRenderer, 0x77, 0x77, 0x77, 0xff);

    // Get window surface
    window->gScreenSurface = SDL_GetWindowSurface(window->gWindow);

    // Initialise mutex
    pthread_mutex_init(&renderer_lock, NULL);

    TTF_Init();

    return 0;
}

void renderWaitingMessage(){
    // Choosing font
    TTF_Font* sans = TTF_OpenFont("../sprites/OpenSans-Regular.ttf", 200);
    char* text = "Waiting for players to join...";
    // Choosing text colour - black
    SDL_Color textColor = {0, 0, 0};
    // Creating texture from the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(sans, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(window->gRenderer, textSurface);

    // Bounding box of the text
    SDL_Rect textRect = {100, 100, WINDOW_WIDTH - 200, 200};

    // Rendering text
    SDL_RenderCopy(window->gRenderer, textTexture, NULL, &textRect);

    // Freeing resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(sans);
}

void renderKillMessage(){
    // Choosing font
    TTF_Font* sans = TTF_OpenFont("../sprites/Walter.ttf", 200);
    char* text = "You Died";
    // Choosing text colour - dark red
    SDL_Color textColor = {125, 0, 0};
    // Creating texture from the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(sans, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(window->gRenderer, textSurface);
    // Bounding box of the text
    SDL_Rect textRect = {200, 100, WINDOW_WIDTH - 400, 200};
    // Rendering text
    SDL_RenderCopy(window->gRenderer, textTexture, NULL, &textRect);
    // Freeing resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(sans);
}

void renderWinMessage(){
    // Choosing font
    TTF_Font* sans = TTF_OpenFont("../sprites/OptimusPrincepsSemiBold.ttf", 200);
    char* text = "You won!";
    // Choosing text colour - dark green
    SDL_Color textColor = {0, 125, 0};
    // Creating texture from the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(sans, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(window->gRenderer, textSurface);
    // Bounding box of the text
    SDL_Rect textRect = {200, 100, WINDOW_WIDTH - 400, 200};
    // Rendering text
    SDL_RenderCopy(window->gRenderer, textTexture, NULL, &textRect);
    // Freeing resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(sans);
}

void renderName(char *name, int x, int y) {
    // Choosing font
    TTF_Font* sans = TTF_OpenFont("../sprites/OpenSans-Regular.ttf", 50);
    // Choosing text colour - black
    SDL_Color textColor = {0, 0, 0};
    // Creating texture from the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(sans, name, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(window->gRenderer, textSurface);
    // Bounding box of the name, 30 pixels above so it doesn't overlap
    SDL_Rect textRect = {x, y - 30, strlen(name)*10, 25};
    // Rendering text
    SDL_RenderCopy(window->gRenderer, textTexture, NULL, &textRect);
    // Freeing resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(sans);
}

void close_window(){
    // Destroy renderer
    SDL_DestroyRenderer(window->gRenderer);
    window->gRenderer = NULL;

    // Destroy window
    SDL_DestroyWindow(window->gWindow);
    window->gWindow = NULL;

    pthread_mutex_destroy(&renderer_lock);

    TTF_Quit();

    // Quit SDL subsystems
    SDL_Quit();
    free(window);
}
