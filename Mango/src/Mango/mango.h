#ifndef MANGO_H
#define MANGO_H

#include "game/camera.h"
#include "game/scene.h"
#include "math/real.h"

void mango_on_update(void (*)(MangoReal dt));
void mango_run(Scene *scene, const char *title, int width, int height);

#endif
