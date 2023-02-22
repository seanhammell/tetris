#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "src/texture.h"

#ifndef TETRIMINO_INTERNAL
typedef void Tetrimino;
#endif /* TETRIMINO_INTERNAL */

Tetrimino *tetrimino_create(void);
void tetrimino_initialize(Tetrimino *self, int block_type);
void tetrimino_destroy(Tetrimino *self);

void tetrimino_rotate(Tetrimino *self);

void tetrimino_render(Tetrimino *self, Texture *blocks);

#endif /* TETRIMINO_H */
