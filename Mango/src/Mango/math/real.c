#include "real.h"

#include <float.h>

const Real MANGO_FOUR_DIV_PI = (float)0x3fa2f983;
const Real MANGO_FOUR_DIV_PI2 = (float)0xbecf817b;
const Real MANGO_X4_CORRECTION_COMPONENT = (float)0x3e666666;
const Real MANGO_PI_DIV_4 = (float)0x3f490fdb;
const Real MANGO_THREE_PI_DIV_4 = (float)0x4016cbe4;
const Real MANGO_REAL_MAX = FLT_MAX;
const Real MANGO_REAL_MIN = FLT_MIN;
const Real MANGO_PI = (float)0x40490fdb;
const Real MANGO_E = (float)0x402df854;
const Real MANGO_ONE = 1.0f;
const Real MANGO_EPS = FLT_EPSILON;

/* Conversion functions between Real and Real/integer.
 * These are inlined to allow compiler to optimize away constant numbers
 */
Real real_from_int(int a) { return (Real)a; }
Real real_from_float(float a) { return (Real)a; }

Real real_abs(Real x) { return (x < 0.0f ? -x : x); }
Real real_floor(Real x) { return floorf(x); }
Real real_ceil(Real x) { return ceilf(x); }
Real real_min(Real x, Real y) { return (x < y ? x : y); }
Real real_max(Real x, Real y) { return (x > y ? x : y); }
Real real_clamp(Real x, Real lo, Real hi) {
    return real_min(real_max(x, lo), hi);
}

Real real_add(Real inArg0, Real inArg1) { return (inArg0 + inArg1); }
Real real_sub(Real inArg0, Real inArg1) { return (inArg0 - inArg1); }
Real real_mul(Real inArg0, Real inArg1) { return inArg0 * inArg1; }

Real real_div(Real inArg0, Real inArg1) { return inArg0 / inArg1; }
Real real_mod(Real inArg0, Real inArg1) { return fmodf(inArg0, inArg1); }

Real real_sin(Real inAngle) { return sinf(inAngle); }
Real real_cos(Real inAngle) { return cosf(inAngle); }
Real real_tan(Real inAngle) { return tanf(inAngle); }
Real real_asin(Real inValue) { return asinf(inValue); }
Real real_acos(Real inValue) { return acos(inValue); }
Real real_atan(Real inValue) { return atanf(inValue); }
Real real_atan2(Real inY, Real inX) { return atan2f(inY, inX); }

const Real RAD_TO_DEG_MULT = (float)0x42652ee1;
Real real_rad_to_deg(Real radians) {
    return real_mul(radians, RAD_TO_DEG_MULT);
}

const Real DEG_TO_RAD_MULT = (float)0x3c8efa35;
Real real_deg_to_rad(Real degrees) {
    return real_mul(degrees, DEG_TO_RAD_MULT);
}

Real real_sqrt(Real inValue) { return sqrtf(inValue); }

Real real_sq(Real x) { return real_mul(x, x); }

Real real_exp(Real inValue) { return expf(inValue); }

Real real_log(Real inValue) { return logf(inValue); }

Real real_log2(Real x) { return log2f(x); }

void real_to_str(Real value, char *buf, int decimals) {
    snprintf(buf, decimals, "%f", value);
}
