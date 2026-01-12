#include "math3d.h"
#include <cmath>

static float fast_sqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int *)&x;             // get bits for floating VALUE
    i = 0x5f3759df - (i >> 1);      // gives initial guess y0
    x = *(float *)&i;               // convert bits BACK to float
    x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
    return 1.0f / x;
};

vec3_t::vec3_t() : x(0), y(0), z(0), r(0), theta(0), phi(0) {}

vec3_t::vec3_t(float x, float y, float z) : x(x), y(y), z(z)
{
    update_spherical();
}

void vec3_t::update_spherical()
{
    r = fast_sqrt(x * x + y * y + z * z);
    if (r > 0.0f)
    {
        theta = acosf(z / r);
        phi = atan2f(y, x);
    }
    else
    {
        theta = 0.0f;
        phi = 0.0f;
    }
}

void vec3_t::update_cartesian()
{
    float sinT = sinf(theta);
    x = r * sinT * cosf(phi);
    y = r * sinT * sinf(phi);
    z = r * cosf(theta);
}

void vec3_t::normalize_fast()
{
    float len2 = x * x + y * y + z * z;
    if (len2 > 0.0f)
    {
        float invLen = fast_sqrt(len2);
        x *= invLen;
        y *= invLen;
        z *= invLen;
        r = 1.0f;
    }
}

vec3_t vec3_t::slerp(const vec3_t &a, const vec3_t &b, float t)
{
    vec3_t v0 = a;
    vec3_t v1 = b;

    v0.normalize_fast();
    v1.normalize_fast();

    float dot = v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;

    // clamp dot product to avoid numerical errors
    if (dot > 1.0f)
        dot = 1.0f;
    if (dot < -1.0f)
        dot = -1.0f;

    float theta = acosf(dot) * t;

    if (theta < 1e-5f)
    {
        vec3_t v(
            v0.x + t * (v1.x - v0.x),
            v0.y + t * (v1.y - v0.y),
            v0.z + t * (v1.z - v0.z));
        v.normalize_fast();
        return v;
    }

    float sinT = sin(theta);
    float w1 = sin((1.0f - t) * theta) / sinT;
    float w2 = sin(t * theta) / sinT;

    vec3_t v(
        w1 * v0.x + w2 * v1.x,
        w1 * v0.y + w2 * v1.y,
        w1 * v0.z + w2 * v1.z);
    v.normalize_fast();
    return v;
}

mat4 mat4::identity()
{
    mat4 m = {};
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[15] = 1.0f;
    return m;
}

mat4 mat4::translation(float tx, float ty, float tz)
{
    mat4 m = mat4::identity();
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}

mat4 mat4::scale(float sx, float sy, float sz)
{
    mat4 m = {};
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    m.m[15] = 1.0f;
    return m;
}

mat4 mat4::rotation_xyz(float rx, float ry, float rz)
{
    float cx = cosf(rx);
    float sx = sinf(rx);
    float cy = cosf(ry);
    float sy = sinf(ry);
    float cz = cosf(rz);
    float sz = sinf(rz);

    mat4 m = {};
    m.m[0] = cy * cz;
    m.m[1] = sx * sy * cz - cx * sz;
    m.m[2] = cx * sy * cz + sx * sz;

    m.m[4] = cy * sz;
    m.m[5] = sx * sy * sz + cx * cz;
    m.m[6] = cx * sy * sz - sx * cz;

    m.m[8] = -sy;
    m.m[9] = sx * cy;
    m.m[10] = cx * cy;

    m.m[15] = 1.0f; // CRITICAL: w component must be 1

    return m;
}

mat4 mat4::frustumAssymetric(float left, float right, float bottom, float top, float nearVal, float farVal)
{
    mat4 m = {};
    m.m[0] = (2.0f * nearVal) / (right - left);
    m.m[5] = (2.0f * nearVal) / (top - bottom);
    m.m[8] = (right + left) / (right - left);
    m.m[9] = (top + bottom) / (top - bottom);
    m.m[10] = -(farVal + nearVal) / (farVal - nearVal);
    m.m[11] = -1.0f;
    m.m[14] = -(2.0f * farVal * nearVal) / (farVal - nearVal);
    return m;
}

mat4 multiply(const mat4 &a, const mat4 &b)
{
    mat4 r{};

    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            r.m[col * 4 + row] =
                a.m[0 * 4 + row] * b.m[col * 4 + 0] +
                a.m[1 * 4 + row] * b.m[col * 4 + 1] +
                a.m[2 * 4 + row] * b.m[col * 4 + 2] +
                a.m[3 * 4 + row] * b.m[col * 4 + 3];
        }
    }

    return r;
}

vec4 multiply(const mat4 &m, const vec4 &v)
{
    vec4 r;
    r.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12] * v.w;
    r.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13] * v.w;
    r.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w;
    r.w = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w;
    return r;
}

vec3_t multiply(const mat4 &m, const vec3_t &v)
{
    // Treat vec3 as a POINT  w = 1
    float x = v.x;
    float y = v.y;
    float z = v.z;
    float w = 1.0f;

    vec3_t r;

    r.x = m.m[0] * x + m.m[4] * y + m.m[8] * z + m.m[12] * w;
    r.y = m.m[1] * x + m.m[5] * y + m.m[9] * z + m.m[13] * w;
    r.z = m.m[2] * x + m.m[6] * y + m.m[10] * z + m.m[14] * w;

    // Compute resulting w (needed for projection)
    float rw = m.m[3] * x + m.m[7] * y + m.m[11] * z + m.m[15] * w;

    // Perspective divide if w changed (projection matrix)
    if (rw != 1.0f && rw > 0.0001f)
    {
        r.x /= rw;
        r.y /= rw;
        r.z /= rw;
    }

    // Keep spherical values consistent
    r.update_spherical();

    return r;
}
