#ifndef ANIMATION_H
#define ANIMATION_H

#include "math3d.h"

vec3_t bezier(
    const vec3_t &p0,
    const vec3_t &p1,
    const vec3_t &p2,
    const vec3_t &p3,
    float t);

float loop_time(float t);

#endif
