#ifndef TETRIS_H
#define TETRIS_H

#ifndef TETRIS_INTERNAL
typedef void Tetris;
#endif /* TETRIS_INTERNAL */

Tetris *tetris_create(void);
void tetris_initialize(Tetris *self);
void tetris_destroy(Tetris *self);

void tetris_render(const Tetris *self, const Texture *blocks);

#endif /* TETRIS_H */
