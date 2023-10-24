#include "scene.h"

const vec3 COLLOR_PALLETE[7] = {
    {1.0f, 0.0f, 0.0f},  // Red
    {0.0f, 0.0f, 0.0f},  // White
    {1.0f, 1.0f, 0.0f},  // Yellow
    {0.0f, 1.0f, 0.0f},  // Green
    {0.0f, 0.0f, 1.0f},  // Blue
    {0.3f, 0.0f, 0.5f},  // Indigo
    {0.5f, 0.0f, 0.5f}   // Violet
};

int init_scene(Scene *scene, UBO *ubo, int frame_width, int frame_height) {
    // Camera properties
    Camera camera;
    camera.transform.position = (vec3){0.0f, -1.0f, -2.0f};
    camera.transform.euler_angles = (vec3){35.0f, 0.0f, 0.0f};
    camera.fov = 90.0f;
    camera.aspect = (float)(frame_width) / frame_height;
    camera.zNear = 0.001f;
    camera.zFar = 1000.0f;
    scene->camera = camera;

    // Color pallete
    vec3 white = (vec3){1.0f,1.0f,1.0f};

    // Objects
    GameObject head_object;
    head_object.transform.position = (vec3){6.0f, -3.0f, -8.0f};
    head_object.transform.euler_angles = (vec3){0.0f, -20.0f, 0.0f};
    head_object.transform.scale = (vec3){6.0f, 6.0f, 6.0f};
    Mesh *head_mesh = load_obj_mesh("../models/head.obj");
    if (head_mesh == NULL) {
        printf("ERROR: Failed to load head mesh\n");
        return -1;
    }
    head_mesh->color = white;
    head_object.mesh = head_mesh;
    add_object_to_scene(scene, head_object);

    GameObject plane_object;
    plane_object.transform.position = (vec3){-3.0f, -1.0f, -6.0f};
    plane_object.transform.euler_angles = (vec3){0.0f, 40.0f, 0.0f};
    plane_object.transform.scale = (vec3){0.3f, 0.3f, 0.3f};
    Mesh *plane_model = load_obj_mesh("../models/Atlas.obj");
    if (plane_model == NULL) {
        printf("ERROR: Failed to plane mesh\n");
        return -1;
    }
    plane_model->color = white;
    plane_object.mesh = plane_model;
    add_object_to_scene(scene, plane_object);

    float light_radius = 5.0f;
    float angle_increment = 2.0f * M_PI / MAX_LIGHTS;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        float angle = angle_increment * i;
        float x = light_radius * cosf(angle);
        float z = light_radius * sinf(angle);

        ubo->lights[i].u_light_position = (vec3){x, 0.0f, z};
        ubo->lights[i].u_light_color = vec3_to_vec4(COLLOR_PALLETE[i], 1.0f);
    }

    return 0;
}

void scene_update(Scene *scene, UBO *ubo, float delta_time) {
    float light_radius = 5.0f;
    float angle_increment = 2.0f * M_PI / MAX_LIGHTS;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        float angle = angle_increment * i + delta_time;
        float x = light_radius * cosf(angle);
        float z = light_radius * sinf(angle);
        ubo->lights[i].u_light_position = (vec3){x, 0.0f, z};
    }

    ubo->u_cam_pos = scene->camera.transform.position;
}

void add_object_to_scene(Scene *scene, GameObject object) {
    scene->game_objects[scene->num_objects] = object;
    ++(scene->num_objects);
}