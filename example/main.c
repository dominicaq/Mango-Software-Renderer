#include <Mango/mango.h>
#include <Mango/mesh/obj_parser.h>
#include <Mango/render/drawing.h>
#include <Mango/render/framedata.h>
#include <time.h>
#include <string.h>

// Window data
const char *GAME_TITLE = "Mango Renderer";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void fps_counter() {
    static int frames = 0;
    static clock_t start, end;
    static float elapsed_time;

    // Print FPS every second
    end = clock();
    elapsed_time = (float)(end - start) / CLOCKS_PER_SEC;
    if (elapsed_time >= 1.0) {
        float fps = frames / elapsed_time;
        printf("FPS: %f\n", fps);

        // Reset counters for the next second
        start = clock();
        frames = 0;
    }
    ++frames;
}

// Game
// -----------------------------------------------------------------------------
Scene scene;
Vec4 slight_right;

int POINT_LIGHTS_BEGIN = 2;
int POINT_LIGHTS_END = 3;
const Vec3 COLLOR_PALLETE[7] = {
    {{1.0f, 0.0f, 0.0f}},  // Red
    {{0.0f, 1.0f, 0.0f}},  // Green
    {{0.3f, 0.3f, 0.8f}},  // Custom color
    {{1.0f, 1.0f, 1.0f}},  // White
    {{1.0f, 1.0f, 0.0f}},  // Yellow
    {{0.3f, 0.0f, 0.5f}},  // Indigo
    {{0.5f, 0.0f, 0.5f}}   // Violet
};

Camera init_camera(int frame_width, int frame_height) {
    Camera cam;
    cam.game_object = game_object_default();
    cam.game_object.position = (Vec3){{0.0f, 1.0f, 25.0f}};
    cam.dirty_local = true;
    cam.fov = 90.0f;
    cam.aspect = (float)(frame_width) / frame_height;
    cam.z_near = 0.01f;
    cam.z_far = 100.0f;
    cam.width = frame_width;
    cam.height = frame_height;
    return cam;
}

int alloc_objects(Scene *scene) {
    Vec3 white = (Vec3){{1.0f, 1.0f, 1.0f}};

    // Objects
    int manual_objects = 7;
    scene->object_count = manual_objects;
    scene->dirty_locals = (bool *)malloc(scene->object_count * sizeof(bool));
    if (scene->dirty_locals == NULL) {
        printf("ERROR: malloc failed dirty_locals\n");
        return 1;
    }

    for (int i = 0; i < scene->object_count; ++i) {
        scene->dirty_locals[i] = true;
    }

    scene->attributes =
        (GameObjectAttr *)malloc(scene->object_count * sizeof(GameObjectAttr));
    if (scene->attributes == NULL) {
        printf("ERROR: malloc failed attributes\n");
        return 1;
    }

    scene->objects =
        (GameObject *)malloc(scene->object_count * sizeof(GameObject));
    if (scene->objects == NULL) {
        printf("ERROR: malloc failed objects\n");
        return 1;
    }

    // Scene object 1
    scene->objects[0] = game_object_default();
    scene->objects[0].position = (Vec3){{-10.0f, 0.0f, -10.0f}};
    scene->objects[0].scale = (Vec3){{15.0f, 15.0f, 15.0f}};
    scene->attributes[0].type = ATTR_MESH;
    scene->attributes[0].mesh = load_obj_mesh("../example/models/Head.obj");
    scene->attributes[0].mesh.color = white;

    Material *mat0 = malloc(sizeof(Material));
    if (mat0 == NULL) {
        printf("ERROR: malloc failed mat0\n");
        return 1;
    }

    Texture *default_texture = load_texture("../example/textures/diablo3_pose_diffuse.tga");
    if (default_texture == NULL) {
        printf("ERROR: failed to create default texture\n");
        return 1;
    }
    mat0->albedo_map = default_texture;
    scene->attributes[0].mesh.material = mat0;

    // Scene Object 2
    // Box (an example of adding multiple objects)
    // scene->objects[1] = game_object_default();
    // scene->objects[1].quaternion = quat_from_units(UNIT_X, UNIT_Z);
    // scene->objects[1].position = (Vec3){{0.0f, 6.0f, -10.0f}};
    // scene->attributes[1].type = ATTR_MESH;
    // scene->attributes[1].mesh = load_obj_mesh("../models/light_box.obj");
    // scene->attributes[1].mesh.color = blue;

    // Lights
    for (int i = POINT_LIGHTS_BEGIN; i < POINT_LIGHTS_END; ++i) {
        scene->objects[i] = game_object_default();
        scene->attributes[i].type = ATTR_LIGHT;
        scene->attributes[i].light.type = LIGHT_POINT;
        scene->attributes[i].light.color = COLLOR_PALLETE[i];
        scene->attributes[i].light.intensity = 15.0f;
        scene->attributes[i].light.radius = 10.0f;
    }

    return 0;
}

Mango *mango;
Vec4 slight_right;
float attack_cd = 0;

void start() {
    slight_right = quat_from_axis(UNIT_Y, 0.001f);
}

void free_camera(float dt) {
    // const Uint8 *state = SDL_GetKeyboardState(NULL);

    // float cameraSpeed = 5.0f * dt; // Adjust speed as necessary

    // Vec3 forward = vec3_scale(scene.camera->game_object.forward, cameraSpeed);
    // Vec3 right = vec3_scale(scene.camera->game_object.right, cameraSpeed);

    // if (state[SDL_SCANCODE_W]) {
    //     scene.camera->game_object.position = vec3_add(scene.camera->game_object.position, forward);
    //     scene.camera->dirty_local = true;
    // }
    // if (state[SDL_SCANCODE_S]) {
    //     scene.camera->game_object.position = vec3_sub(scene.camera->game_object.position, forward);
    //     scene.camera->dirty_local = true;
    // }
    // if (state[SDL_SCANCODE_A]) {
    //     scene.camera->game_object.position = vec3_sub(scene.camera->game_object.position, right);
    //     scene.camera->dirty_local = true;
    // }
    // if (state[SDL_SCANCODE_D]) {
    //     scene.camera->game_object.position = vec3_add(scene.camera->game_object.position, right);
    //     scene.camera->dirty_local = true;
    // }
}

void update(float dt) {
    static float frames = 0;

    // Rotate camera
    // quat_mul(&scene.camera->game_object.quaternion, &slight_right);
    // scene.camera->dirty_local = true;

    free_camera(dt);

    // quat_mul(&scene.objects[7].quaternion, &slight_right);
    // scene.dirty_locals[7] = true;

    // quat_mul(&scene.camera->game_object.quaternion, &slight_right);
    // scene.camera->dirty_local = true;

    // Rotate 1st model
    quat_mul(&scene.objects[0].quaternion, &slight_right);
    scene.dirty_locals[0] = true;

    // Circling point lights
    float circle_radius = 30.0f;
    int num_lights = POINT_LIGHTS_END - POINT_LIGHTS_BEGIN;
    float angle_increment = 2.0f * M_PI / num_lights;
    for (int i = POINT_LIGHTS_BEGIN; i < POINT_LIGHTS_END; ++i) {
        float angle = angle_increment * (i + (frames / 1000.0f));
        float x = circle_radius * cosf(angle);
        float z = circle_radius * sinf(angle);
        scene.dirty_locals[i] = true;
        scene.objects[i].position = (Vec3){{x, 5.0f, z}};
    }

    if (scene.options & OPT_FPS_COUNTER) {
        fps_counter();
    }
    ++frames;
}

int MAIN(int argc, char *argv[]) {
    printf("Initializing mango renderer...\n");

    // Scene data
    if (alloc_objects(&scene) != 0) {
        return 1;
    }

    Camera camera = init_camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    scene.camera = &camera;
    scene.options = OPT_USE_WIREFRAME | OPT_NO_LIGHTING | OPT_FPS_COUNTER | OPT_DEBUG_CLIP_PLANE;
    mango = mango_alloc(&scene, GAME_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    printf("Success.\n");

    // Game core
    start();
    printf("%s running...\n", GAME_TITLE);
    mango->user_update = &update;
    mango_run(mango);

    printf("Mango closed successfully\n");

    return 0;
}
