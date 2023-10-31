#ifndef ANIMATION_H
#define ANIMATION_H
#include <stdint.h>
#include <stdlib.h>

#include "../math/vec3.h"
#include "../math/vec4.h"

#define LIST_TYPE(p_name, p_type) \
    typedef struct p_name {       \
        p_type *data;             \
        size_t count;             \
    } p_name

typedef struct {
    int boneIndex;
    float weight;
} BoneWeight;

LIST_TYPE(BoneWeightList, BoneWeight);

typedef enum interpolation {
    INTERPOLATION_CONSTANT_PREV,  // < Hold previous key value
    INTERPOLATION_CONSTANT_NEXT,  // < Hold next key value
    INTERPOLATION_LINEAR,         // < Linear interpolation between two keys
    INTERPOLATION_CUBIC,          // < Cubic interpolation, see `ufbx_tangent`
} Interpolation;

typedef struct {
    float dx;  // < Derivative in the time axis
    float dy;  // < Derivative in the (curve specific) value axis
} Tangent;

typedef struct {
    float time;
    float value;
    Interpolation interpolation;
    Tangent left;
    Tangent right;
} Keyframe;

LIST_TYPE(KeyframeList, Keyframe);

typedef struct {
    KeyframeList keyframes;
} AnimCurve;

typedef struct {
    Vec3 default_value;
    AnimCurve curves[3];
} AnimValue;

typedef struct {
    float weight;
    bool weight_is_animated;
    bool blended;
    bool additive;
    bool compose_rotation;
    bool compose_scale;

    AnimValue *anim_values;
    int *bone_inds;  // < Sorted by `element,prop_name`
} AnimLayer;

LIST_TYPE(AnimLayerList, AnimLayer);

typedef struct {
    float time_begin;
    float time_end;
    AnimLayer layers;
} AnimStack;

#endif
