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
    int matrix[19][12];

    Tetrimino *current;
    Tetrimino *next;
    int random_bag[7];
    int bag_index;

    int gravity_status;
    int das_status;
} Tetris;
#endif /* TETRIS_INTERNAL */

#include "src/tetris.h"

static int frames_per_step[21] = {
    53, 49, 45, 41, 37, 33, 28, 22, 17, 11, 10, 9, 8, 7, 6, 6, 5, 5, 4, 4, 3,
};

static int delayed_auto_shift[3] = {
    0, 23, 9,
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
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (i == 18 || j == 0 || j == 11) {
                self->matrix[i][j] = 1;
            } else {
                self->matrix[i][j] = 0;
            }
        }
    }

    self->current = tetrimino_create();
    self->next = tetrimino_create();

    self->gravity_status = self->level;
    self->das_status = 0;
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
                int col = j + (tetrimino_get_x_pos(self->current) - 32) / 32;
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
                int col = j + (tetrimino_get_x_pos(self->current) - 32) / 32;
                self->matrix[row][col] = tetrimino_get_block_type(self->current);
            }
            bit <<= j < 3 ? 1 : 0;
        }
        bit >>= 7;
    }
}

#include <stdio.h>
/**
 * Checks if there are any lines to clear, adding full rows to the cleared
 * lines array.
 */
int has_lines_to_clear(Tetris *self, int *cleared_lines)
{
    int line_index = 0;
    for (int i = 0; i < 18; ++i) {
        for (int j = 1; j < 11; ++j) {
            if (self->matrix[i][j] == 0) {
                break;
            }

            if (j == 10) {
                cleared_lines[line_index] = i;
                ++line_index;
            }
        }
    }

    self->lines += line_index;
    self->level = self->lines / 10;
    return line_index > 0;
}

/**
 * Removes any full rows from the matrix.
 */
void tetris_clear_lines(Tetris *self, int *cleared_lines)
{
    int line_index = 0;
    for (int i = 0; i < 4 && cleared_lines[i] != 0; ++i) {
        ++line_index;
    }

    for (int i = 0; i < line_index; ++i) {
        for (int j = cleared_lines[i] - 1; j > 0; --j) {
            for (int k = 1; k < 11; ++k) {
                self->matrix[j + 1][k] = self->matrix[j][k];
            }
        }
    }

    self->score += 10 * (self->level * 10 + line_index);
}

/**
 * Apply gravity to the current Tetrimino based on the drop rate of the
 * current level and returns the number of frames to wait based on if the
 * Tetrimino was locked with additional frames for a line clear.
 */
int tetris_apply_gravity(Tetris *self, int *cleared_lines)
{
    if (state.frames_since_step > frames_per_step[self->gravity_status]) {
        tetrimino_set_y_pos(self->current, tetrimino_get_y_pos(self->current) + 32);
        state.frames_since_step = 0;

        if (current_tetrimino_matrix_collision(self)) {
            tetrimino_set_y_pos(self->current, tetrimino_get_y_pos(self->current) - 32);
            add_current_tetrimino_to_matrix(self);
            state.are_frames = 0;
            if (has_lines_to_clear(self, cleared_lines)) {
                return 93;
            }
            return 2;
        }
    }

    return 0;
}

/**
 * Get the next Tetrimino. This is not called in the apply gravity function to
 * account for the ARE delay.
 */
void tetris_next_tetrimino(Tetris *self)
{
    pull_from_random_bag(self);
    tetrimino_set_x_pos(self->current, 160);
    tetrimino_set_y_pos(self->current, 0);
    tetrimino_reset_rotation(self->current);
}

/**
 * Controls the current Tetrimino based on input events.
 */
void tetris_handle_event(Tetris *self, const SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            tetrimino_rotate(self->current);
            if (current_tetrimino_matrix_collision(self)) {
                tetrimino_unrotate(self->current);
            }
            break;
        case SDLK_DOWN:
            self->gravity_status = 20;
            break;
        case SDLK_LEFT:
            if (state.das_frames > delayed_auto_shift[self->das_status]) {
                tetrimino_set_x_pos(self->current, tetrimino_get_x_pos(self->current) - 32);
                if (current_tetrimino_matrix_collision(self)) {
                    tetrimino_set_x_pos(self->current, tetrimino_get_x_pos(self->current) + 32);
                    break;
                }
                state.das_frames = 0;
                if (self->das_status < 2) {
                    ++self->das_status;
                }
            }
            break;
        case SDLK_RIGHT:
            if (state.das_frames > delayed_auto_shift[self->das_status]) {
                tetrimino_set_x_pos(self->current, tetrimino_get_x_pos(self->current) + 32);
                if (current_tetrimino_matrix_collision(self)) {
                    tetrimino_set_x_pos(self->current, tetrimino_get_x_pos(self->current) - 32);
                    break;
                }
                state.das_frames = 0;
                if (self->das_status < 2) {
                    ++self->das_status;
                }
            }
            break;
        }
    } else if (event.type == SDL_KEYUP) {
        self->gravity_status = self->level;
        self->das_status = 0;
    }
}

/**
 * Renders the current Tetrimino, next Tetrimino, and playfield matrix to the
 * screen.
 */
void tetris_render(const Tetris *self, const Texture *blocks, const Texture *numbers, const int are_status)
{
    if (are_status == 0) {
        tetrimino_render(self->current, blocks);
    }
    tetrimino_render(self->next, blocks);

    for (int i = 0; i < 18; ++i) {
        for (int j = 1; j < 11; ++j) {
            if (self->matrix[i][j] == 0) {
                continue;
            }
            texture_render(blocks, self->matrix[i][j], 32 + 32 * j, 32 * i);
        }
    }

    if (self->score == 0) {
        texture_render(numbers, 0, 576, 96);
        return;
    }

    int score = self->score;
    for (int i = 0; score; ++i, score /= 10) {
        texture_render(numbers, score % 10, 576 - 32 * i, 96);
    }
}
