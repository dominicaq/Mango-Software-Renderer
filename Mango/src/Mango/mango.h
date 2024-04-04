#ifndef MANGO_H
#define MANGO_H

#include "game/scene.h"
#include "render/framedata.h"

typedef struct {
    Frame *frame;
    Scene *scene;
    void (*user_update)(float dt);
    UBO ubo;
} Mango;

Mango *mango_alloc(Scene *scene, const char *title, int width, int height);
void mango_run(Mango *);
void mango_free(Mango *);

#endif
