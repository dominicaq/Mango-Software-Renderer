#ifndef CAMERA_h
#define CAMERA_h

#include "../math/mat4.h"
#include "../math/vec3.h"
#include "gameobject.h"

typedef struct {
    GameObject game_object;
    bool dirty_local;
    Real fov;
    Real z_near, z_far;
    Real aspect;
    int width, height;
} Camera;

void camera_update_matrix(Camera *cam);

/*
 * perspective - Calculate a perspective projection matrix
 * @cam: The Camera containing the field of view, aspect ratio, and near/far
 * planes
 *
 * Calculate a 4x4 perspective projection matrix based on the Camera's
 * parameters.
 *
 * Source:
 * https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
 *
 * Return: The perspective projection matrix.
 */
Mat4 perspective(const Camera *cam);

#endif
