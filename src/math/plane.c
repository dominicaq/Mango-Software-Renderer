#include "plane.h"

float distance_to_plane(const Plane* plane, const Vec3* point) {
    // Calculate the vector from the plane's position to the point
    Vec3 toPoint;
    subtractVec3(point, &plane->position, &toPoint);

    // Calculate the dot product of the vector and the plane's normal
    float dotProduct = dotVec3(&toPoint, &plane->normal);

    // Calculate the magnitude of the plane's normal
    float normalMagnitude = lengthVec3(&plane->normal);

    // Calculate the distance
    return fabs(dotProduct) / normalMagnitude;
}

bool is_point_on_plane(const Plane* plane, const Vec3* point) {
    // Calculate the vector from the plane's position to the point
    Vec3 to_point = vec3_sub(*point, plane->position);

    // Calculate the dot product of the vector and the plane's normal
    float dot_product = vec3_dot(to_point, plane->normal);

    return fabs(dot_product) < EPSILON;
}
