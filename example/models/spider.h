#ifndef SPIDER_H
#define SPIDER_H
#include <Mango/math/vec3.h>
#include <Mango/math/vec2.h>
#include <Mango/game/animation.h>
extern int spider_object_amt;
extern int spider_max_depth;
extern AnimStack spider_Attack_anim;
extern AnimStack spider_die_anim;
extern AnimStack spider_die2_anim;
extern AnimStack spider_fall_anim;
extern AnimStack spider_Jump_anim;
extern AnimStack spider_normal_anim;
extern AnimStack spider_runaniback_anim;
extern AnimStack spider_runanivor_anim;
extern AnimStack spider_runleft_anim;
extern AnimStack spider_runright_anim;
extern AnimStack spider_walkaniback_anim;
extern AnimStack spider_walkanivor_anim;
extern AnimStack spider_walkleft_anim;
extern AnimStack spider_walkright_anim;
extern AnimStack spider_wartepose_anim;
extern GameObject spider_game_objects[93];
extern GameObjectAttr spider_attrs[93];
#endif
