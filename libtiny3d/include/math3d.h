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
    void normalize_fast();
    static vec3_t slerp(const vec3_t &a, const vec3_t &b, float t);
};

struct vec4
{
    float x, y, z, w;
};

struct mat4
{
    float m[16]; // Column-major order

    static mat4 identity();
    static mat4 translation(float tx, float ty, float tz);
    static mat4 rotation_xyz(float rx, float ry, float rz);
    static mat4 scale(float sx, float sy, float sz);
    static mat4 frustumAssymetric(float left, float right, float bottom, float top, float nearVal, float farVal);
};

mat4 multiply(const mat4 &a, const mat4 &b);
vec3_t multiply(const mat4 &m, const vec3_t &v);

#endif