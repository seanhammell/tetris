#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef TEXTURE_INTERNAL
typedef void Texture;
#endif /* TEXTURE_INTERNAL */

Texture *texture_create(void);
int texture_initialize(Texture *self, const char *path);
void texture_set_clips(Texture *self, const int n_rows, const int n_cols, const int width, const int height);
void texture_destroy(Texture *self);

void texture_render(const Texture *self, const int clip, const int x, const int y);

#endif /* TEXTURE_H */
