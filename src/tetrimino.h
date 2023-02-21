#ifndef TETRIMINO_H
#define TETRIMINO_H

#ifndef TETRIMINO_INTERNAL
typedef void Tetrimino;
#endif /* TETRIMINO_INTERNAL */

Tetrimino *tetrimino_create(void);
void tetrimino_initialize(Tetrimino *self, int block_type);
void tetrimino_destroy(Tetrimino *self);

#endif /* TETRIMINO_H */
