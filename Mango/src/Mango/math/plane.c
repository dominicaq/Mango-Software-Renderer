#include "plane.h"

bool is_point_on_plane(const Plane *plane, const Vec3 *point) {
    // Calculate the vector from the plane's position to the point
    Vec3 to_point = vec3_sub(*point, plane->position);

    // Calculate the dot product of the vector and the plane's normal
    float dot_product = vec3_dot(to_point, plane->normal);

    return fabs(dot_product) < EPSILON;
}
