#include "renderer.h"
#include "canvas.h"

// NOTE: multiply(mat4, vec3_t) must treat vec3 as (x,y,z,1)

void project_vertex(
    const vec3_t &in,
    ScreenVertex &out,
    const mat4 &model,
    const mat4 &view,
    const mat4 &projection,
    int screen_width,
    int screen_height)
{
    // Local → World
    vec3_t v = multiply(model, in);

    // World → Camera
    v = multiply(view, v);

    // Camera → Clip (projection + perspective divide)
    v = multiply(projection, v);

    // NDC → Screen
    out.x = static_cast<int>((v.x + 1.0f) * 0.5f * screen_width);
    out.y = static_cast<int>((1.0f - v.y) * 0.5f * screen_height);
    out.z = v.z;
}

bool clip_to_circular_viewport(
    int cx, int cy, int radius,
    int x, int y)
{
    int dx = x - cx;
    int dy = y - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

// depth sorting
struct Edge
{
    ScreenVertex a;
    ScreenVertex b;
    float depth;
};

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
    int screen_height)
{
    ScreenVertex projected[128];
    Edge edge_list[256];

    // Project all vertices
    for (int i = 0; i < vertex_count; ++i)
    {
        project_vertex(
            vertices[i],
            projected[i],
            model,
            view,
            projection,
            screen_width,
            screen_height);
    }

    // Build edge list with depth
    for (int i = 0; i < edge_count; ++i)
    {
        int a = edges[i][0];
        int b = edges[i][1];

        edge_list[i].a = projected[a];
        edge_list[i].b = projected[b];
        edge_list[i].depth =
            (projected[a].z + projected[b].z) * 0.5f;
    }

    // Sort edges back → front (Painter’s algorithm)
    for (int i = 0; i < edge_count - 1; ++i)
    {
        for (int j = i + 1; j < edge_count; ++j)
        {
            if (edge_list[i].depth < edge_list[j].depth)
            {
                Edge tmp = edge_list[i];
                edge_list[i] = edge_list[j];
                edge_list[j] = tmp;
            }
        }
    }

    // Draw edges with rectangular clipping (full screen)
    for (int i = 0; i < edge_count; ++i)
    {
        const Edge &e = edge_list[i];

        // Simple bounds check - only draw if both points are reasonably on screen
        bool a_visible = (e.a.x >= -100 && e.a.x < screen_width + 100 &&
                          e.a.y >= -100 && e.a.y < screen_height + 100);
        bool b_visible = (e.b.x >= -100 && e.b.x < screen_width + 100 &&
                          e.b.y >= -100 && e.b.y < screen_height + 100);

        if (a_visible || b_visible)
        {
            draw_line_f(
                canvas,
                e.a.x, e.a.y,
                e.b.x, e.b.y,
                1.0f, 1.0f);
        }
    }
}
