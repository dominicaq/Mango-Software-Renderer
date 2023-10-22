// #include <stdlib.h>
// #include <stdio.h>
// #include "animation.h"
// #include "ufbx/ufbx.h"

// void *load_fbx_scene(const char * filename) {
//   ufbx_load_opts opts = { 0 };
//   ufbx_error error;
//   ufbx_scene *scene = ufbx_load_file(filename, &opts, &error);
//   if (!scene) {
//     fprintf(stderr, "Failed to load: %s\n", error.description.data);
//     exit(1);
//   }
//   for (size_t i = 0; i < scene->nodes.count; i++) {
//       ufbx_node *node = scene->nodes.data[i];
//       if (node->is_root) continue;

//       printf("Object: %s\n", node->name.data);
//       if (node->mesh) {
//           printf("-> mesh with %zu faces\n", node->mesh->faces.count);
//       }
//   }

//   ufbx_free_scene(scene);
//   return NULL;
// }
