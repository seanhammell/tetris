#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef TEXTURE_INTERNAL
typedef void Texture;
#endif /* TEXTURE_INTERNAL */

Texture *texture_create(void);
int texture_initialize(Texture *self, const char *path);
void texture_destroy(Texture *self);

void texture_render(const Texture *self, const int block_type, const int x, const int y);

#endif /* TEXTURE_H */
