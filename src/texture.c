#include <stdlib.h>

#include <SDL2/SDL.h>

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
 * Destroys a Texture object.
 */
void texture_destroy(Texture *self)
{
    reset(self);
    free(self);
    self = NULL;
}

#include "src/texture.h"