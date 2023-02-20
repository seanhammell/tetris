#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef TEXTURE_INTERNAL
typedef void Texture;
#endif /* TEXTURE_INTERNAL */

Texture *texture_create(void);
int texture_initialize(Texture *self, const char *path);
void texture_destroy(Texture *self);

#endif /* TEXTURE_H */
