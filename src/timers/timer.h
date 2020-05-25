//
// Created by ves on 30/04/2020.
//

#ifndef BOMBERMAN_CLIENT_TIMER_H
#define BOMBERMAN_CLIENT_TIMER_H

#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct timer_struct{
    Uint32 startTicks; // time when timer was started on
    Uint32 pausedTicks; // time when timer was paused on
    int paused; // status of timer; if timer is paused
    int started; // status of timer; if timer is started
}Timer;

/**
 * Initialises timer structure
 * @param timer - timer to initialise
 */
void initTimer(Timer* timer);

/**
 * Starts timer.
 * @param timer - timer to start
 */
void startTimer(Timer* timer);

/**
 * Stops timer.
 * @param timer - timer to stop
 */
void stopTimer(Timer* timer);

/**
 * Pauses timer.
 * @param timer - timer to pause
 */
void pauseTimer(Timer* timer);

/**
 * Unpauses timer.
 * @param timer - timer to unpause
 */
void unpauseTimer(Timer* timer);

/**
 * Returns time passed since starting timer.
 * Time given in milliseconds.
 * @param timer - timer to check
 * @return time in milliseconds; 0 - timer is stopped
 */
Uint32 getTicksTimer(Timer* timer);

#endif //BOMBERMAN_CLIENT_TIMER_H
