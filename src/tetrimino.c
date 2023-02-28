#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "src/texture.h"

#ifndef TETRIMINO_INTERNAL
#define TETRIMINO_INTERNAL
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

typedef struct tetrimino {
    enum blocks block_type;
    int current_rotation;
    int x;
    int y;
} Tetrimino;
#endif /* TETRIMINO_INTERNAL */

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

void tetrimino_set_position(Tetrimino *self, const int x, const int y)
{
    self->x = x;
    self->y = y;
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
