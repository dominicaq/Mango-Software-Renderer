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

Mat4x4 view(const Camera *cam) {
    Mat4x4 rot_matrix = rotate(cam->transform.euler_angles);
    Mat4x4 pos_matrix = translate(IDENTITY, cam->transform.position);

    return mat_mul(pos_matrix, rot_matrix);
}

Mat4x4 lookAt(const vec3 eye, const vec3 center, const vec3 up) {
    // NOTE: crashes, dont use
    vec3 f = normalize(vec3_sub(center, eye));
    vec3 u = normalize(up);
    vec3 s = normalize(cross(f, u));
    u = cross(s, f);

    float tx = dot(s, eye);
    float ty = dot(u, eye);
    float tz = dot(f, eye);

    Mat4x4 m = {
        {{s.x, u.x, -f.x, -tx},
         {s.y, u.y, -f.y, -ty},
         {s.z, u.z, -f.z,  tz},
         {0.0f, 0.0f, 0.0f, 1.0f}}
    };

    return m;
}