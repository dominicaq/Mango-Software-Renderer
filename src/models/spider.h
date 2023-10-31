#ifndef SPIDER_H
#define SPIDER_H
#include "../math/vec3.h"
#include "../math/vec2.h"
#include "../game/gameobject.h"
extern int spider_object_amt;
extern int spider_max_depth;
extern Vec3 spider_verts[1110];
extern int spider_vert_inds[6408];
extern Vec3 spider_norms[6408];
extern int spider_norm_inds[6408];
extern Vec2 spider_uvs[1221];
extern int spider_uv_inds[6408];
extern Vec3 spidereyes_verts[156];
extern int spidereyes_vert_inds[768];
extern Vec3 spidereyes_norms[768];
extern int spidereyes_norm_inds[768];
extern Vec2 spidereyes_uvs[336];
extern int spidereyes_uv_inds[768];
extern Vec3 spiderfur_verts[392];
extern int spiderfur_vert_inds[960];
extern Vec3 spiderfur_norms[960];
extern int spiderfur_norm_inds[960];
extern Vec2 spiderfur_uvs[394];
extern int spiderfur_uv_inds[960];
extern Vec3 spiderteeth_verts[102];
extern int spiderteeth_vert_inds[540];
extern Vec3 spiderteeth_norms[540];
extern int spiderteeth_norm_inds[540];
extern Vec2 spiderteeth_uvs[180];
extern int spiderteeth_uv_inds[540];
extern GameObject spider_game_objects[93];
extern GameObjectAttr spider_attrs[93];
#endif
