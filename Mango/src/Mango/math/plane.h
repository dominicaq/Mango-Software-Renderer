#ifndef PLANE_H
#define PLANE_H

#include <stdbool.h>
#include <math.h>

#include "vec3.h"

typedef struct {
    Vec3 position;
    Vec3 normal;
} Plane;

/**
 * Calculate the distance from a point to a plane.
 *
 * @param plane The Plane to calculate the distance to.
 * @param point The point from which to calculate the distance.
 * @return The distance from the point to the plane.
 */
float distance_to_plane(const Plane* plane, const Vec3* point);

/**
 * Check if a point lies on the plane.
 *
 * @param plane The Plane to check against.
 * @param point The point to check if it lies on the plane.
 * @return true if the point is on the plane, false otherwise.
 */
bool is_point_on_plane(const Plane* plane, const Vec3* point);

#endif
