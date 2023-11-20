#include <string.h>

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include "ufbx/ufbx.h"

std::string str_sanitized(std::string const &name) {
    std::string sub = name;
    auto iter = std::find(name.rbegin(), name.rend(), '|');
    if (iter != name.rend()) {
        sub = name.substr(name.size() - (iter - name.rbegin()));
    }
    std::string res;
    std::copy_if(sub.begin(), sub.end(), std::back_inserter(res), isalnum);
    return res;
}

template <typename T, typename C>
void write_list(std::ostream &os, C const &list) {
    os << "{";
    for (T const &e : list) {
        os << e << ", ";
    }
    os << "}";
}

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
        h_ofs_ << "#include <Mango/math/vec3.h>" << std::endl;
        h_ofs_ << "#include <Mango/math/vec2.h>" << std::endl;
        h_ofs_ << "#include <Mango/game/gameobject.h>" << std::endl;
        c_ofs_ << "#include \"" << h_filename << "\"" << std::endl;
        c_ofs_ << "#include <Mango/game/animation.h>" << std::endl;
        c_ofs_ << "#include \"" << h_filename << "\"" << std::endl;
        push_nodes_depth_first(scene->root_node);
        h_ofs_ << "extern int " << name << "_object_amt;" << std::endl;
        c_ofs_ << "int " << name << "_object_amt = " << nodes_.size() << ";"
               << std::endl;
        h_ofs_ << "extern int " << name << "_max_depth;" << std::endl;
        c_ofs_ << "int " << name << "_max_depth = " << max_depth_ << ";"
               << std::endl;

        for (auto stack : scene->anim_stacks) {
            write_anim_stack(*stack);
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
    void write_anim_stack(ufbx_anim_stack const &s) {
        std::string anim_name = str_sanitized(s.name.data);
        // build_layer names
        auto layer_names = std::vector<std::string>();
        std::transform(s.layers.begin(), s.layers.end(),
                       std::back_inserter(layer_names), [&](auto l) {
                           return anim_name + "_" + str_sanitized(l->name.data);
                       });
        // build prop names
        auto prop_names = std::vector<std::vector<std::string>>(s.layers.count);
        for (int i = 0; i < s.layers.count; ++i) {
            std::transform(s.layers[i]->anim_props.begin(),
                           s.layers[i]->anim_props.end(),
                           std::back_inserter(prop_names[i]),
                           [&, i](ufbx_anim_prop const &p) {
                               return layer_names[i] +
                                      std::to_string(p.element->element_id) +
                                      str_sanitized(p.prop_name.data);
                           });
        }
        // write keyframes
        for (int i = 0; i < s.layers.count; ++i) {
            auto l = s.layers[i];
            for (int j = 0; j < l->anim_props.count; ++j) {
                auto const &p = l->anim_props[j];
                c_ofs_ << "Keyframe " << prop_names[i][j] + "0"
                       << "[]=";
                write_list<ufbx_keyframe>(c_ofs_,
                                          p.anim_value->curves[0]->keyframes);
                c_ofs_ << ";" << std::endl;
                c_ofs_ << "Keyframe " << prop_names[i][j] + "1"
                       << "[]=";
                write_list<ufbx_keyframe>(c_ofs_,
                                          p.anim_value->curves[0]->keyframes);
                c_ofs_ << ";" << std::endl;
                c_ofs_ << "Keyframe " << prop_names[i][j] + "2"
                       << "[]=";
                write_list<ufbx_keyframe>(c_ofs_,
                                          p.anim_value->curves[0]->keyframes);
                c_ofs_ << ";" << std::endl;
            }
        }
        // write props
        for (int i = 0; i < s.layers.count; ++i) {
            auto l = s.layers[i];
            c_ofs_ << "AnimProp " << layer_names[i] << "[]={";
            for (int j = 0; j < l->anim_props.count; ++j) {
                auto const &p = l->anim_props[j];
                c_ofs_ << "{" << p.element->element_id << ", "
                       << p.anim_value->default_value << ", {{"
                       << p.anim_value->curves[0]->keyframes.count << ", "
                       << prop_names[i][j] + "0}, {"
                       << p.anim_value->curves[1]->keyframes.count << ", "
                       << prop_names[i][j] + "1}, {"
                       << p.anim_value->curves[2]->keyframes.count << ",  "
                       << prop_names[i][j] + "2}}}, ";
            }
            c_ofs_ << "};" << std::endl;
        }

        // write layers
        std::string layer_list_name = anim_name + "_layers";
        c_ofs_ << "AnimLayer " << layer_list_name << "[]={";
        for (int i = 0; i < s.layers.count; ++i) {
            auto l = s.layers[i];
            c_ofs_ << "{" << l->weight << ", " << l->weight_is_animated << ", "
                   << l->blended << ", " << l->additive << ", "
                   << l->compose_rotation << ", " << l->compose_scale << ", "
                   << "{" << l->anim_props.count << ", " << layer_names[i]
                   << "}}";
        }
        // write stack
        c_ofs_ << "};" << std::endl;
        h_ofs_ << "extern AnimStack " << anim_name << ";" << std::endl;
        c_ofs_ << "AnimStack " << anim_name << "={" << s.time_begin << ", "
               << s.time_end << ", {" << s.layers.count << ", " << anim_name
               << "_layers"
               << "}};";
    }

    void write_game_objects() {
        h_ofs_ << "extern GameObject " << name_ << "_game_objects["
               << nodes_.size() << "];" << std::endl;
        c_ofs_ << "GameObject " << name_ << "_game_objects[" << nodes_.size()
               << "] = {" << std::endl;
        for (auto node : nodes_) {
            std::string name =
                node->is_root ? "root" : str_sanitized(node->name.data);
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
            std::string name = str_sanitized(node->name.data);

            if (node->mesh) {
                c_ofs_ << "{ATTR_MESH, "
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
                c_ofs_ << "{ATTR_BONE, "
                       << ".bone = { .radius = " << node->bone->radius
                       << ", .length = " << node->bone->relative_length << "}},"
                       << std::endl;
            } else {
                c_ofs_ << "{ATTR_NONE}, " << std::endl;
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
        std::string name = str_sanitized(node->name.data);
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

        c_ofs_ << "Vec3 " + name + "_verts[]=";
        write_list<ufbx_vec3>(c_ofs_, node->mesh->vertices);
        c_ofs_ << ";" << std::endl;

        c_ofs_ << "int " + name + "_vert_inds[]=";
        write_list<uint32_t>(c_ofs_, node->mesh->vertex_indices);
        c_ofs_ << ";" << std::endl;

        c_ofs_ << "Vec3 " + name + "_norms[]=";
        write_list<ufbx_vec3>(c_ofs_, node->mesh->vertex_normal.values);
        c_ofs_ << ";" << std::endl;

        c_ofs_ << "int " + name + "_norm_inds[]=",
            write_list<uint32_t>(c_ofs_, node->mesh->vertex_normal.indices);
        c_ofs_ << ";" << std::endl;

        c_ofs_ << "Vec2 " + name + "_uvs[]=";
        write_list<ufbx_vec2>(c_ofs_, node->mesh->vertex_uv.values);
        c_ofs_ << ";" << std::endl;

        c_ofs_ << "int " + name + "_uv_inds[]=";
        write_list<uint32_t>(c_ofs_, node->mesh->vertex_uv.indices);
        c_ofs_ << ";" << std::endl;
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
