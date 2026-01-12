#include "animation.h"

vec3_t bezier(
    const vec3_t &p0,
    const vec3_t &p1,
    const vec3_t &p2,
    const vec3_t &p3,
    float t)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;

    vec3_t result;

    result.x =
        uu * u * p0.x +
        3 * uu * t * p1.x +
        3 * u * tt * p2.x +
        tt * t * p3.x;

    result.y =
        uu * u * p0.y +
        3 * uu * t * p1.y +
        3 * u * tt * p2.y +
        tt * t * p3.y;

    result.z =
        uu * u * p0.z +
        3 * uu * t * p1.z +
        3 * u * tt * p2.z +
        tt * t * p3.z;

    result.update_spherical();
    return result;
}

float loop_time(float t)
{
    if (t > 1.0f)
    {
        t -= static_cast<int>(t);
    }
    return t;
}