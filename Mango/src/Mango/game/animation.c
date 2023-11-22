#include "animation.h"

void prop_update(AnimProp *prop, GameObject *obj, Real time_prog) {
    Vec3 values;
    for (int i = 0; i < 3; ++i) {
        Keyframe key_prev = {0, prop->default_value.elem[i], 0};

        Real time_prev = 0;
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

Real find_cubic_bezier(Real p1, Real p2, Real x0) {
    Real p1_3 = p1 * 3.0, p2_3 = p2 * 3.0;
    Real a = p1_3 - p2_3 + 1.0;
    Real b = p2_3 - p1_3 - p1_3;
    Real c = p1_3;

    Real a_3 = 3.0 * a, b_2 = 2.0 * b;
    Real t = x0;
    Real x1, t2, t3;

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
    if (real_abs(x1) <= REAL_EPS) return t;

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

        if (real_abs(x1) <= REAL_EPS) return t;
    }

    return t;
}

Real keyframe_lerp(Keyframe *next, Keyframe *prev, Real t) {
    Real res = 0.0f;
    switch (next->interpolation) {
    case INTERPOLATION_CONSTANT_PREV:
        return prev->value;
    case INTERPOLATION_CONSTANT_NEXT:
        return next->value;
    case INTERPOLATION_LINEAR:
        return prev->value * (1.0 - t) + next->value * t;
    case INTERPOLATION_CUBIC:
        {
            Real rcp_delta = real_div(1, real_sub(next->time, prev->time));
            Real x1 = real_mul(prev->right.dx, rcp_delta);
            Real x2 = real_sub(1.0, real_mul(next->left.dx, rcp_delta));
            t = find_cubic_bezier(x1, x2, t);

            Real t2 = real_mul(t, t);
            Real t3 = real_mul(t2, t);
            Real u = real_sub(1.0, t);
            Real u2 = real_mul(u, u);
            Real u3 = real_mul(u2, u);

            Real y0 = prev->value;
            Real y3 = next->value;
            Real y1 = real_add(y0, prev->right.dy);
            Real y2 = real_sub(y3, next->left.dy);

            return real_add(
                real_mul(u3, y0),
                real_add(real_mul(3.0, real_add(real_mul(real_mul(u2, t), y1),
                                                real_mul(real_mul(u, t2), y2))),
                         real_mul(t3, y3)));
        }
    }
    return res;
}
