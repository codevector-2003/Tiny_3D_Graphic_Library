#ifndef MATH3D_H
#define MATH3D_H

struct vec3_t
{
    float x;
    float y;
    float z;

    float r;
    float theta;
    float phi;

    vec3_t();
    vec3_t(float x, float y, float z);

    void update_spherical();
    void update_cartesian();
};

vec3_t vec3_from_spherical(float r, float theta, float phi);

vec3_t vec3_normalize_fast(const vec3_t &v);

vec3_t vec3_slerp(const vec3_t &a, const vec3_t &b, float t);

#endif