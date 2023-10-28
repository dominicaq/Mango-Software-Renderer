#include "camera.h"

Mat4x4 perspective(const Camera *cam) {
  float f = 1.0f / tanf((cam->fov * DEG2RAD) * 0.5f);
  float range_inv = 1.0f / (cam->zNear - cam->zFar);
    float a = (cam->zFar + cam->zNear) * range_inv;
    float b = (2.0f * cam->zFar * cam->zNear) * range_inv;
    Mat4x4 perspective_matrix = {
        {{f / cam->aspect, 0.0f, 0.0f, 0.0f},
        {0.0f, f, 0.0f, 0.0f},
        {0.0f, 0.0f, a, b},
        {0.0f, 0.0f, -1.0f, 0.0f}}
    };
    return perspective_matrix;
}

