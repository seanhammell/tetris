#ifndef TETRIMINO_H
#define TETRIMINO_H

#ifndef TETRIMINO_INTERNAL
typedef void Tetrimino;
#endif /* TETRIMINO_INTERNAL */

Tetrimino *tetrimino_create(void);
void tetrimino_destroy(Tetrimino *self);

void tetrimino_set_block_type(Tetrimino *self, int block_type);

#endif /* TETRIMINO_H */
