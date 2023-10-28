#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <ranges>
#include "ufbx/ufbx.h"
#include "math/vec3.h"

void *load_fbx_scene(const char * model_path, const char *output_name) {
  ufbx_load_opts opts = { 0 };
  ufbx_error error;
  char path[256];
  snprintf(path, 256, "../models/%s", model_path);
  ufbx_scene *scene = ufbx_load_file(path, &opts, &error);
  if (!scene) {
    std::cerr << "Failed to load:" << error.description.data << std::endl;
    exit(1);
  }

  std::string dest = "../src/models/";
  std::string h_filename = std::string(output_name) + ".h";
  std::ofstream h_file(dest + h_filename, std::ios::out);
  std::string c_filename = std::string(output_name) + ".c";
  std::ofstream c_file(dest + c_filename, std::ios::out);
  std::string upper_model_path(output_name);
  std::transform(upper_model_path.begin(), upper_model_path.end(), upper_model_path.begin(), ::toupper); 

  h_file << "#ifndef " << upper_model_path << "_H" << std::endl;
  h_file << "#define " << upper_model_path << "_H" << std::endl;
  h_file << "#include \"../math/vec3.h\"" <<std::endl;
  h_file << "#include \"../math/vec2.h\"" <<std::endl;
  h_file << "#include \"../mesh/mesh.h\"" <<std::endl;
  c_file << "#include \"" << h_filename << "\"" <<std::endl;
  auto write_vec3 = [&] (ufbx_vec3 const &v) {
    c_file << "{" << v.x << ", " << v.y << ", " << v.z  << "}, ";
  };
  auto write_vec2 = [&] (ufbx_vec2 const &v) {
    c_file << "{" << v.x << ", " << v.y << "}, ";
  };
  auto write_i = [&] (int32_t i) {
    c_file << i << ", ";
  };
  for (size_t i = 0; i < scene->nodes.count; ++i) {
    ufbx_node *node = scene->nodes.data[i];
    if (node->is_root) continue;

    if (node->mesh) {
      auto sanitized = std::string(node->mesh->name.data) | std::views::filter(isalnum);
      std::string name(sanitized.begin(), sanitized.end());
      int32_t vert_count = node->mesh->vertices.count;
      h_file << "vec3 " << name << "_verts[" << vert_count << "];" << std::endl;
      c_file << "vec3 " << name << "_verts[" << vert_count << "] = {";
      std::ranges::for_each(node->mesh->vertices, write_vec3);
      c_file << "};" << std::endl;

      int32_t vert_inds_count = node->mesh->vertex_indices.count;
      h_file << "int32_t " << name << "_vert_inds[" << vert_inds_count << "];" << std::endl;
      c_file << "int32_t " << name << "_vert_inds[" << vert_inds_count << "] = {";
      std::ranges::for_each(node->mesh->vertex_indices, write_i);
      c_file << "};" << std::endl;

      int32_t norm_count = node->mesh->vertex_normal.values.count;
      h_file << "vec3 " << name << "_norms[" << norm_count << "];" << std::endl;
      c_file << "vec3 " << name << "_norms[" << norm_count << "] = {";
      std::ranges::for_each(node->mesh->vertex_normal.values, write_vec3);
      c_file << "};" << std::endl;

      int32_t norm_inds_count = node->mesh->vertex_normal.indices.count;
      h_file << "int32_t " << name << "_norm_inds[" << norm_inds_count << "];" << std::endl;
      c_file << "int32_t " << name << "_norm_inds[" << norm_inds_count << "] = {";
      std::ranges::for_each(node->mesh->vertex_indices, write_i);
      c_file << "};" << std::endl;

      int32_t uv_count = node->mesh->vertex_uv.values.count;
      h_file << "vec2 " << name << "_uvs[" << uv_count << "];" << std::endl;
      c_file << "vec2 " << name << "_uvs[" << uv_count << "] = {";
      std::ranges::for_each(node->mesh->vertex_uv.values, write_vec2);
      c_file << "};" << std::endl;

      int32_t uv_inds_count = node->mesh->vertex_uv.indices.count;
      h_file << "int32_t " << name << "_uv_inds[" << uv_inds_count << "];" << std::endl;
      c_file << "int32_t " << name << "_uv_inds[" << uv_inds_count << "] = {";
      std::ranges::for_each(node->mesh->vertex_uv.indices, write_i);
      c_file << "};" << std::endl;

      h_file << "Mesh " << name << "_mesh;" << std::endl;
      c_file << "Mesh " << name << "_mesh = { " 
        ".ind_count = " << vert_inds_count << ", "
        ".vert_count = " << vert_count << ", "
        ".norm_count = " << norm_count << ", "
        ".uv_count = " << uv_count  << ", "

        ".verts = " << name << "_verts, "
        ".norms = " << name << "_norms, "
        ".uvs = " << name << "_uvs, "

        ".vert_inds = " << name << "_vert_inds, "
        ".norm_inds = " << name << "_norm_inds, "
        ".uv_inds = " << name << "_uv_inds, "
        "};" << std::endl;

    }
  }
  h_file << "#endif" << std::endl;

  ufbx_free_scene(scene);

  return NULL;
}


int32_t main(int32_t argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "invalid arguments" << std::endl;
  }
  load_fbx_scene(argv[1], argv[2]);
}
