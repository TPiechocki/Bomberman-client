//
// Created by ves on 30/04/2020.
//

#ifndef BOMBERMAN_CLIENT_TIMER_H
#define BOMBERMAN_CLIENT_TIMER_H

#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct timer_struct{
    Uint32 startTicks;
    Uint32 pausedTicks;
    int paused;
    int started;
}Timer;

void initTimer(Timer* timer);
void startTimer(Timer* timer);
void stopTimer(Timer* timer);
void pauseTimer(Timer* timer);
void unpauseTimer(Timer* timer);
Uint32 getTicksTimer(Timer* timer);

#endif //BOMBERMAN_CLIENT_TIMER_H
