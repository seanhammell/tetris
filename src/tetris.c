#include <stdlib.h>

#include "src/texture.h"

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

/**
 * Renders the matrix to the screen.
 * 
 * The matrix represents the playfield of 20 rows and 10 columns. The layout is
 * as follows:
 * 
 *        0   1  2    3   4   5   6   7   8   9
 *       10  11  12  13  14  15  16  17  18  19
 *      ...
 *      190 191 192 193 194 195 196 197 198 199
 * 
 * This layout aligns with the coordinate system used in SDL where (0, 0) is
 * the top-left corner.
 */
void tetris_render_matrix(Tetris *self, Texture *blocks)
{
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            texture_render(blocks, self->matrix[10 * i + j], 64 + 32 * j, 32 * i);
        }
    }
}
