#ifndef SHADER_MATH_H
#define SHADER_MATH_H

float clamp(float value, float min, float max);

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