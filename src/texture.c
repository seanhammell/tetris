#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "src/state.h"

#ifndef TEXTURE_INTERNAL
#define TEXTURE_INTERNAL
typedef struct texture {
    SDL_Texture *texture;
    int width;
    int height;
    SDL_Rect clips[16];
} Texture;
#endif /* TEXTURE_INTERNAL */

#include "src/texture.h"

/**
 * Resets the members of a Texture object, if applicable.
 */
void reset(Texture *self)
{
    if (self->texture != NULL) {
        SDL_DestroyTexture(self->texture);
        self->texture = NULL;
        self->width = 0;
        self->height = 0;
    }
}

/**
 * Creates a new Texture object.
 */
Texture *texture_create(void)
{
    Texture *self = malloc(sizeof(Texture));
    self->texture = NULL;
    self->width = 0;
    self->height = 0;
    return self;
}

/**
 * Initializes the Texture with the image at the provided location.
 */
int texture_initialize(Texture *self, const char *path)
{
    reset(self);

    SDL_Surface *sprite_sheet = IMG_Load(path);
    if (sprite_sheet == NULL) {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *temp = SDL_CreateTextureFromSurface(state.renderer, sprite_sheet);
    if (temp == NULL) {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(sprite_sheet);
        return 1;
    }

    self->texture = temp;
    self->width = sprite_sheet->w;
    self->height = sprite_sheet->h;

    SDL_FreeSurface(sprite_sheet);

    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 4; ++c) {
            const int i = 4 * r + c;
            self->clips[i].x = 16 * c;
            self->clips[i].y = 16 * r;
            self->clips[i].w = 16;
            self->clips[i].h = 16;
        }
    }

    if (self->texture == NULL) {
        return 1;
    }

    return 0;
}

/**
 * Destroys the Texture object.
 */
void texture_destroy(Texture *self)
{
    reset(self);
    free(self);
    self = NULL;
}

/**
 * Renders the Texture to the screen.
 */
void texture_render(const Texture *self)
{
    SDL_Rect dest = { 0, 0, self->width, self->height };
    SDL_RenderCopy(state.renderer, self->texture, NULL, &dest);
}
