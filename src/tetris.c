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
    int current_block;
    int next_block;
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
    return self;
}

/**
 * Generates a new set of blocks for the random bag.
 */
void generate_random_bag(Tetris *self)
{
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
}

/**
 * Initializes the Tetris object with a random bag of blocks.
 */
void tetris_initialize(Tetris *self)
{
    srand(time(NULL));
    generate_random_bag(self);
    self->current_block = self->random_bag[0];
    self->next_block = self->random_bag[1];
    self->bag_index = 2;
}

/**
 * Destroys the Tetris object.
 */
void tetris_destroy(Tetris *self)
{
    free(self);
}

/**
 * Sets the block on deck to the current block and pulls the next block from
 * the random bag.
 */
void tetris_pull_from_random_bag(Tetris *self)
{
    self->current_block = self->next_block;

    if (self->bag_index > 6) {
        generate_random_bag(self);
        self->bag_index = 0;
    }

    self->next_block  = self->random_bag[self->bag_index];
    ++self->bag_index;
}

/**
 * Returns the current block.
 */
int tetris_get_current_block(Tetris *self)
{
    return self->current_block;
}

/**
 * Returns the next block.
 */
int tetris_get_next_block(Tetris *self)
{
    return self->next_block;
}
