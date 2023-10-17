#ifndef CAMERA_h
#define CAMERA_h

#include "../math/vec3.h"
#include "../math/mat4x4.h"

#include "transform.h"

typedef struct {
    float fov;
    float zNear, zFar;
    float aspect;
    Transform transform;
} Camera;

// Source: https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
Mat4x4 perspective(const Camera *cam) {
    float f = 1.0f / tanf((cam->fov * DEG2RAD) * 0.5f);
    float range_inv = 1.0f / (cam->zNear - cam->zFar);
    Mat4x4 perspective_matrix = {
        {{f / cam->aspect, 0.0f, 0.0f, 0.0f},
        {0.0f, f, 0.0f, 0.0f},
        {0.0f, 0.0f, (cam->zFar + cam->zNear) * range_inv, 2.0f * cam->zFar * cam->zNear * range_inv},
        {0.0f, 0.0f, -1.0f, 0.0f}}
    };
    return perspective_matrix;
}

Mat4x4 view(const Camera *cam) {
    vec3 x_axis = {1.0f, 0.0f, 0.0f};
    vec3 y_axis = {0.0f, 1.0f, 0.0f};
    vec3 z_axis = {0.0f, 0.0f, 1.0f};

    Mat4x4 rot_matrix = rotate(cam->transform.eulerAngles.x * DEG2RAD, x_axis);
    rot_matrix = rotate(cam->transform.eulerAngles.y * DEG2RAD, y_axis);
    rot_matrix = rotate(cam->transform.eulerAngles.z * DEG2RAD, z_axis);

    Mat4x4 posMatrix = translate(IDENTITY, cam->transform.position);
    return mat_mul(rot_matrix, posMatrix);
}

Mat4x4 lookAt(vec3 eye, vec3 center, vec3 up) {
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

#endif