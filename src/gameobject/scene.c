#include "scene.h"

const vec3 COLLOR_PALLETE[7] = {
    {1.0f, 0.0f, 0.0f},  // Red
    {0.0f, 1.0f, 0.0f},  // Green
    {0.0f, 0.0f, 1.0f},  // Blue
    {1.0f, 1.0f, 0.0f},  // Yellow
    {0.0f, 0.0f, 0.0f},  // White
    {0.3f, 0.0f, 0.5f},  // Indigo
    {0.5f, 0.0f, 0.5f}   // Violet
};

int init_scene(Scene *scene, UBO *ubo, int frame_width, int frame_height) {
    // Camera properties
    Camera camera;
    camera.transform.position = (vec3){0.0f, -5.0f, -5.0f};
    camera.transform.euler_angles = (vec3){25.0f, 0.0f, 0.0f};
    camera.fov = 90.0f;
    camera.aspect = (float)(frame_width) / frame_height;
    camera.zNear = 0.1f;
    camera.zFar = 1000.0f;
    scene->camera = camera;

    // Color pallete
    vec3 white = (vec3){1.0f,1.0f,1.0f};
    vec3 red = (vec3){0.0f,1.0f,0.5f};
    vec3 ambience = (vec3){0.4f,0.4f,0.4f};
    ubo->u_ambient = ambience;

    // Objects
    GameObject head_object;
    head_object.transform.position = (vec3){6.0f, -3.0f, -8.0f};
    head_object.transform.euler_angles = (vec3){0.0f, -20.0f, 0.0f};
    head_object.transform.scale = (vec3){5.0f, 5.0f, 5.0f};
    Mesh *head_mesh = load_obj_mesh("../models/head.obj");
    if (head_mesh == NULL) {
        printf("ERROR: Failed to load head mesh\n");
        return -1;
    }
    head_mesh->color = white;
    head_object.mesh = head_mesh;
    add_object_to_scene(scene, head_object);

    GameObject box_object;
    box_object.transform.position = (vec3){0.0f, 6.0f, -10.0f};
    box_object.transform.euler_angles = (vec3){180.0f, 90.0f, 90.0f};
    box_object.transform.scale = (vec3){1.0f, 1.0f, 1.0f};
    Mesh *box_model = load_obj_mesh("../models/light_box.obj");
    if (box_model == NULL) {
        printf("ERROR: Failed to loead plane mesh\n");
        return -1;
    }
    box_model->color = red;
    box_object.mesh = box_model;
    add_object_to_scene(scene, box_object);


    for (int i = 0; i <   box_object.mesh->vert_count; ++i) {
        print_vec3(box_object.mesh->vertices[i]);
    }
    for (int i = 0; i <   box_object.mesh->index_count; ++i) {
        printf("%d\n", box_object.mesh->vertex_index[i]);
    }
    GameObject diablo_object;
    diablo_object.transform.position = (vec3){-5.0f, -3.0f, -10.0f};
    diablo_object.transform.euler_angles = (vec3){0.0f, 70.0f, 0.0f};
    diablo_object.transform.scale = (vec3){6.0f, 6.0f, 6.0f};
    Mesh *diablo_model = load_obj_mesh("../models/head.obj");
    if (diablo_model == NULL) {
        printf("ERROR: Failed to load diablo mesh\n");
        return -1;
    }
    diablo_model->color = white;
    diablo_object.mesh = diablo_model;
    add_object_to_scene(scene, diablo_object);

    // Init light data
    float light_radius = 20.0f;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        ubo->lights[i].u_color = vec3_to_vec4(COLLOR_PALLETE[i], 1.0f);
        ubo->lights[i].u_radius = light_radius;
    }

    return 0;
}

void scene_update(Scene *scene, UBO *ubo, float delta_time) {
    float circle_radius = 4.0f;
    float angle_increment = 2.0f * M_PI / MAX_LIGHTS;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        float angle = angle_increment * i + delta_time;
        float x = circle_radius * cosf(angle);
        float z = circle_radius * sinf(angle);
        ubo->lights[i].u_position = (vec3){x, 0.0f, z};
    }

    ubo->u_cam_pos = scene->camera.transform.position;
}

void add_object_to_scene(Scene *scene, GameObject object) {
    scene->game_objects[scene->num_objects] = object;
    ++(scene->num_objects);
}