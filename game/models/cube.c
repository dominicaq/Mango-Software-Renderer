#include "cube.h"
const int cube_object_amt = 2;
const int cube_max_depth = 1;
BoneWeight Cube_weights[8][8] = {{}, {}, {}, {}, {}, {}, {}, {}, };
Vec3 Cube_verts[]={{{1, 1, 1}}, {{1, 1, -1}}, {{1, -1, 1}}, {{1, -1, -1}}, {{-1, 1, 1}}, {{-1, 1, -1}}, {{-1, -1, 1}}, {{-1, -1, -1}}, };
int Cube_vert_inds[]={4, 2, 0, 2, 7, 3, 6, 5, 7, 1, 7, 5, 0, 3, 1, 4, 1, 5, 4, 6, 2, 2, 6, 7, 6, 4, 5, 1, 3, 7, 0, 2, 3, 4, 0, 1, };
Vec3 Cube_norms[]={{{0, 0, 1}}, {{0, 0, 1}}, {{0, 0, 1}}, {{0, -1, 0}}, {{0, -1, 0}}, {{0, -1, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, {{0, 0, -1}}, {{0, 0, -1}}, {{0, 0, -1}}, {{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, {{0, 0, 1}}, {{0, 0, 1}}, {{0, 0, 1}}, {{0, -1, 0}}, {{0, -1, 0}}, {{0, -1, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, {{0, 0, -1}}, {{0, 0, -1}}, {{0, 0, -1}}, {{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, };
int Cube_norm_inds[]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, };
Vec2 Cube_uvs[]={{{0.375, 0}}, {{0.625, 0}}, {{0.375, 0.25}}, {{0.625, 0.25}}, {{0.125, 0.5}}, {{0.375, 0.5}}, {{0.625, 0.5}}, {{0.875, 0.5}}, {{0.125, 0.75}}, {{0.375, 0.75}}, {{0.625, 0.75}}, {{0.875, 0.75}}, {{0.375, 1}}, {{0.625, 1}}, };
int Cube_uv_inds[]={7, 10, 6, 10, 12, 9, 1, 2, 0, 5, 8, 4, 6, 9, 5, 3, 5, 2, 7, 11, 10, 10, 13, 12, 1, 3, 2, 5, 9, 8, 6, 10, 9, 3, 6, 5, };
GameObject cube_game_objects[2] = {
{.position = {{0, 0, 0}}, .quaternion = {{0, 0, 0, 1}}, .scale = {{1, 1, 1}}, .num_children = 1}, 
{.position = {{0, 0, 0}}, .quaternion = {{-0.707107, 0, 0, 0.707107}}, .scale = {{100, 100, 100}}, .num_children = 0}, 
};
GameObjectAttr cube_attrs[2] = {{ATTR_NONE}, 
{ATTR_MESH, .mesh = {  .ind_count = 36, .vert_count = 8, .norm_count = 36, .uv_count = 14, .weights = Cube_weights, .verts = Cube_verts, .norms = Cube_norms, .uvs = Cube_uvs, .vert_inds = Cube_vert_inds, .norm_inds = Cube_norm_inds, .uv_inds = Cube_uv_inds, .color = {{1, 1, 1}}}}, 
};