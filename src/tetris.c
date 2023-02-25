#include <stdlib.h>
#include <time.h>

#ifndef TETRIS_INTERNAL
#define TETRIS_INTERNAL
typedef struct tetris {
    int score;
    int level;
    int lines;

    int random_bag[7];
    int bag_index;
} Tetris;
#endif /* TETRIS_INTERNAL */

/**
 * Creates a new Tetris object.
 */
Tetris *tetris_create(void)
{
    Tetris *self = malloc(sizeof(Tetris));
    self->score = 0;
    self->level = 1;
    self->lines = 0;
    srand(time(NULL));
    self->bag_index = 0;
    return self;
}

/**
 * Destroys the Tetris object.
 */
void tetris_destroy(Tetris *self)
{
    free(self);
}
