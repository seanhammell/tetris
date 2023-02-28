#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "src/state.h"
#include "src/texture.h"
#include "src/tetrimino.h"

#ifndef TETRIS_INTERNAL
#define TETRIS_INTERNAL
typedef struct tetris {
    int score;
    int level;
    int lines;
    int matrix[20][10];
    SDL_Rect matrix_bounds[3];

    Tetrimino *current;
    Tetrimino *next;
    int random_bag[7];
    int bag_index;
} Tetris;
#endif /* TETRIS_INTERNAL */

#include "src/tetris.h"

enum bounds {
    GROUND,
    LEFT_WALL,
    RIGHT_WALL,
};

static int frames_per_step[30] = {
    48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
};

/**
 * Creates a new Tetris object.
 */
Tetris *tetris_create(void)
{
    Tetris *self = malloc(sizeof(Tetris));
    self->score = 0;
    self->level = 0;
    self->lines = 0;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            self->matrix[i][j] = 0;
        }
    }
    self->matrix_bounds[GROUND].x = 64;
    self->matrix_bounds[GROUND].y = 576;
    self->matrix_bounds[GROUND].w = 320;
    self->matrix_bounds[GROUND].h = 0;
    self->current = tetrimino_create();
    self->next = tetrimino_create();
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
 * Pulls the next block type from the random bag.
 */
void pull_from_random_bag(Tetris *self)
{
    tetrimino_set_block_type(self->current, tetrimino_get_block_type(self->next));
    if (self->bag_index > 6) {
        generate_random_bag(self);
        self->bag_index = 0;
    }
    tetrimino_set_block_type(self->next, self->random_bag[self->bag_index]);
    ++self->bag_index;
}

/**
 * Initializes the Tetris object with a random bag of blocks.
 */
void tetris_initialize(Tetris *self)
{
    srand(time(NULL));
    generate_random_bag(self);

    tetrimino_set_block_type(self->next, self->random_bag[0]);
    self->bag_index = 1;
    pull_from_random_bag(self);

    tetrimino_set_x_pos(self->current, 160);
    tetrimino_set_x_pos(self->next, 480);
    tetrimino_set_y_pos(self->next, 416);
}

/**
 * Destroys a Tetris object.
 */
void tetris_destroy(Tetris *self)
{
    tetrimino_destroy(self->current);
    tetrimino_destroy(self->next);
    free(self);
    self = NULL;
}

/**
 * Tests whether the current Tetrimino is colliding with blocks in the matrix.
 */
int current_tetrimino_matrix_collision(Tetris *self)
{
    uint16_t mino_bits = tetrimino_get_rotation(self->current);
    uint16_t bit = 0x1000;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (mino_bits & bit) {
                int row = i + tetrimino_get_y_pos(self->current) / 32;
                int col = j + (tetrimino_get_x_pos(self->current) - 64) / 32;
                if (self->matrix[row][col] != 0) {
                    return 1;
                }
            }
            bit <<= j < 3 ? 1 : 0;
        }
        bit >>= 7;
    }

    return 0;
}

/**
 * Adds the current Tetrimino to the matrix.
 */
void add_current_tetrimino_to_matrix(Tetris *self)
{
    uint16_t mino_bits = tetrimino_get_rotation(self->current);
    uint16_t bit = 0x1000;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (mino_bits & bit) {
                int row = i + tetrimino_get_y_pos(self->current) / 32;
                int col = j + (tetrimino_get_x_pos(self->current) - 64) / 32;
                self->matrix[row][col] = tetrimino_get_block_type(self->current);
            }
            bit <<= j < 3 ? 1 : 0;
        }
        bit >>= 7;
    }
}

/**
 * Apply gravity to the current Tetrimino based on the drop rate of the
 * current level.
 */
void tetris_apply_gravity(Tetris *self)
{
    if (state.frames_since_step > frames_per_step[self->level]) {
        tetrimino_set_y_pos(self->current, tetrimino_get_y_pos(self->current) + 32);
        state.frames_since_step = 0;

        if (tetrimino_check_collision(self->current, self->matrix_bounds[GROUND]) || current_tetrimino_matrix_collision(self)) {
            tetrimino_set_y_pos(self->current, tetrimino_get_y_pos(self->current) - 32);
            add_current_tetrimino_to_matrix(self);
            pull_from_random_bag(self);
            tetrimino_set_x_pos(self->current, 160);
            tetrimino_set_y_pos(self->current, 0);
        }
    }
}

/**
 * Renders the current Tetrimino, next Tetrimino, and playfield matrix to the
 * screen.
 */
void tetris_render(const Tetris *self, const Texture *blocks)
{
    tetrimino_render(self->current, blocks);
    tetrimino_render(self->next, blocks);

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (self->matrix[i][j] == 0) {
                continue;
            }
            texture_render(blocks, self->matrix[i][j], 64 + 32 * j, 32 * i);
        }
    }
}
