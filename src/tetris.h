#ifndef TETRIS_H
#define TETRIS_H

#ifndef TETRIS_INTERNAL
typedef void Tetris;
#endif /* TETRIS_INTERNAL */

Tetris *tetris_create(void);
void tetris_destroy(Tetris *self);

int tetris_pull_from_random_bag(Tetris *self);

#endif /* TETRIS_H */
