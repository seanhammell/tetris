#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>

typedef struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;
} State;

extern State state;

#endif /* STATE_H */
