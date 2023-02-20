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
    int rotations[4][16];
    int current_rotation;
} Tetrimino;
#endif /* TETRIMINO_INTERNAL */

/**
 * Creates a new Tetrimino object.
 */
Tetrimino *tetrimino_create(void)
{
    Tetrimino *self = malloc(sizeof(Tetrimino));
    self->block_type = EMPTY;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            self->rotations[i][j] = 0;
        }
    }
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
