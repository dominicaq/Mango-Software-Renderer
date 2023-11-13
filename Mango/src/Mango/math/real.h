#ifndef MANGO_REAL
#define MANGO_REAL
#include <math.h>
#include <stdio.h>
typedef float MangoReal;

extern const MangoReal MANGO_FOUR_DIV_PI;
extern const MangoReal MANGO_FOUR_DIV_PI2;
extern const MangoReal MANGO_X4_CORRECTION_COMPONENT;
extern const MangoReal MANGO_PI_DIV_4;
extern const MangoReal MANGO_THREE_PI_DIV_4;
extern const MangoReal MANGO_REAL_MAX;
extern const MangoReal MANGO_REAL_MIN;
extern const MangoReal MANGO_PI;
extern const MangoReal MANGO_E;
extern const MangoReal MANGO_ONE;
extern const MangoReal MANGO_EPS;
MangoReal mango_real_from_int(int a);
MangoReal mango_real_from_float(float a);
MangoReal mango_real_abs(MangoReal x);
MangoReal mango_real_floor(MangoReal x);
MangoReal mango_real_ceil(MangoReal x);
MangoReal mango_real_min(MangoReal x, MangoReal y);
MangoReal mango_real_max(MangoReal x, MangoReal y);
MangoReal mango_real_clamp(MangoReal x, MangoReal lo, MangoReal hi);
MangoReal mango_real_add(MangoReal inArg0, MangoReal inArg1);
MangoReal mango_real_sub(MangoReal inArg0, MangoReal inArg1);
MangoReal mango_real_mul(MangoReal inArg0, MangoReal inArg1);
MangoReal mango_real_div(MangoReal inArg0, MangoReal inArg1);
MangoReal mango_real_mod(MangoReal inArg0, MangoReal inArg1);
MangoReal mango_real_sin(MangoReal inAngle);
MangoReal mango_real_cos(MangoReal inAngle);
MangoReal mango_real_tan(MangoReal inAngle);
MangoReal mango_real_asin(MangoReal inValue);
MangoReal mango_real_acos(MangoReal inValue);
MangoReal mango_real_atan(MangoReal inValue);
MangoReal mango_real_atan2(MangoReal inY, MangoReal inX);
MangoReal mango_real_rad_to_deg(MangoReal radians);
MangoReal mango_real_deg_to_rad(MangoReal degrees);
MangoReal mango_real_sqrt(MangoReal inValue);
MangoReal mango_real_sq(MangoReal x);
MangoReal mango_real_exp(MangoReal inValue);
MangoReal mango_real_log(MangoReal inValue);
MangoReal mango_real_log2(MangoReal x);
void mango_real_to_str(MangoReal value, char *buf, int decimals);
#endif
