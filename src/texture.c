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

    if (self->texture == NULL) {
        return 1;
    }

    return 0;
}

/**
 * Sets the clips for the Texture based on the number of rows and columns,
 * and the width and height of the sprites.
 */
void texture_set_clips(Texture *self, const int n_rows, const int n_cols, const int width, const int height)
{
    for (int r = 0; r < n_rows; ++r) {
        for (int c = 0; c < n_cols; ++c) {
            const int i = n_cols * r + c;
            self->clips[i].x = width * c;
            self->clips[i].y = height * r;
            self->clips[i].w = width;
            self->clips[i].h = height;
        }
    }
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
void texture_render(const Texture *self, const int clip, const int x, const int y)
{
    SDL_Rect dest = { x, y, self->width, self->height };
    dest.w = self->clips[clip].w;
    dest.h = self->clips[clip].h;
    SDL_RenderCopy(state.renderer, self->texture, &self->clips[clip], &dest);
}
