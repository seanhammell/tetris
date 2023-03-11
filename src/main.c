#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "src/state.h"
#include "src/texture.h"
#include "src/tetris.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 576

#define R 236
#define G 239
#define B 244
#define A 255

State state;

static Texture *background;
static Texture *blocks;
static Texture *numbers;

/**
 * Initializes SDL and the global state.
 */
int initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        fprintf(stderr, "Warning: linear texture filtering not enabled\n");
    }

    state.window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (state.window == NULL) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (state.renderer == NULL) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetRenderDrawColor(state.renderer, R, G, B, A);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
        return 1;
    }

    return 0;
}

/**
 * Cleans up the global state and quits SDL.
 */
void cleanup(void)
{
    texture_destroy(blocks);
    texture_destroy(background);

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);

    state.renderer = NULL;
    state.window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int arg, char *argv[])
{
    background = texture_create();
    blocks = texture_create();
    numbers = texture_create();

    if (initialize() != 0) {
        fprintf(stderr, "Failed to initialize\n");
        goto terminate;
    }

    if (texture_initialize(background, "./img/background.png") !=  0) {
        fprintf(stderr, "Failed to initialize background texture\n");
        goto terminate;
    }

    if (texture_initialize(blocks, "./img/tetriminoes.png") != 0) {
        fprintf(stderr, "Failed to initialize blocks texture\n");
        goto terminate;
    }

    if (texture_initialize(numbers, "./img/numbers.png") != 0) {
        fprintf(stderr, "Failed to initialize numbers texture\n");
        goto terminate;
    }

    texture_set_clips(background, 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);
    texture_set_clips(blocks, 2, 4, 32, 32);
    texture_set_clips(numbers, 2, 5, 32, 32);

    Tetris *tetris = tetris_create();
    tetris_initialize(tetris);

    SDL_Event event;
    state.frames_since_step = 0;
    state.das_frames = 0;
    state.are_frames = 0;
    int are_frame_delay = 0;
    int cleared_lines[4] = {0};
    for (;;) {
        if (are_frame_delay == 0) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    tetris_destroy(tetris);
                    goto terminate;
                }

                tetris_handle_event(tetris, event);
            }
            are_frame_delay = tetris_apply_gravity(tetris, cleared_lines);
        }

        SDL_SetRenderDrawColor(state.renderer, R, G, B, A);
        SDL_RenderClear(state.renderer);

        texture_render(background, 0, 0, 0);
        tetris_render(tetris, blocks, numbers, are_frame_delay);

        if (are_frame_delay == 93 && (state.are_frames / 12) % 2) {
            for (int i = 0; cleared_lines[i] != 0; ++i) {
                SDL_Rect flash = {64, 32 * cleared_lines[i], 320, 32};
                SDL_SetRenderDrawColor(state.renderer, 76, 86, 106, A);
                SDL_RenderFillRect(state.renderer, &flash);
            }
        }

        SDL_RenderPresent(state.renderer);
        ++state.frames_since_step;
        ++state.das_frames;
        ++state.are_frames;

        if (are_frame_delay && state.are_frames > are_frame_delay) {
            tetris_clear_lines(tetris, cleared_lines);
            tetris_next_tetrimino(tetris);
            are_frame_delay = 0;
            for (int i = 0; i < 4; ++i) {
                cleared_lines[i] = 0;
            }
        }
    }

terminate:
    cleanup();
    return 0;
}
