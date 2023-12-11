#ifndef SHADER_MATH_H
#define SHADER_MATH_H

float clamp(float value, float min, float max);

#define BUILD_LG_SP(i, x, y) \
    SET_LG_CONT(i, ((128 + x) << 2) | ((64 + y) << 12) | (3 << 22))
#define BUILD_MD_SP(i, x, y) \
    SET_MD_CONT(i, ((128 + x) << 2) | ((32 + y) << 12) | (3 << 22))
#define BUILD_SM_SP(i, x, y) \
    SET_SM_CONT(i, ((128 + x) << 2) | ((16 + y) << 12) | (3 << 22))

/**
 * Linear interpolation between two values.
 *
 * @param a Starting value.
 * @param b Ending value.
 * @param t Interpolation factor [0, 1].
 * @return Result of linear interpolation between `a` and `b`.
 */
float lerp(float a, float b, float t);

#endif
