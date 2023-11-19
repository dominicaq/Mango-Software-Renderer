#include "camera.h"

Mat4 perspective(const Camera *cam) {
    Real f = real_div(
        real_from_float(1.0f),
        real_tan(real_mul(real_deg_to_rad(cam->fov), real_from_float(0.5f))));
    Real range_inv =
        real_div(real_from_float(1.0f), real_sub(cam->z_near, cam->z_far));
    Real a = real_mul(real_add(cam->z_far, cam->z_near), range_inv);
    Real b = real_mul(
        real_mul(real_mul(real_from_float(2.0f), cam->z_far), cam->z_near),
        range_inv);
    Mat4 perspective_matrix = {{{real_div(f, cam->aspect), 0.0f, 0.0f, 0.0f},
                                {0.0f, f, 0.0f, 0.0f},
                                {0.0f, 0.0f, a, b},
                                {0.0f, 0.0f, -1.0f, 0.0f}}};
    return perspective_matrix;
}

void camera_update_matrix(Camera *cam) {
    if (cam->dirty_local) {
        game_object_update_local_matrix(&cam->game_object);
        cam->game_object.world_matrix = cam->game_object.local_matrix;
        cam->dirty_local = false;
    }
}
