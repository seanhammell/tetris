#ifndef TETRIS_H
#define TETRIS_H

#ifndef TETRIS_INTERNAL
typedef void Tetris;
#endif /* TETRIS_INTERNAL */

Tetris *tetris_create(void);
void tetris_destroy(Tetris *self);

void tetris_render_matrix(Tetris *self, Texture *blocks);

#endif /* TETRIS_H */
