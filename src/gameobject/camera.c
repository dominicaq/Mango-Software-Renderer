#include "camera.h"

Mat4 perspective(const Camera *cam) {
    float f = 1.0f / tanf((cam->fov * DEG2RAD) * 0.5f);
    float range_inv = 1.0f / (cam->zNear - cam->zFar);
    float a = (cam->zFar + cam->zNear) * range_inv;
    float b = (2.0f * cam->zFar * cam->zNear) * range_inv;
    Mat4 perspective_matrix = {{{f / cam->aspect, 0.0f, 0.0f, 0.0f},
                                {0.0f, f, 0.0f, 0.0f},
                                {0.0f, 0.0f, a, b},
                                {0.0f, 0.0f, -1.0f, 0.0f}}};
    return perspective_matrix;
}

void update_view_frustum(Camera *cam) {
    // 6 planes (left, right, bottom, top, near, far).
    Plane frustum[6];

    // Extract camera properties.
    Vec3 cam_position = cam->transform.position;
    Vec3 cam_forward = cam->transform.forward;
    Vec3 cam_right = cam->transform.right;
    Vec3 cam_up = cam->transform.up;
    float near_clip = cam->zNear;
    float far_clip = cam->zFar;
    float fov = cam->fov;

    // Calculate half of the near plane height and width.
    float half_near_height = tan(fov * DEG2RAD * 0.5f) * near_clip;
    float half_near_width = half_near_height * cam->aspect;

    // Calculate the near and far plane centers.
    Vec3 near_center = vec3_add(cam_position,
        vec3_scale(near_clip, cam_forward));
    Vec3 far_center = vec3_add(cam_position,
        vec3_scale(far_clip, cam_forward));

    // Calculate the normals for the near and far planes.
    Vec3 near_normal = cam_forward;
    Vec3 far_normal = vec3_negate(cam_forward);

    // Calculate the normals and points on the planes
    Vec3 right_normal = vec3_normalize(vec3_cross(cam_up, cam_forward));
    Vec3 left_normal = vec3_negate(right_normal);

    Vec3 top_normal = vec3_normalize(vec3_cross(cam_right, cam_forward));
    Vec3 bottom_normal = vec3_negate(top_normal);

    Vec3 left_point = vec3_add(near_center,
        vec3_scale(-half_near_width, right_normal));
    Vec3 right_point = vec3_add(near_center,
        vec3_scale(half_near_width, right_normal));
    Vec3 bottom_point = vec3_add(near_center,
        vec3_scale(-half_near_height, top_normal));
    Vec3 top_point = vec3_add(near_center,
        vec3_scale(half_near_height, top_normal));

    // Define the planes based on normals and points.
    frustum[0].normal = left_normal;
    frustum[0].position = left_point;

    frustum[1].normal = right_normal;
    frustum[1].position = right_point;

    frustum[2].normal = bottom_normal;
    frustum[2].position = bottom_point;

    frustum[3].normal = top_normal;
    frustum[3].position = top_point;

    frustum[4].normal = near_normal;
    frustum[4].position = near_center;

    frustum[5].normal = far_normal;
    frustum[5].position = far_center;

    for (int i = 0; i < 6; i++) {
        cam->view_frustum[i] = frustum[i];
    }
}