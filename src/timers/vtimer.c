//
// Created by ves on 30/04/2020.
//

#include "vtimer.h"

void initVTimer(VTimer *vTimer) {
    vTimer->startTicks = 0;
    vTimer->pausedTicks = 0;
    // Pseudo booleans
    vTimer->paused = 0;
    vTimer->started = 0;
}

void startVTimer(VTimer *vTimer) {
    // Timer started
    vTimer->started = 1;
    vTimer->paused = 0;
    // Get current clock time
    vTimer->startTicks = SDL_GetTicks();
    vTimer->pausedTicks = 0;
}

void stopVTimer(VTimer *vTimer) {
    // Timer stopped
    vTimer->started = 0;
    vTimer->paused = 0;
    // Resetting timer
    vTimer->startTicks = 0;
    vTimer->pausedTicks = 0;
}

void pauseVTimer(VTimer *vTimer) {
    // Check if timer is running
    if(vTimer->started == 1 && vTimer->paused == 0){
        vTimer->paused = 1;

        vTimer->pausedTicks = SDL_GetTicks() - vTimer->startTicks;
        vTimer->startTicks = 0;
    }
}

void unpauseVTimer(VTimer *vTimer) {
    if(vTimer->started == 1 && vTimer->paused == 1){
        vTimer->paused = 0;

        vTimer->startTicks = SDL_GetTicks() - vTimer->pausedTicks;
        vTimer->pausedTicks = 0;
    }
}

Uint32 getTicksVTimer(VTimer *vTimer) {
    Uint32 time = 0;
    if(vTimer->started == 1){
        if(vTimer->paused == 1)
            time = vTimer->pausedTicks;
        else
            time = SDL_GetTicks() - vTimer->startTicks;
    }
    return time;
}

