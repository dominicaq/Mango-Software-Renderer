
#include <Mango/mango.h>
#include <Mango/mesh/obj_parser.h>
#include <Mango/render/drawing.h>
#include <Mango/render/framedata.h>

// Pre parsed game data
#include "cube.h"

// Window data
const char *GAME_TITLE = "Mango Renderer";
// -----------------------------------------------------------------------------

// Lights
const Vec3 COLLOR_PALLETE[7] = {
    {{255.0f, 0.0f, 0.0f}},  // Red
    {{0.0f, 255.0f, 0.0f}},  // Green
    {{0.0f, 0.0f, 1.0f}},    // Blue
    {{1.0f, 1.0f, 0.0f}},    // Yellow
    {{1.0f, 1.0f, 1.0f}},    // White
    {{0.3f, 0.0f, 0.5f}},    // Indigo
    {{0.5f, 0.0f, 0.5f}}     // Violet
};

int POINT_LIGHTS_BEGIN = 3;
int POINT_LIGHTS_END = 6;

// Scene data
Scene scene;
Vec4 slight_right;
Vec4 slight_left;

Vec4 slight_up;
Vec4 slight_down;

Camera init_camera(int frame_width, int frame_height) {
    Camera cam;
    cam.game_object = game_object_default();
    cam.game_object.position = (Vec3){{0.0f, 1.0f, 60.0f}};
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
    // Vec3 white = COLLOR_PALLETE[4];
    // Vec3 blue = (Vec3){{0.0f, 0.5f, 1.0f}};

    // Objects
    int32_t cube_amt = 2;
    scene->object_count = cube_amt * cube_object_amt;
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

    for (int32_t i = 0; i < cube_amt; ++i) {
        int cube_i = i * cube_object_amt;
        memcpy(scene->objects + cube_i, cube_game_objects,
               cube_object_amt * sizeof(GameObject));
        memcpy(scene->attributes + cube_i, cube_attrs,
               cube_object_amt * sizeof(GameObjectAttr));
        scene->max_depth = MAX(scene->max_depth, cube_max_depth);

        scene->objects[cube_i].scale = (Vec3){{0.1f, 0.1f, 0.1f}};
        scene->attributes[cube_i + 1].mesh.color = COLLOR_PALLETE[i];
    }

    return 0;
}

Mango *mango;
Vec4 slight_right;
Real attack_cd = 0;
int cube0 = 1;
int cube1 = 3;

void update(Real dt) {
    static float frames = 0;
    attack_cd += dt;
    if (attack_cd > 3000) {
        // mango_play_anim(mango, 7, &cube_Attack_anim);
        attack_cd = 0;
    }

    // Update object(s)
    // quat_mul(&scene.camera.game_object.quaternion, &slight_right);
    // scene.camera.game_object.needs_update = true;
    // End
    if (vec4_magnitude(vec4_sub(scene.objects[cube0].quaternion,
                                scene.objects[cube1].quaternion)) < 0.1) {
        Real x = clock();
        int y = srand(real_to_i32(x));
        int z = srand(y);
        int w = srand(z);

        Vec4 new_quat = {
            {x, real_from_i32(y), real_from_i32(z), real_from_i32(w)}};
        // Vec4 new_quat = {{2, 3, 324, 2}};
        scene.objects[cube1].quaternion = quat_normalize(new_quat);
        scene.dirty_locals[cube1] = true;
    }
    uint32_t controls = get_controller();
    if (controls & INPUT_DIRECTION_RIGHT) {
        scene.objects[cube0].quaternion =
            quat_mul(slight_right, scene.objects[cube0].quaternion);
        scene.dirty_locals[cube0] = true;
    }

    if (controls & INPUT_DIRECTION_LEFT) {
        scene.objects[cube0].quaternion =
            quat_mul(slight_left, scene.objects[cube0].quaternion);
        scene.dirty_locals[cube0] = true;
    }

    if (controls & INPUT_DIRECTION_UP) {
        scene.objects[cube0].quaternion =
            quat_mul(slight_up, scene.objects[cube0].quaternion);
        scene.dirty_locals[cube0] = true;
    }

    if (controls & INPUT_DIRECTION_DOWN) {
        scene.objects[cube0].quaternion =
            quat_mul(slight_down, scene.objects[cube0].quaternion);
        scene.dirty_locals[cube0] = true;
    }

    if (controls & INPUT_BUTTON_1) {
        mango->ubo.options = OPT_USE_RASTERIZE | OPT_VIEW_NORMALS;
    }
    if (controls & INPUT_BUTTON_2) {
        mango->ubo.options = OPT_USE_WIREFRAME;
    }

    ++frames;
}

int main(int argc, char *argv[]) {
    // Start
    printf("Initializing mango renderer...\n");

    // Scene data
    slight_right = quat_from_axis(UNIT_Y, 0.05f);
    slight_left = quat_from_axis(UNIT_Y, -0.05f);

    slight_up = quat_from_axis(UNIT_X, -0.05f);
    slight_down = quat_from_axis(UNIT_X, 0.05f);

    if (alloc_objects(&scene) != 0) {
        return 1;
    }

    Camera camera = init_camera(BG_W, BG_H);
    scene.camera = &camera;

    // Debug options
    scene.options = OPT_USE_WIREFRAME;

    printf("Success.\n");

    // Update loop
    printf("%s running...\n", GAME_TITLE);

    mango = mango_alloc(&scene, GAME_TITLE, BG_W, BG_H);
    mango->user_update = &update;
    mango_run(mango);

    printf("Mango closed successfully\n");

    return 0;
}
