#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"

// Forward declaration
struct Canvas;

struct ScreenVertex
{
    int x;
    int y;
    float z; // depth
};

// Applies: Local -> World -> View -> Clip -> NDC -> Screen
void project_vertex(
    const vec3_t &in,
    ScreenVertex &out,
    const mat4 &model,
    const mat4 &view,
    const mat4 &projection,
    int screen_width,
    int screen_height);

// Circular viewport clipping
bool clip_to_circular_viewport(
    int cx, int cy, int radius,
    int x, int y);

// draw wireframes edges
void renderer_wireframe(
    Canvas &canvas,
    const vec3_t *vertices,
    int vertex_count,
    const int (*edges)[2],
    int edge_count,
    const mat4 &model,
    const mat4 &view,
    const mat4 &projection,
    int screen_width,
    int screen_height);

#endif