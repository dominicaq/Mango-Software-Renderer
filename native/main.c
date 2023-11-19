#include <Mango/mango.h>
#include <Mango/mesh/obj_parser.h>
#include <Mango/render/drawing.h>
#include <Mango/render/framedata.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "models/spider.h"

// Window data
const char *GAME_TITLE = "Mango Renderer";
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 440;

// Debugging
// -----------------------------------------------------------------------------

const bool DEBUG_ENABLE_FPS_COUNT = true;
const bool DEBUG_USE_WIREFRAME = false;
const bool DEBUG_USE_RASTERIZE = true;

void fps_counter() {
    static int frames = 0;
    static clock_t start, end;
    static double elapsed_time;

    // Print FPS every second
    end = clock();
    elapsed_time = (double) (end - start) / CLOCKS_PER_SEC;
    if (elapsed_time >= 1.0) {
        double fps = frames / elapsed_time;
        printf("FPS: %f\n", fps);

        // Reset counters for the next second
        start = clock();
        frames = 0;
    }
    ++frames;
}

// -----------------------------------------------------------------------------

// Lights
const Vec3 COLLOR_PALLETE[7] = {
    {{1.0f, 0.0f, 0.0f}},  // Red
    {{0.0f, 1.0f, 0.0f}},  // Green
    {{0.0f, 0.0f, 1.0f}},  // Blue
    {{1.0f, 1.0f, 0.0f}},  // Yellow
    {{1.0f, 1.0f, 1.0f}},  // White
    {{0.3f, 0.0f, 0.5f}},  // Indigo
    {{0.5f, 0.0f, 0.5f}}   // Violet
};

int POINT_LIGHTS_BEGIN = 3;
int POINT_LIGHTS_END = 6;

// Scene data
Scene scene;
Vec4 slight_right;

Camera init_camera(int frame_width, int frame_height) {
    Camera cam;
    cam.game_object = game_object_default();
    cam.game_object.position = (Vec3){{0.0f, 1.0f, 20.0f}};
    cam.dirty_local = true;
    cam.fov = 50.0f;
    cam.aspect = (float)(frame_width) / frame_height;
    cam.z_near = 0.1f;
    cam.z_far = 1000.0f;
    cam.width = frame_width;
    cam.height = frame_height;
    return cam;
}

int alloc_objects(Scene *scene) {
    Vec3 white = COLLOR_PALLETE[4];
    // Vec3 blue = (Vec3){{0.0f, 0.5f, 1.0f}};

    // Objects
    int manual_objects = 7;
    scene->object_count = manual_objects + spider_object_amt;
    scene->dirty_locals = malloc(scene->object_count * sizeof(bool));
    if (scene->dirty_locals == NULL) {
        fprintf(stderr, "ERROR: malloc failed dirty_locals\n");
        return 1;
    }

    for (int i = 0; i < scene->object_count; ++i) {
        scene->dirty_locals[i] = true;
    }

    scene->attributes = malloc(scene->object_count * sizeof(GameObjectAttr));
    if (scene->attributes == NULL) {
        fprintf(stderr, "ERROR: malloc failed attributes\n");
        return 1;
    }

    scene->objects = malloc(scene->object_count * sizeof(GameObject));
    if (scene->objects == NULL) {
        fprintf(stderr, "ERROR: malloc failed objects\n");
        return 1;
    }

    scene->objects[0] = game_object_default();
    scene->objects[0].position = (Vec3){{0.0f, 1.0f, -3.0f}};
    scene->objects[0].scale = (Vec3){{5.0f, 5.0f, 5.0f}};
    scene->attributes[0].type = ATTR_MESH;
    scene->attributes[0].mesh = load_obj_mesh("../models/head.obj");
    scene->attributes[0].mesh.color = white;

    // scene->objects[1] = game_object_default();
    // scene->objects[1].quaternion = quat_from_units(UNIT_X, UNIT_Z);
    // scene->objects[1].position = (Vec3){{0.0f, 6.0f, -10.0f}};
    // scene->attributes[1].type = ATTR_MESH;
    // scene->attributes[1].mesh = load_obj_mesh("../models/light_box.obj");
    // scene->attributes[1].mesh.color = blue;

    // scene->objects[2] = game_object_default();
    // scene->objects[2].position = (Vec3){{-5.0f, -3.0f, -10.0f}};
    // scene->objects[2].scale = (Vec3){{6.0f, 6.0f, 6.0f}};
    // scene->attributes[2].type = ATTR_MESH;
    // scene->attributes[2].mesh = load_obj_mesh("../models/head.obj");
    // scene->attributes[2].mesh.color = white;

    float light_intensity = 50.0f;
    for (int i = POINT_LIGHTS_BEGIN; i < POINT_LIGHTS_END; ++i) {
        scene->objects[i] = game_object_default();
        scene->attributes[i].type = ATTR_LIGHT;
        scene->attributes[i].light.type = LIGHT_POINT;
        scene->attributes[i].light.color = COLLOR_PALLETE[i];
        scene->attributes[i].light.intensity = light_intensity;
    }

    // scene->objects[6] = game_object_default();
    // scene->attributes[6].type = ATTR_LIGHT;
    // scene->attributes[6].light.type = LIGHT_AMBIENT;
    // scene->attributes[6].light.color = (Vec3){{0.4f, 0.4f, 0.4f}};
    // scene->attributes[6].light.intensity = light_intensity;

    // memcpy(scene->objects + manual_objects, spider_game_objects,
    //        spider_object_amt * sizeof(GameObject));
    // memcpy(scene->attributes + manual_objects, spider_attrs,
    //        spider_object_amt * sizeof(GameObjectAttr));
    // scene->max_depth = MAX(scene->max_depth, spider_max_depth);

    // scene->objects[manual_objects].scale = (Vec3){{0.1f, 0.1f, 0.1f}};

    return 0;
}

Scene scene;
Vec4 slight_right;

void update(Real dt) {
    static float frames = 0;

    // Update object(s)
    // quat_mul(&scene.camera.game_object.quaternion, &slight_right);
    // scene.camera.game_object.needs_update = true;
    // End
    quat_mul(&scene.objects[7].quaternion, &slight_right);
    scene.dirty_locals[7] = true;
    float circle_radius = 10.0f;
    int num_lights = POINT_LIGHTS_END - POINT_LIGHTS_BEGIN;
    float angle_increment = 2.0f * M_PI / num_lights;
    for (int i = POINT_LIGHTS_BEGIN; i < POINT_LIGHTS_END; ++i) {
        float angle = angle_increment * (i + (frames / 20.0f));
        float x = circle_radius * cosf(angle);
        float z = circle_radius * sinf(angle);
        scene.dirty_locals[i] = true;
        scene.objects[i].position = (Vec3){{x, 0.0f, z}};
    }

    // Rotate model
    quat_mul(&scene.objects[0].quaternion, &slight_right);
    scene.dirty_locals[0] = true;
    ++frames;

    if (DEBUG_ENABLE_FPS_COUNT) {
        fps_counter();
    }
}

int SDL_main(int argc, char *argv[]) {
    // Start
    printf("Initializing mango renderer...\n");

    // Scene data
    slight_right = quat_from_axis(UNIT_Y, 0.01f);
    if (alloc_objects(&scene) != 0) {
        return 1;
    }

    Camera camera = init_camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    scene.camera = &camera;

    // Debug options
    scene.debug.wireframe = DEBUG_USE_WIREFRAME;
    scene.debug.rasterize = DEBUG_USE_RASTERIZE;

    printf("Success.\n");

    // Update loop
    printf("%s running...\n", GAME_TITLE);

    mango_on_update(&update);
    mango_run(&scene, GAME_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);

    printf("Mango closed successfully\n");

    return 0;
}
