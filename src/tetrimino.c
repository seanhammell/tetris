#include <stdint.h>
#include <stdlib.h>

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
} Tetrimino;
#endif /* TETRIMINO_INTERNAL */

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
    return self;
}

/**
 * Destroys the Tetrimino object.
 */
void tetrimino_destroy(Tetrimino *self)
{
    free(self);
}
