#ifndef LIGHTING_H
#define LIGHTING_H

#include "math3d.h"

struct Light
{
    vec3_t direction;
    float intensity;
};

float lambert_edge(
    const vec3_t &v1,
    const vec3_t &v2,
    const Light &light);

float lambert_edge_multi(
    const vec3_t &v1,
    const vec3_t &v2,
    const Light *lights,
    int light_count);

#endif
