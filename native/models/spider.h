#ifndef SPIDER_H
#define SPIDER_H
#include <Mango/math/vec3.h>
#include <Mango/math/vec2.h>
#include <Mango/game/gameobject.h>
extern int spider_object_amt;
extern int spider_max_depth;
extern AnimStack Attack;
extern AnimStack die;
extern AnimStack die2;
extern AnimStack fall;
extern AnimStack Jump;
extern AnimStack normal;
extern AnimStack runaniback;
extern AnimStack runanivor;
extern AnimStack runleft;
extern AnimStack runright;
extern AnimStack walkaniback;
extern AnimStack walkanivor;
extern AnimStack walkleft;
extern AnimStack walkright;
extern AnimStack wartepose;
extern GameObject spider_game_objects[93];
extern GameObjectAttr spider_attrs[93];
#endif
