//
// Created by ves on 30/04/2020.
//

#include "timer.h"

void initTimer(Timer *timer) {
    timer->startTicks = 0;
    timer->pausedTicks = 0;
    // Pseudo booleans
    timer->paused = 0;
    timer->started = 0;
}

void startTimer(Timer *timer) {
    // Timer started
    timer->started = 1;
    timer->paused = 0;
    // Get current clock time
    timer->startTicks = SDL_GetTicks();
    timer->pausedTicks = 0;
}

void stopTimer(Timer *timer) {
    // Timer stopped
    timer->started = 0;
    timer->paused = 0;
    // Resetting timer
    timer->startTicks = 0;
    timer->pausedTicks = 0;
}

void pauseTimer(Timer *timer) {
    // Check if timer is running
    if(timer->started == 1 && timer->paused == 0){
        timer->paused = 1;

        timer->pausedTicks = SDL_GetTicks() - timer->startTicks;
        timer->startTicks = 0;
    }
}

void unpauseTimer(Timer *timer) {
    if(timer->started == 1 && timer->paused == 1){
        timer->paused = 0;

        timer->startTicks = SDL_GetTicks() - timer->pausedTicks;
        timer->pausedTicks = 0;
    }
}

Uint32 getTicksTimer(Timer *timer) {
    Uint32 time = 0;
    if(timer->started == 1){
        if(timer->paused == 1)
            time = timer->pausedTicks;
        else
            time = SDL_GetTicks() - timer->startTicks;
    }
    return time;
}

