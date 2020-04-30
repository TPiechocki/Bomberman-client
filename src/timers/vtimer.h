//
// Created by ves on 30/04/2020.
//

#ifndef BOMBERMAN_CLIENT_VTIMER_H
#define BOMBERMAN_CLIENT_VTIMER_H

#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct VelocityTimer{
    Uint32 startTicks;
    Uint32 pausedTicks;
    int paused;
    int started;
}VTimer;

void initVTimer(VTimer* vTimer);
void startVTimer(VTimer* vTimer);
void stopVTimer(VTimer* vTimer);
void pauseVTimer(VTimer* vTimer);
void unpauseVTimer(VTimer* vTimer);
Uint32 getTicksVTimer(VTimer* vTimer);

#endif //BOMBERMAN_CLIENT_VTIMER_H
