#include "scene.h"

const Vec3 COLLOR_PALLETE[7] = {
    {{1.0f, 0.0f, 0.0f}},  // Red
    {{0.0f, 1.0f, 0.0f}},  // Green
    {{0.0f, 0.0f, 1.0f}},  // Blue
    {{1.0f, 1.0f, 0.0f}},  // Yellow
    {{0.0f, 0.0f, 0.0f}},  // White
    {{0.3f, 0.0f, 0.5f}},  // Indigo
    {{0.5f, 0.0f, 0.5f}}   // Violet
};

void scene_update(Scene *scene, UBO *ubo, float delta_time) {
    float circle_radius = 4.0f;
    float angle_increment = 2.0f * M_PI / ubo->num_lights;
    for (int i = 0; i < ubo->num_lights; ++i) {
        float angle = angle_increment * i + delta_time;
        float x = circle_radius * cosf(angle);
        float z = circle_radius * sinf(angle);
        ubo->lights[i].position = (Vec3){{x, 0.0f, z}};
    }

    ubo->u_cam_pos = scene->camera.transform.position;
}
