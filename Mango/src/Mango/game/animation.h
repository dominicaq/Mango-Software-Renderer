#ifndef ANIMATION_H
#define ANIMATION_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../math/vec3.h"
#include "../math/vec4.h"

#define LIST_TYPE(p_name, p_type) \
    typedef struct p_name {       \
        size_t count;             \
        p_type *data;             \
    } p_name

typedef enum interpolation {
    INTERPOLATION_CONSTANT_PREV,  // < Hold previous key value
    INTERPOLATION_CONSTANT_NEXT,  // < Hold next key value
    INTERPOLATION_LINEAR,         // < Linear interpolation between two keys
    INTERPOLATION_CUBIC,          // < Cubic interpolation, see `ufbx_tangent`
} Interpolation;

typedef struct {
    Real dx;  // < Derivative in the time axis
    Real dy;  // < Derivative in the (curve specific) value axis
} Tangent;

typedef struct {
    Real time;
    Real value;
    Interpolation interpolation;
    Tangent left;
    Tangent right;
} Keyframe;

LIST_TYPE(KeyframeList, Keyframe);

typedef struct {
    int node_index;
    Vec3 default_value;
    KeyframeList curves[3];
} AnimProp;

LIST_TYPE(AnimPropList, AnimProp);

typedef struct {
    Real weight;
    bool weight_is_animated;
    bool blended;
    bool additive;
    bool compose_rotation;
    bool compose_scale;

    AnimPropList anim_values;
} AnimLayer;

LIST_TYPE(AnimLayerList, AnimLayer);

typedef struct {
    Real time_begin;
    Real time_end;
    AnimLayerList layers;
} AnimStack;

#endif
