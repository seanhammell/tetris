#ifndef TETRIS_H
#define TETRIS_H

#ifndef TETRIS_INTERNAL
typedef void Tetris;
#endif /* TETRIS_INTERNAL */

Tetris *tetris_create(void);
void tetris_initialize(Tetris *self);
void tetris_destroy(Tetris *self);

void tetris_pull_from_random_bag(Tetris *self);
int tetris_get_current_block(Tetris *self);
int tetris_get_next_block(Tetris *self);

#endif /* TETRIS_H */
