#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>

typedef struct state {
    SDL_Window *window;
    SDL_Renderer *renderer;

    int frames_since_step;
    int das_frames;
    int are_status;
    int are_frames;
} State;

extern State state;

#endif /* STATE_H */
