#include "camera.h"

Mat4 perspective(const Camera *cam) {
    float f = 1.0f / tanf((cam->fov * DEG2RAD) * 0.5f);
    float range_inv = 1.0f / (cam->z_near - cam->z_far);
    float a = (cam->z_far + cam->z_near) * range_inv;
    float b = (2.0f * cam->z_far * cam->z_near) * range_inv;
    Mat4 perspective_matrix = {
        {{f / cam->aspect, 0.0f, 0.0f, 0.0f},
        {0.0f, f, 0.0f, 0.0f},
        {0.0f, 0.0f, a, b},
        {0.0f, 0.0f, -1.0f, 0.0f}}
    };
    return perspective_matrix;
}

Mat4 orthographic(const Camera *cam, float left, float right, float bottom, float top) {
    // Works but clip planes arent adjusted to this
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(cam->z_far + cam->z_near) / (cam->z_far - cam->z_near);

    Mat4 orthographic_matrix = {
        {{2.0f / (right - left), 0.0f, 0.0f, tx},
        {0.0f, 2.0f / (top - bottom), 0.0f, ty},
        {0.0f, 0.0f, -2.0f / (cam->z_far - cam->z_near), tz},
        {0.0f, 0.0f, 0.0f, 1.0f}}
    };

    return orthographic_matrix;
}

void camera_update_matrix(Camera *cam) {
    if (cam->dirty_local) {
        game_object_update_local_matrix(&cam->game_object);
        cam->game_object.world_matrix = cam->game_object.local_matrix;
        cam->dirty_local = false;
    }
}
