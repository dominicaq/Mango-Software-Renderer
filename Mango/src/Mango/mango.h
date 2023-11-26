#ifndef MANGO_H
#define MANGO_H

#include "game/animation.h"
#include "game/scene.h"
#include "render/framedata.h"

LIST_TYPE(AnimList, Anim);
typedef struct {
    Frame *frame;
    Scene *scene;
    AnimList running_anims;
    void (*user_update)(Real dt);
    UBO ubo;
} Mango;

Mango *mango_alloc(Scene *scene, const char *title, int width, int height);
void mango_run(Mango *);
void mango_play_anim(Mango *, int object_index, AnimStack *);
void mango_free(Mango *);

#endif
