#include <cmath>
#include <vector>
#include <utility>
#include "math3d.h"
#include "renderer.h"
#include "canvas.h"

/* =========================================================
   CONFIG
   ========================================================= */

const int SCREEN_W = 800;
const int SCREEN_H = 800;

/* =========================================================
   SOCCER BALL (TRUNCATED ICOSAHEDRON)
   ========================================================= */

// Golden ratio
static const float PHI = 1.61803398875f;

/* -------- Icosahedron vertices -------- */
static vec3_t ico_vertices[12] = {
    {-1, PHI, 0}, {1, PHI, 0}, {-1, -PHI, 0}, {1, -PHI, 0}, {0, -1, PHI}, {0, 1, PHI}, {0, -1, -PHI}, {0, 1, -PHI}, {PHI, 0, -1}, {PHI, 0, 1}, {-PHI, 0, -1}, {-PHI, 0, 1}};

/* -------- Icosahedron edges (30 total) -------- */
static const int ico_edges[30][2] = {
    {0, 1}, {0, 5}, {0, 7}, {0, 10}, {0, 11}, {1, 5}, {1, 7}, {1, 8}, {1, 9}, {2, 3}, {2, 4}, {2, 6}, {2, 10}, {2, 11}, {3, 4}, {3, 6}, {3, 8}, {3, 9}, {4, 5}, {4, 9}, {4, 11}, {5, 9}, {5, 11}, {6, 7}, {6, 8}, {7, 8}, {7, 10}, {8, 9}, {10, 11}};

/* =========================================================
   GENERATE SOCCER BALL GEOMETRY
   ========================================================= */

void generate_soccer_ball(
    std::vector<vec3_t> &out_vertices,
    std::vector<std::pair<int, int>> &out_edges)
{
    out_vertices.clear();
    out_edges.clear();

    /* Normalize base icosahedron */
    for (int i = 0; i < 12; i++)
        ico_vertices[i].normalize_fast();

    /* ---- Truncate edges: generate 60 vertices ---- */
    for (int i = 0; i < 30; i++)
    {
        int a = ico_edges[i][0];
        int b = ico_edges[i][1];

        vec3_t v1(
            (2 * ico_vertices[a].x + ico_vertices[b].x) / 3.0f,
            (2 * ico_vertices[a].y + ico_vertices[b].y) / 3.0f,
            (2 * ico_vertices[a].z + ico_vertices[b].z) / 3.0f);

        vec3_t v2(
            (ico_vertices[a].x + 2 * ico_vertices[b].x) / 3.0f,
            (ico_vertices[a].y + 2 * ico_vertices[b].y) / 3.0f,
            (ico_vertices[a].z + 2 * ico_vertices[b].z) / 3.0f);

        v1.normalize_fast();
        v2.normalize_fast();

        out_vertices.push_back(v1);
        out_vertices.push_back(v2);
    }

    /* ---- Build edges automatically (distance test) ---- */
    const float EDGE_DIST2 = 0.30f; // tuned for truncated icosahedron

    int n = (int)out_vertices.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            float dx = out_vertices[i].x - out_vertices[j].x;
            float dy = out_vertices[i].y - out_vertices[j].y;
            float dz = out_vertices[i].z - out_vertices[j].z;

            float d2 = dx * dx + dy * dy + dz * dz;
            if (d2 < EDGE_DIST2)
                out_edges.push_back({i, j});
        }
    }
}

/* =========================================================
   MAIN
   ========================================================= */

int main()
{
    /* -------- Generate soccer ball -------- */
    std::vector<vec3_t> ball_vertices;
    std::vector<std::pair<int, int>> ball_edges;

    generate_soccer_ball(ball_vertices, ball_edges);

    /* Convert edges to renderer format */
    std::vector<int[2]> edge_list(ball_edges.size());
    for (size_t i = 0; i < ball_edges.size(); i++)
    {
        edge_list[i][0] = ball_edges[i].first;
        edge_list[i][1] = ball_edges[i].second;
    }

    /* -------- Camera & Projection -------- */
    mat4 view = mat4::identity();

    mat4 projection = mat4::frustumAssymetric(
        -1.0f, 1.0f,
        -1.0f, 1.0f,
        1.0f, 50.0f);

    /* -------- Create canvas -------- */
    Canvas canvas(SCREEN_W, SCREEN_H);

    /* -------- Animation loop (pseudo) -------- */
    float angle = 0.0f;
    int frame_count = 0;
    const int MAX_FRAMES = 100; // Limit frames for testing

    while (frame_count < MAX_FRAMES)
    {
        angle += 0.01f;
        frame_count++;

        mat4 rotation = mat4::rotation_xyz(angle, angle * 0.7f, 0.0f);
        mat4 translate = mat4::translation(0, 0, -6.0f);
        mat4 model = multiply(translate, rotation);

        renderer_wireframe(
            canvas,
            ball_vertices.data(),
            (int)ball_vertices.size(),
            edge_list.data(),
            (int)edge_list.size(),
            model,
            view,
            projection,
            SCREEN_W,
            SCREEN_H);

        /* swap_buffers(), clear_screen(), delay(), etc */
    }

    return 0;
}
