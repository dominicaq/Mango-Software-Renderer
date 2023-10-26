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
    return mat_mul(rot_matrix, pos_matrix);
    // vec3 target = (vec3){0,4,0};
    // vec3 up = (vec3){0,1,0};
    // return lookAt(cam->transform.position, target, up);
}

Mat4x4 lookAt(const vec3 eye, const vec3 center, const vec3 up) {
    // NOTE: crashes, dont use
    vec3 f;
    vec3 u;
    vec3 s = vec3_sub(eye, center);
    if (dot(s, s) == 0) {
        s.z = 1;
    }
    s = normalize(s);
    f = cross(up, s);

    if (dot(f, f) == 0) {
        if (abs(up.z) == 1) {
            s.x += 0.0001;
        } else {
           s.z += 0.0001;

        }
        s = normalize(s);
        f = cross(up, s);
    }

    f = normalize(f);
    u = cross(s, f);


    Mat4x4 m = {
        {{s.x, u.x, f.x, 0.0f},
         {s.y, u.y, f.y, 0.0f},
         {s.z, u.z, f.z, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}}
    };

    return m;
}