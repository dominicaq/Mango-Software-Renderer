#include "animation.h"

void prop_update(AnimProp *prop, GameObject *obj, float time_prog) {
    Vec3 values;
    for (int i = 0; i < 3; ++i) {
        Keyframe key_prev = {0, prop->default_value.elem[i], 0};

        float time_prev = 0;
        for (int j = 0; j < prop->curves[i].len; ++j) {
            Keyframe *key = &prop->curves[i].arr[j];
            if (time_prog < key->time) {
                values.elem[i] =
                    keyframe_lerp(key, &key_prev, time_prog - time_prev);
                continue;
            }
            key_prev = *key;
        }
    }
    switch (prop->type) {
    case PROP_TSL:
        obj->position = values;
        break;
    case PROP_ROT:
        obj->quaternion = quat_from_euler(values);
        break;
    case PROP_SCL:
        obj->scale = values;
        break;
    }
}

float find_cubic_bezier(float p1, float p2, float x0) {
    float p1_3 = p1 * 3.0, p2_3 = p2 * 3.0;
    float a = p1_3 - p2_3 + 1.0;
    float b = p2_3 - p1_3 - p1_3;
    float c = p1_3;

    float a_3 = 3.0 * a, b_2 = 2.0 * b;
    float t = x0;
    float x1, t2, t3;

    // Manually unroll three iterations of Newton-Rhapson, this is enough
    // for most tangents
    t2 = t * t;
    t3 = t2 * t;
    x1 = a * t3 + b * t2 + c * t - x0;
    t -= x1 / (a_3 * t2 + b_2 * t + c);

    t2 = t * t;
    t3 = t2 * t;
    x1 = a * t3 + b * t2 + c * t - x0;
    t -= x1 / (a_3 * t2 + b_2 * t + c);

    t2 = t * t;
    t3 = t2 * t;
    x1 = a * t3 + b * t2 + c * t - x0;
    t -= x1 / (a_3 * t2 + b_2 * t + c);

    // 4 ULP from 1.0
    if (fabsf(x1) <= EPSILON) return t;

    // Perform more iterations until we reach desired accuracy
    for (size_t i = 0; i < 4; i++) {
        t2 = t * t;
        t3 = t2 * t;
        x1 = a * t3 + b * t2 + c * t - x0;
        t -= x1 / (a_3 * t2 + b_2 * t + c);

        t2 = t * t;
        t3 = t2 * t;
        x1 = a * t3 + b * t2 + c * t - x0;
        t -= x1 / (a_3 * t2 + b_2 * t + c);

        if (fabsf(x1) <= EPSILON) return t;
    }

    return t;
}

float keyframe_lerp(Keyframe *next, Keyframe *prev, float t) {
    float res = 0.0f;
    switch (next->interpolation) {
    case INTERPOLATION_CONSTANT_PREV:
        return prev->value;
    case INTERPOLATION_CONSTANT_NEXT:
        return next->value;
    case INTERPOLATION_LINEAR:
        return prev->value * (1.0 - t) + next->value * t;
    case INTERPOLATION_CUBIC:
        {
            float rcp_delta = 1.0 / (next->time - prev->time);
            float x1 = prev->right.dx * rcp_delta;
            float x2 = 1.0 - next->left.dx * rcp_delta;
            t = find_cubic_bezier(x1, x2, t);

            float t2 = t * t;
            float t3 = t2 * t;
            float u = 1.0 - t;
            float u2 = u * u;
            float u3 = u2 * u;

            float y0 = prev->value;
            float y3 = next->value;
            float y1 = y0 + next->right.dy;
            float y2 = y3 - next->left.dy;

            return u3 * y0 + ((3.0 * ((u2 * t * y1) + (u * t2 * y2))) +
            (t3* y3));

            // return float_add(
            //     float_mul(u3, y0),
            //     float_add(float_mul(3.0, float_add(float_mul(float_mul(u2, t), y1),
            //                                     float_mul(float_mul(u, t2), y2))),
            //              float_mul(t3, y3)));
        }
    }
    return res;
}
