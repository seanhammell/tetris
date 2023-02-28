#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>

typedef struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int frames_since_step;
} State;

extern State state;

#endif /* STATE_H */
