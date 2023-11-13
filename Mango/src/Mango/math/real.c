#include "real.h"

#include <float.h>

const MangoReal MANGO_FOUR_DIV_PI = (float)0x3fa2f983;
const MangoReal MANGO_FOUR_DIV_PI2 = (float)0xbecf817b;
const MangoReal MANGO_X4_CORRECTION_COMPONENT = (float)0x3e666666;
const MangoReal MANGO_PI_DIV_4 = (float)0x3f490fdb;
const MangoReal MANGO_THREE_PI_DIV_4 = (float)0x4016cbe4;
const MangoReal MANGO_REAL_MAX = FLT_MAX;
const MangoReal MANGO_REAL_MIN = FLT_MIN;
const MangoReal MANGO_PI = (float)0x40490fdb;
const MangoReal MANGO_E = (float)0x402df854;
const MangoReal MANGO_ONE = 1.0f;
const MangoReal MANGO_EPS = FLT_EPSILON;

/* Conversion functions between MangoReal and MangoReal/integer.
 * These are inlined to allow compiler to optimize away constant numbers
 */
MangoReal mango_real_from_int(int a) { return (MangoReal)a; }
MangoReal mango_real_from_float(float a) { return (MangoReal)a; }

MangoReal mango_real_abs(MangoReal x) { return (x < 0.0f ? -x : x); }
MangoReal mango_real_floor(MangoReal x) { return floorf(x); }
MangoReal mango_real_ceil(MangoReal x) { return ceilf(x); }
MangoReal mango_real_min(MangoReal x, MangoReal y) { return (x < y ? x : y); }
MangoReal mango_real_max(MangoReal x, MangoReal y) { return (x > y ? x : y); }
MangoReal mango_real_clamp(MangoReal x, MangoReal lo, MangoReal hi) {
    return mango_real_min(mango_real_max(x, lo), hi);
}

MangoReal mango_real_add(MangoReal inArg0, MangoReal inArg1) {
    return (inArg0 + inArg1);
}
MangoReal mango_real_sub(MangoReal inArg0, MangoReal inArg1) {
    return (inArg0 - inArg1);
}
MangoReal mango_real_mul(MangoReal inArg0, MangoReal inArg1) {
    return inArg0 * inArg1;
}

MangoReal mango_real_div(MangoReal inArg0, MangoReal inArg1) {
    return inArg0 / inArg1;
}
MangoReal mango_real_mod(MangoReal inArg0, MangoReal inArg1) {
    return fmodf(inArg0, inArg1);
}

MangoReal mango_real_sin(MangoReal inAngle) { return sinf(inAngle); }

MangoReal mango_real_cos(MangoReal inAngle) { return cosf(inAngle); }

MangoReal mango_real_tan(MangoReal inAngle) { return tanf(inAngle); }

MangoReal mango_real_asin(MangoReal inValue) { return asinf(inValue); }

MangoReal mango_real_acos(MangoReal inValue) { return acos(inValue); }

MangoReal mango_real_atan(MangoReal inValue) { return atanf(inValue); }

MangoReal mango_real_atan2(MangoReal inY, MangoReal inX) {
    return atan2f(inY, inX);
}

const MangoReal RAD_TO_DEG_MULT = (float)0x42652ee1;
MangoReal mango_real_rad_to_deg(MangoReal radians) {
    return mango_real_mul(radians, RAD_TO_DEG_MULT);
}

const MangoReal DEG_TO_RAD_MULT = (float)0x3c8efa35;
MangoReal mango_real_deg_to_rad(MangoReal degrees) {
    return mango_real_mul(degrees, DEG_TO_RAD_MULT);
}

MangoReal mango_real_sqrt(MangoReal inValue) { return sqrtf(inValue); }

MangoReal mango_real_sq(MangoReal x) { return mango_real_mul(x, x); }

MangoReal mango_real_exp(MangoReal inValue) { return expf(inValue); }

MangoReal mango_real_log(MangoReal inValue) { return logf(inValue); }

MangoReal mango_real_log2(MangoReal x) { return log2f(x); }

void mango_real_to_str(MangoReal value, char *buf, int decimals) {
    snprintf(buf, decimals, "%f", value);
}
