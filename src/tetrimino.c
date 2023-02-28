#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "src/texture.h"

#ifndef TETRIMINO_INTERNAL
#define TETRIMINO_INTERNAL
typedef struct tetrimino {
    int block_type;
    int current_rotation;
    int x;
    int y;
    SDL_Rect bounds[4];
} Tetrimino;
#endif /* TETRIMINO_INTERNAL */

enum blocks {
    EMPTY,
    O,
    I,
    Z,
    S,
    T,
    L,
    J
};

#include "src/tetrimino.h"

static const uint16_t empty_rotations[4];

static const uint16_t O_rotations[4] = {
    0x660,
    0x660,
    0x660,
    0x660,
};

static const uint16_t I_rotations[4] = {
    0xF00,
    0x4444,
    0xF0,
    0x2222,
};

static const uint16_t Z_rotations[4] = {
    0x360,
    0x462,
    0x36,
    0x231,
};

static const uint16_t S_rotations[4] = {
    0x630,
    0x264,
    0x63,
    0x132,
};

static const uint16_t T_rotations[4] = {
    0x270,
    0x262,
    0x72,
    0x232,
};

static const uint16_t L_rotations[4] = {
    0x470,
    0x226,
    0x71,
    0x322,
};

static const uint16_t J_rotations[4] = {
    0x170,
    0x622,
    0x74,
    0x223,
};

static const uint16_t *tetrimino_rotations[8] = {
    empty_rotations,
    O_rotations,
    I_rotations,
    Z_rotations,
    S_rotations,
    T_rotations,
    L_rotations,
    J_rotations,
};

/**
 * Creates a new Tetrimino object.
 */
Tetrimino *tetrimino_create(void)
{
    Tetrimino *self = malloc(sizeof(Tetrimino));
    self->block_type = EMPTY;
    self->current_rotation = 0;
    self->x = 0;
    self->y = 0;
    return self;
}

/**
 * Destroys the Tetrimino object.
 */
void tetrimino_destroy(Tetrimino *self)
{
    free(self);
    self = NULL;
}

/**
 * Returns the block type of the Tetrimino.
 */
int tetrimino_get_block_type(const Tetrimino *self)
{
    return self->block_type;
}

/**
 * Returns the current rotation of the Tetrimino.
 */
uint16_t tetrimino_get_rotation(const Tetrimino *self)
{
    return tetrimino_rotations[self->block_type][self->current_rotation];
}

/**
 * Returns the x position of the Tetrimino.
 */
int tetrimino_get_x_pos(const Tetrimino *self)
{
    return self->x;
}

/**
 * Returns the y position of the Tetrimino.
 */
int tetrimino_get_y_pos(const Tetrimino *self)
{
    return self->y;
}

/**
 * Sets the block type of the Tetrimino.
 */
void tetrimino_set_block_type(Tetrimino *self, const int block_type)
{
    self->block_type = block_type;
}

/**
 * Sets the bounds of the Tetrimino based on the current rotation.
 */
void set_bounds(Tetrimino *self)
{
    uint16_t mino_bits = tetrimino_rotations[self->block_type][self->current_rotation];
    uint16_t bit = 0x1000;
    int n_bounds = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (mino_bits & bit) {
                SDL_Rect bound;
                bound.x = self->x + 32 * j;
                bound.y = self->y + 32 * i;
                bound.w = 32;
                bound.h = 32;
                self->bounds[n_bounds] = bound;
                ++n_bounds;
            }
            bit <<= j < 3 ? 1 : 0;
        }
        bit >>= 7;
    }
}

/**
 * Sets the x position of the Tetrimino.
 */
void tetrimino_set_x_pos(Tetrimino *self, const int x)
{
    self->x = x;
    set_bounds(self);
}

/**
 * Sets the y position of the Tetrimino.
 */
void tetrimino_set_y_pos(Tetrimino *self, const int y)
{
    self->y = y;
    set_bounds(self);
}

/**
 * Tests if the Tetrimino is colliding with an object.
 */
int tetrimino_check_collision(const Tetrimino *self, const SDL_Rect object)
{
    const int object_left = object.x;
    const int object_right = object.x + object.w;
    const int object_top = object.y;
    const int object_bottom = object.y + object.h;

    for (int i = 0; i < 4; ++i) {
        const int self_left = self->bounds[i].x;
        const int self_right = self->bounds[i].x + self->bounds[i].w;
        const int self_top = self->bounds[i].y;
        const int self_bottom = self->bounds[i].y + self->bounds[i].h;

        if (self_left >= object_right || self_right <= object_left || self_right <= object_left || self_bottom <= object_top) {
            continue;
        }

        return 1;
    }

    return 0;
}

/**
 * Renders the Tetrimino to the screen.
 */
void tetrimino_render(const Tetrimino *self, const Texture *blocks)
{
    uint16_t mino_bits = tetrimino_rotations[self->block_type][self->current_rotation];
    uint16_t bit = 0x1000;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (mino_bits & bit) {
                texture_render(blocks, self->block_type, self->x + 32 * j, self->y + 32 * i);
            }
            bit <<= j < 3 ? 1 : 0;
        }
        bit >>= 7;
    }
}
