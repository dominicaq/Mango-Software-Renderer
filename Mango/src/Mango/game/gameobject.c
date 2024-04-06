#include "gameobject.h"

const float DEG2RAD = M_PI / 180.0f;

Mat4 rotate(Vec3 angles) {
    // Z = Roll
    // X = Pitch
    // Y = Yaw
    float x = angles.x * DEG2RAD;
    float y = angles.y * DEG2RAD;
    float z = angles.z * DEG2RAD;

    float a = cosf(x);
    float b = sinf(x);

    float c = cosf(y);
    float d = sinf(y);

    float e = cosf(z);
    float f = sinf(z);

    float ae = a * e, af = a * f, be = b * e, bf = b * f;

    float m1 = c * e;
    float m2 = -c * f;
    float m3 = d;

    float m4 = af + be * d;
    float m5 = ae - bf * d;
    float m6 = -b * c;

    float m7 = bf - ae * d;
    float m8 = be + af * d;
    float m9 = a * c;

    Mat4 result = {
        {{m1, m2, m3, 0.0f},
         {m4, m5, m6, 0.0f},
         {m7, m8, m9, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}}  // Last row is homogeneous
    };

    return result;
}

void game_object_update_local_matrix(GameObject *go) {
    // go = game object
    float x = go->quaternion.x, y = go->quaternion.y, z = go->quaternion.z,
         w = go->quaternion.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    float sx = go->scale.x, sy = go->scale.y, sz = go->scale.z;

    go->local_matrix.elem[0][0] = (1 - (yy + zz)) * sx;
    go->local_matrix.elem[1][0] = (xy + wz) * sx;
    go->local_matrix.elem[2][0] = (xz - wy) * sx;
    go->local_matrix.elem[3][0] = 0;

    go->local_matrix.elem[0][1] = (xy - wz) * sy;
    go->local_matrix.elem[1][1] = (1 - (xx + zz)) * sy;
    go->local_matrix.elem[2][1] = (yz + wx) * sy;
    go->local_matrix.elem[3][1] = 0;

    go->local_matrix.elem[0][2] = (xz + wy) * sz;
    go->local_matrix.elem[1][2] = (yz - wx) * sz;
    go->local_matrix.elem[2][2] = (1 - (xx + yy)) * sz;
    go->local_matrix.elem[3][2] = 0;

    go->local_matrix.elem[0][3] = go->position.x;
    go->local_matrix.elem[1][3] = go->position.y;
    go->local_matrix.elem[2][3] = go->position.z;
    go->local_matrix.elem[3][3] = 1;

    // Extract the forward direction (third column)
    go->forward.x = go->local_matrix.elem[0][2];
    go->forward.y = go->local_matrix.elem[1][2];
    go->forward.z = go->local_matrix.elem[2][2];

    // Extract the right direction (first column)
    go->right.x = go->local_matrix.elem[0][0];
    go->right.y = go->local_matrix.elem[1][0];
    go->right.z = go->local_matrix.elem[2][0];

    // Extract the up direction (second column)
    go->up.x = go->local_matrix.elem[0][1];
    go->up.y = go->local_matrix.elem[1][1];
    go->up.z = go->local_matrix.elem[2][1];
}

GameObject game_object_default() {
    GameObject go = {
        .position = {{0.0f, 0.0f, 0.0f}},
        .quaternion = {{0.0f, 0.0f, 0.0f, 1.0f}},
        .scale = {{1.0f, 1.0f, 1.0f}},
        .num_children = 0,
    };
    return go;
}

Mesh mesh_empty() {
    Mesh mesh = {.ind_count = 0,
                 .vert_count = 0,
                 .norm_count = 0,
                 .uv_count = 0,
                 .color = {{1.0f, 1.0f, 1.0f}}};
    return mesh;
}
