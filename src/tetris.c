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
    self->bag_index = 7;
    return self;
}

/**
 * Destroys the Tetris object.
 */
void tetris_destroy(Tetris *self)
{
    free(self);
}

/**
 * Returns the next block type in the random bag, generating the next bag if
 * the current bag is empty.
 */
int tetris_pull_from_random_bag(Tetris *self)
{
    if (self->bag_index > 6) {
        for (int i = 0; i < 7; ++i) {
            self->random_bag[i] = 0;
        }

        for (int block = 1; block < 8; ++block) {
            int i;
            do {
                i = rand() % 7;
            } while (self->random_bag[i] != 0);
            self->random_bag[i] = block;
        }

        self->bag_index = 0;
    }

    int block = self->random_bag[self->bag_index];
    ++self->bag_index;
    return block;
}
