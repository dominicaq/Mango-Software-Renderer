#ifndef REAL
#define REAL
#include <math.h>
#include <stdint.h>

typedef float Real;
extern const Real REAL_FOUR_DIV_PI;
extern const Real REAL_FOUR_DIV_PI2;
extern const Real REAL_X4_CORRECTION_COMPONENT;
extern const Real REAL_PI_DIV_4;
extern const Real REAL_THREE_PI_DIV_4;
extern const Real REAL_REAL_MAX;
extern const Real REAL_REAL_MIN;
extern const Real REAL_PI;
extern const Real REAL_E;
extern const Real REAL_ONE;
extern const Real REAL_EPS;
Real real_from_i32(int32_t a);
Real real_from_float(float a);
int32_t real_to_i32(Real a);
Real real_abs(Real x);
Real real_floor(Real x);
Real real_ceil(Real x);
Real real_min(Real x, Real y);
Real real_max(Real x, Real y);
Real real_clamp(Real x, Real lo, Real hi);
Real real_add(Real inArg0, Real inArg1);
Real real_sub(Real inArg0, Real inArg1);
Real real_mul(Real inArg0, Real inArg1);
Real real_div(Real inArg0, Real inArg1);
Real real_mod(Real inArg0, Real inArg1);
Real real_sin(Real inAngle);
Real real_cos(Real inAngle);
Real real_tan(Real inAngle);
Real real_asin(Real inValue);
Real real_acos(Real inValue);
Real real_atan(Real inValue);
Real real_atan2(Real inY, Real inX);
Real real_rad_to_deg(Real radians);
Real real_deg_to_rad(Real degrees);
Real real_sqrt(Real inValue);
Real real_sq(Real x);
Real real_exp(Real inValue);
Real real_log(Real inValue);
Real real_log2(Real x);
#endif
