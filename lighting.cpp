#include "lighting.h"
#include <cmath>

float Lambert_edge(
    const vec3_t &v1,
    const vec3_t &v2,
    const Light &light)
{
    vec3_t e(
        v2.x - v1.x,
        v2.y - v1.y,
        v2.z - v1.z);

    e.normalize_fast();

    float dot =
        e.x * light.direction.x +
        e.y * light.direction.y +
        e.z * light.direction.z;

    float intensity = std::fabs(dot) * light.intensity;

    if (intensity < 0.0f)
        intensity = 0.0f;
    if (intensity > 1.0f)
        intensity = 1.0f;

    return intensity;
}

float lambert_edge_multi(
    const vec3_t &v1,
    const vec3_t &v2,
    const Light *lights,
    int light_count)
{
    float total = 0.0f;

    for (int i = 0; i < light_count; i++)
    {
        total += lambert_edge(v1, v2, lights[i]);
    }

    // Clamp final intensity
    if (total > 1.0f)
        total = 1.0f;

    return total;
}