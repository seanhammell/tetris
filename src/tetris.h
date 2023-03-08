#ifndef TETRIS_H
#define TETRIS_H

#include <SDL2/SDL.h>

#ifndef TETRIS_INTERNAL
typedef void Tetris;
#endif /* TETRIS_INTERNAL */

Tetris *tetris_create(void);
void tetris_initialize(Tetris *self);
void tetris_destroy(Tetris *self);

int tetris_apply_gravity(Tetris *self, int *cleared_lines);
void tetris_clear_lines(Tetris *self, int *cleared_lines);
void tetris_next_tetrimino(Tetris *self);
void tetris_handle_event(Tetris *self, const SDL_Event event);

void tetris_render(const Tetris *self, const Texture *blocks, const int are_status);

#endif /* TETRIS_H */
