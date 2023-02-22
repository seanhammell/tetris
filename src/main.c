#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "src/state.h"
#include "src/texture.h"
#include "src/tetrimino.h"

#define R 236
#define G 239
#define B 244
#define A 255

State state;

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

    state.window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_SHOWN);
    if (state.window == NULL) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
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
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);

    state.renderer = NULL;
    state.window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int arg, char *argv[])
{
    if (initialize() != 0) {
        fprintf(stderr, "Failed to initialize\n");
        cleanup();
        return 0;
    }

    Texture *blocks = texture_create();
    if (texture_initialize(blocks, "./img/tetriminoes.png") != 0) {
        fprintf(stderr, "Failed to initialize texture\n");
        texture_destroy(blocks);
        cleanup();
        return 0;
    }

    Tetrimino *O = tetrimino_create();
    tetrimino_initialize(O, 1);
    tetrimino_set_position(O, 56, 56);

    Tetrimino *I = tetrimino_create();
    tetrimino_initialize(I, 2);
    tetrimino_set_position(I, 256, 56);

    Tetrimino *Z = tetrimino_create();
    tetrimino_initialize(Z, 3);
    tetrimino_set_position(Z, 456, 56);

    Tetrimino *S = tetrimino_create();
    tetrimino_initialize(S, 4);
    tetrimino_set_position(S, 56, 256);

    Tetrimino *T = tetrimino_create();
    tetrimino_initialize(T, 5);
    tetrimino_set_position(T, 256, 256);

    Tetrimino *L = tetrimino_create();
    tetrimino_initialize(L, 6);
    tetrimino_set_position(L, 456, 256);

    Tetrimino *J = tetrimino_create();
    tetrimino_initialize(J, 7);
    tetrimino_set_position(J, 256, 456);

    Tetrimino *minoes[7] = {O, I, Z, S, T, L, J};

    SDL_Event event;
    for (;;) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                for (int i = 0; i < 7; ++i) {
                    tetrimino_destroy(minoes[i]);
                }
                texture_destroy(blocks);
                cleanup();
                return 0;
            }

            if (event.type == SDL_KEYDOWN) {
                int i;
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    for (i = 0; i < 7; ++i) {
                        tetrimino_rotate(minoes[i]);
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(state.renderer, R, G, B, A);
        SDL_RenderClear(state.renderer);
        for (int i = 0; i < 7; ++i) {
            tetrimino_render(minoes[i], blocks);
        }
        SDL_RenderPresent(state.renderer);
    }

    return 0;
}
