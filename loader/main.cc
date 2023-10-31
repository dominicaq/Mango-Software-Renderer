#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include "ufbx/ufbx.h"

std::ostream &operator<<(std::ostream &os, const ufbx_vec2 &v) {
    os << "{{" << v.x << ", " << v.y << "}}";
    return os;
}
std::ostream &operator<<(std::ostream &os, const ufbx_vec3 &v) {
    os << "{{" << v.x << ", " << v.y << ", " << v.z << "}}";
    return os;
}
std::ostream &operator<<(std::ostream &os, const ufbx_quat &q) {
    os << "{{" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "}}";
    return os;
}

std::string node_parse_name(ufbx_node *node) {
    std::string name;
    if (node->is_root) {
        name = "root";
    } else {
        std::copy_if(node->name.data, node->name.data + node->name.length,
                     std::back_inserter(name), isalnum);
        std::transform(name.begin(), name.end(), name.begin(), tolower);
    }
    return name;
}

class FBXWriter {
   public:
    FBXWriter(std::string fbx_path, std::string output_path, std::string name) {
        name_ = name;
        ufbx_load_opts opts = {0};
        ufbx_error error;
        ufbx_scene *scene = ufbx_load_file(fbx_path.data(), &opts, &error);
        if (!scene) {
            std::cerr << "Failed to load " << fbx_path << " : "
                      << error.description.data << std::endl;
            exit(1);
        }

        std::string h_filename = name + ".h";
        std::string h_path = output_path + h_filename;
        h_ofs_ = std::ofstream(h_path);
        std::string c_path = output_path + name + ".c";
        c_ofs_ = std::ofstream(c_path);
        auto upper_model_path = std::string(name);
        std::transform(upper_model_path.begin(), upper_model_path.end(),
                       upper_model_path.begin(), toupper);

        h_ofs_ << "#ifndef " << upper_model_path << "_H" << std::endl;
        h_ofs_ << "#define " << upper_model_path << "_H" << std::endl;
        h_ofs_ << "#include \"../math/vec3.h\"" << std::endl;
        h_ofs_ << "#include \"../math/vec2.h\"" << std::endl;
        h_ofs_ << "#include \"../game/gameobject.h\"" << std::endl;
        c_ofs_ << "#include \"" << h_filename << "\"" << std::endl;
        push_nodes_depth_first(scene->root_node);
        h_ofs_ << "extern int " << name << "_object_amt;" << std::endl;
        c_ofs_ << "int " << name << "_object_amt = " << nodes_.size() << ";"
               << std::endl;
        h_ofs_ << "extern int " << name << "_max_depth;" << std::endl;
        c_ofs_ << "int " << name << "_max_depth = " << max_depth_ << ";"
               << std::endl;

        for (auto node : nodes_) {
            if (node->mesh) {
                write_mesh_buffers(node);
            }
        }
        write_game_objects();
        write_attributes();

        h_ofs_ << "#endif" << std::endl;

        ufbx_free_scene(scene);

        std::cout << "wrote: " << h_path << ", " << c_path << std::endl;
    }

   private:
    std::string name_;
    std::ofstream h_ofs_;
    std::ofstream c_ofs_;
    std::vector<ufbx_node *> nodes_;
    uint32_t max_depth_ = 0;
    void push_nodes_depth_first(ufbx_node *node) {
        nodes_.push_back(node);
        max_depth_ = std::max(max_depth_, node->node_depth);
        for (auto child : node->children) {
            push_nodes_depth_first(child);
        }
    }

    void write_game_objects() {
        h_ofs_ << "extern GameObject " << name_ << "_game_objects["
               << nodes_.size() << "];" << std::endl;
        c_ofs_ << "GameObject " << name_ << "_game_objects[" << nodes_.size()
               << "] = {" << std::endl;
        for (auto node : nodes_) {
            std::string name = node_parse_name(node);
            c_ofs_ << "{.position = " << node->local_transform.translation
                   << ", .quaternion = " << node->local_transform.rotation
                   << ", .scale = " << node->local_transform.scale
                   << ", .num_children = " << node->children.count << "}, "
                   << std::endl;
        }
        c_ofs_ << "};" << std::endl;
    }
    void write_attributes() {
        h_ofs_ << "extern GameObjectAttr " << name_ << "_attrs["
               << nodes_.size() << "];" << std::endl;
        c_ofs_ << "GameObjectAttr " << name_ << "_attrs[" << nodes_.size()
               << "] = {";
        for (auto node : nodes_) {
            std::string name = node_parse_name(node);

            if (node->mesh) {
                c_ofs_ << "{MESH, "
                       << ".mesh = {  .ind_count = "
                       << node->mesh->vertex_indices.count
                       << ", .vert_count = " << node->mesh->vertices.count
                       << ", .norm_count = "
                       << node->mesh->vertex_normal.values.count
                       << ", .uv_count = " << node->mesh->vertex_uv.values.count
                       << ", .verts = " << name << "_verts, .norms = " << name
                       << "_norms, .uvs = " << name
                       << "_uvs, .vert_inds = " << name
                       << "_vert_inds, .norm_inds = " << name
                       << "_norm_inds, .uv_inds = " << name
                       << "_uv_inds, .color = " << ufbx_vec3{1, 1, 1} << "}}, "
                       << std::endl;
            } else if (node->bone) {
                c_ofs_ << "{BONE, "
                       << ".bone = { .radius = " << node->bone->radius
                       << ", .length = " << node->bone->relative_length << "}},"
                       << std::endl;
            } else {
                c_ofs_ << "{NONE}, " << std::endl;
            }
        }
        c_ofs_ << "};";
    }
    void write_mesh_buffers(ufbx_node *node) {
        std::string name = node_parse_name(node);
        int32_t vert_count = node->mesh->vertices.count;
        h_ofs_ << "extern Vec3 " << name << "_verts[" << vert_count << "];"
               << std::endl;
        c_ofs_ << "Vec3 " << name << "_verts[" << vert_count << "] = {";
        for (auto const &v : node->mesh->vertices) {
            c_ofs_ << v << ", ";
        }
        c_ofs_ << "};" << std::endl;

        int32_t vert_inds_count = node->mesh->vertex_indices.count;
        h_ofs_ << "extern int " << name << "_vert_inds[" << vert_inds_count
               << "];" << std::endl;
        c_ofs_ << "int " << name << "_vert_inds[" << vert_inds_count << "] = {";
        for (int32_t i : node->mesh->vertex_indices) {
            c_ofs_ << i << ", ";
        }
        c_ofs_ << "};" << std::endl;

        int32_t norm_count = node->mesh->vertex_normal.values.count;
        h_ofs_ << "extern Vec3 " << name << "_norms[" << norm_count << "];"
               << std::endl;
        c_ofs_ << "Vec3 " << name << "_norms[" << norm_count << "] = {";
        for (auto const &v : node->mesh->vertex_normal.values) {
            c_ofs_ << v << ", ";
        }
        c_ofs_ << "};" << std::endl;

        int32_t norm_inds_count = node->mesh->vertex_normal.indices.count;
        h_ofs_ << "extern int " << name << "_norm_inds[" << norm_inds_count
               << "];" << std::endl;
        c_ofs_ << "int " << name << "_norm_inds[" << norm_inds_count << "] = {";
        for (int32_t i : node->mesh->vertex_normal.indices) {
            c_ofs_ << i << ", ";
        }
        c_ofs_ << "};" << std::endl;

        int32_t uv_count = node->mesh->vertex_uv.values.count;
        h_ofs_ << "extern Vec2 " << name << "_uvs[" << uv_count << "];"
               << std::endl;
        c_ofs_ << "Vec2 " << name << "_uvs[" << uv_count << "] = {";
        for (auto const &v : node->mesh->vertex_uv.values) {
            c_ofs_ << v << ", ";
        }
        c_ofs_ << "};" << std::endl;

        int32_t uv_inds_count = node->mesh->vertex_uv.indices.count;
        h_ofs_ << "extern int " << name << "_uv_inds[" << uv_inds_count << "];"
               << std::endl;
        c_ofs_ << "int " << name << "_uv_inds[" << uv_inds_count << "] = {";
        for (auto i : node->mesh->vertex_uv.indices) {
            c_ofs_ << i << ", ";
        }
        c_ofs_ << "};" << std::endl;
    }
};

void load_fbx_scene(const char *fbx_path, const char *output_path) {}

int32_t main(int32_t argc, char **argv) {
    if (argc != 4) {
        std::cerr << "invalid arguments" << std::endl;
        return 1;
    }
    FBXWriter(argv[1], argv[2], argv[3]);
}
