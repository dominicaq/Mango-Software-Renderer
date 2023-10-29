#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>
#include <stdbool.h>

#include "mat4.h"
#include "vec3.h"
#include "vec4.h"

/*
 * barycentric_coords - Calculate efficient barycentric coordinates
 * @p: The point to calculate barycentric coordinates for
 * @a: The first vertex of the triangle
 * @b: The second vertex of the triangle
 * @c: The third vertex of the triangle
 *
 * Calculate the efficient barycentric coordinates of a point @p within the
 * triangle formed by vertices @a, @b, and @c.
 *
 * Source:
 * https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
 *
 * Return: The barycentric coordinates of point @p.
 */
Vec3 barycentric_coords(Vec3 p, Vec3 a, Vec3 b, Vec3 c);

Vec3 lerp_bc_coords(Vec3 bc_coords, Vec3 normals[3]);

/*
 * ndc_to_screen - Convert NDC to screen coordinates
 * @screenWidth: The width of the screen
 * @screenHeight: The height of the screen
 * @ndc_coords: The coordinates in Normalized Device Coordinates (NDC)
 *
 * Convert NDC (Normalized Device Coordinates) to screen coordinates based on
 * the provided screen width and height.
 *
 * Return: The coordinates in screen space.
 */
Vec3 ndc_to_screen(int screenWidth, int screenHeight, Vec3 ndc_coords);

/*
 * world_to_screen - Convert world coordinates to screen coordinates
 * @width: The width of the screen
 * @height: The height of the screen
 * @v: The world coordinates to be converted
 *
 * Convert world coordinates to screen coordinates based on the provided
 * screen width and height.
 *
 * Return: The coordinates in screen space.
 */
Vec3 world_to_screen(int width, int height, Vec3 v);

/*
 * is_backface - Check if a triangle is a backface in NDC
 * @ndc: An array of three vertices in Normalized Device Coordinates (NDC)geome
 *
 * Determine whether a triangle with vertices represented by NDC (Normalized
 * Device Coordinates) is a backface or not.
 *
 * Source:
 * https://gamedev.stackexchange.com/questions/203694/how-to-make-backface-culling-work-correctly-in-both-orthographic-and-perspective
 *
 * Return: 'true' if the triangle is a backface; 'false' otherwise.
 */
bool is_backface(Vec3 ndc[3]);

float clamp(float value, float min, float max);

#endif
