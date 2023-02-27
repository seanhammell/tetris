#ifndef STATE_H
#define STATE_H

#include <stdint.h>

#include <SDL2/SDL.h>

typedef struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint32_t ticks;
} State;

extern State state;

#endif /* STATE_H */
