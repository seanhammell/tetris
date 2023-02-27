#include <stdlib.h>

#ifndef TETRIS_INTERNAL
#define TETRIS_INTERNAL
typedef struct tetris {
    int score;
    int level;
    int lines;
    int matrix[200];
} Tetris;
#endif /* TETRIS_INTERNAL */

#include "src/tetris.h"

/**
 * Creates a new Tetris object.
 */
Tetris *tetris_create(void)
{
    Tetris *self = malloc(sizeof(Tetris));
    self->score = 0;
    self->level = 1;
    self->lines = 0;
    for (int i = 0; i < 200; ++i) {
        self->matrix[i] = 0;
    }
    return self;
}

/**
 * Destroys a Tetris object.
 */
void tetris_destroy(Tetris *self)
{
    free(self);
    self = NULL;
}
