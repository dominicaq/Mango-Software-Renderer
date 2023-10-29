#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>
#include <stdbool.h>

#include "mat4.h"
#include "vec3.h"
#include "vec4.h"

/**
 * Calculate efficient barycentric coordinates of a point in a triangle.
 *
 * This function calculates efficient barycentric coordinates of a point `p`
 * within the triangle formed by vertices `a`, `b`, and `c`.
 *
 * Source:
 * https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
 *
 * @param p The point to calculate barycentric coordinates for.
 * @param a The first vertex of the triangle.
 * @param b The second vertex of the triangle.
 * @param c The third vertex of the triangle.
 * @return The barycentric coordinates of point `p`.
 */
Vec3 barycentric_coords(Vec3 p, Vec3 a, Vec3 b, Vec3 c);

/**
 * Linearly interpolate data based on barycentric coordinates.
 *
 * This function linearly interpolates data using barycentric coordinates.
 *
 * @param bc_coords The barycentric coordinates.
 * @param data An array of data values associated with the vertices.
 * @return The interpolated data value.
 */
Vec3 lerp_bc_coords(Vec3 bc_coords, Vec3 data[3]);

/**
 * Convert NDC to screen coordinates.
 *
 * @param screen_width The width of the screen.
 * @param screen_height The height of the screen.
 * @param ndc_coords The coordinates in Normalized Device Coordinates (NDC).
 * @return The coordinates in screen space.
 */
Vec3 ndc_to_screen(int screen_width, int screen_height, Vec3 ndc_coords);

/**
 * Convert world coordinates to screen coordinates.
 *
 * @param width The width of the screen.
 * @param height The height of the screen.
 * @param v The world coordinates to be converted.
 * @return The coordinates in screen space.
 */
Vec3 world_to_screen(int width, int height, Vec3 v);

/**
 * Check if a triangle is a backface in NDC.
 *
 * This function determines if a triangle with vertices in NDC (Normalized
 * Device Coordinates) is a backface or not.
 *
 * Source:
 * https://gamedev.stackexchange.com/questions/203694/how-to-make-backface-culling-work-correctly-in-both-orthographic-and-perspective
 *
 * @param ndc An array of three vertices in Normalized Device Coordinates (NDC).
 * @return 'true' if the triangle is a backface; 'false' otherwise.
 */
bool is_backface(Vec3 ndc[3]);

/**
 * Check if a point is within the normalized device coordinates (NDC) range of [-1, 1].
 *
 * @param ndc_point The 3D point represented in NDC.
 * @return true if the point is within the NDC range of [-1, 1] in all
 * dimensions, false otherwise.
 */
bool is_point_in_frustum(const Vec3* ndc_point);

/**
 * Clamp a value within a specified range.
 *
 * This function clamps a value to be within the specified minimum and maximum range.
 *
 * @param value The value to clamp.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value.
 */
float clamp(float value, float min, float max);

#endif
