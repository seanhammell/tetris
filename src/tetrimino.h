#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <stdint.h>

#ifndef TETRIMINO_INTERNAL
typedef void Tetrimino;
#endif /* TETRIMINO_INTERNAL */

Tetrimino *tetrimino_create(void);
void tetrimino_destroy(Tetrimino *self);

int tetrimino_get_block_type(const Tetrimino *self);
uint16_t tetrimino_get_rotation(const Tetrimino *self);
int tetrimino_get_x_pos(const Tetrimino *self);
int tetrimino_get_y_pos(const Tetrimino *self);

void tetrimino_set_block_type(Tetrimino *self, const int block_type);
void tetrimino_set_position(Tetrimino *self, const int x, const int y);

#endif /* TETRIMINO_H */
