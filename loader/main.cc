#include <string.h>

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

std::ostream &operator<<(std::ostream &os, const ufbx_tangent &t) {
    os << "{" << t.dx << ", " << t.dy << "} ";
    return os;
}

std::ostream &operator<<(std::ostream &os, const ufbx_keyframe &k) {
    os << "{" << k.time << ", " << k.value << ", " << k.interpolation << ", "
       << k.left << ", " << k.left << "}";
    return os;
}

enum AnimProp {
    ANIM_PROP_LCL_ROT,
    ANIM_PROP_LCL_SCL,
    ANIM_PROP_LCL_TSL,
};

std::string str_to_var_name(std::string const &name) {
    std::string res;
    std::copy_if(name.begin(), name.end(), std::back_inserter(res), isalnum);
    std::transform(res.begin(), res.end(), res.begin(), tolower);
    return res;
}

std::string str_parse_anim(std::string const &name) {
    auto iter = std::find(name.rbegin(), name.rend(), '|');
    if (iter == name.rend()) {
        return name;
    }
    std::string sub = name.substr(name.size() - (iter - name.rbegin()));
    std::string res;
    std::copy_if(sub.begin(), sub.end(), std::back_inserter(res), isalnum);
    return res;
}

struct BoneWeight {
    int bone_index;
    float weight;
};
std::ostream &operator<<(std::ostream &os, const BoneWeight &bw) {
    os << "{" << bw.bone_index << ", " << bw.weight << "}";
    return os;
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

        for (auto stack : scene->anim_stacks) {
            write_anim_stack(stack);
        }
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
    std::vector<const ufbx_node *> nodes_;
    uint32_t max_depth_ = 0;
    void push_nodes_depth_first(ufbx_node *node) {
        nodes_.push_back(node);
        max_depth_ = std::max(max_depth_, node->node_depth);
        for (auto child : node->children) {
            push_nodes_depth_first(child);
        }
    }

    void write_anim_stack(ufbx_anim_stack *stack) {
        std::string anim_name = str_parse_anim(stack->name.data);
        std::cout << stack->time_begin << std::endl;
        std::cout << stack->time_end << std::endl;
        for (auto layer : stack->layers) {
            for (int i = 0; i < layer->anim_props.count; ++i) {
                auto node_iter =
                    std::find_if(nodes_.begin(), nodes_.end(), [=](auto node) {
                        return node->name.data ==
                               layer->anim_props[i].element->name.data;
                    });
                int32_t node_i = node_iter - nodes_.begin();
                std::string prop_name =
                    anim_name + "_" + std::to_string(node_i) +
                    str_to_var_name(layer->anim_props[i].prop_name.data);
                write_list<ufbx_keyframe>(
                    "Keyframe " + prop_name + "X_curve",
                    layer->anim_values[i]->curves[0]->keyframes);
                write_list<ufbx_keyframe>(
                    "Keyframe " + prop_name + "Y_curve",
                    layer->anim_values[i]->curves[1]->keyframes);
                write_list<ufbx_keyframe>(
                    "Keyframe " + prop_name + "Z_curve",
                    layer->anim_values[i]->curves[2]->keyframes);
                c_ofs_ << "AnimValue " << prop_name << "_anim_value = {"
                       << layer->anim_values[i]->default_value << ", {{"
                       << prop_name << "X_curve,"
                       << layer->anim_values[i]->curves[0]->keyframes.count
                       << "}, {" << prop_name << "Y_curve,"
                       << layer->anim_values[i]->curves[1]->keyframes.count
                       << "}, {" << prop_name << "Z_curve,"
                       << layer->anim_values[i]->curves[2]->keyframes.count
                       << "}}};" << std::endl;
            }
        }
    }

    template <typename T, typename C>
    void write_list(std::string name, C const &list) {
        c_ofs_ << name << '[' << list.count << "] = {";
        for (T const &e : list) {
            c_ofs_ << e << ", ";
        }
        c_ofs_ << "};" << std::endl;
    }

    void write_game_objects() {
        h_ofs_ << "extern GameObject " << name_ << "_game_objects["
               << nodes_.size() << "];" << std::endl;
        c_ofs_ << "GameObject " << name_ << "_game_objects[" << nodes_.size()
               << "] = {" << std::endl;
        for (auto node : nodes_) {
            std::string name =
                node->is_root ? "root" : str_to_var_name(node->name.data);
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
            std::string name = str_to_var_name(node->name.data);

            if (node->mesh) {
                c_ofs_ << "{MESH, "
                       << ".mesh = {  .ind_count = "
                       << node->mesh->vertex_indices.count
                       << ", .vert_count = " << node->mesh->vertices.count
                       << ", .norm_count = "
                       << node->mesh->vertex_normal.values.count
                       << ", .uv_count = " << node->mesh->vertex_uv.values.count
                       << ", .weights = " << name << "_weights"
                       << ", .verts = " << name << "_verts"
                       << ", .norms = " << name << "_norms"
                       << ", .uvs = " << name << "_uvs"
                       << ", .vert_inds = " << name << "_vert_inds"
                       << ", .norm_inds = " << name << "_norm_inds"
                       << ", .uv_inds = " << name << "_uv_inds"
                       << ", .color = " << ufbx_vec3{1, 1, 1} << "}}, "
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
    void push_bone_weights(std::vector<std::vector<BoneWeight>> &weights,
                           ufbx_skin_deformer *deformer) {
        for (auto cluster : deformer->clusters) {
            int bone_i =
                std::find(nodes_.begin(), nodes_.end(), cluster->bone_node) -
                nodes_.begin();
            for (int i = 0; i < cluster->num_weights; ++i) {
                weights[cluster->vertices[i]].push_back({
                    bone_i,
                    (float)cluster->weights[i],
                });
            }
        }
    }

    void write_mesh_buffers(const ufbx_node *node) {
        std::string name = str_to_var_name(node->name.data);
        int32_t vert_count = node->mesh->vertices.count;
        std::vector<std::vector<BoneWeight>> bone_weights;
        bone_weights.resize(node->mesh->vertices.count);
        for (auto deformer : node->mesh->skin_deformers) {
            push_bone_weights(bone_weights, deformer);
        }
        auto max_weight = std::max_element(
            bone_weights.begin(), bone_weights.end(),
            [](auto const &a, auto const &b) { return a.size() < b.size(); });
        std::cout << name << " max " << max_weight->size() << std::endl;
        c_ofs_ << "BoneWeight " << name << "_weights[" << vert_count
               << "][8] = {";
        for (auto const &vert_weights : bone_weights) {
            c_ofs_ << "{";
            for (auto const &w : vert_weights) {
                c_ofs_ << w << ", ";
            }
            c_ofs_ << "}, ";
        }
        c_ofs_ << "};" << std::endl;

        write_list<ufbx_vec3>("Vec3 " + name + "_verts", node->mesh->vertices);

        write_list<uint32_t>("int " + name + "_vert_inds",
                             node->mesh->vertex_indices);

        write_list<ufbx_vec3>("Vec3 " + name + "_norms",
                              node->mesh->vertex_normal.values);

        write_list<uint32_t>("int " + name + "_norm_inds",
                             node->mesh->vertex_normal.indices);

        write_list<ufbx_vec2>("Vec2 " + name + "_uvs",
                              node->mesh->vertex_uv.values);

        write_list<uint32_t>("int " + name + "_uv_inds",
                             node->mesh->vertex_uv.indices);
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
