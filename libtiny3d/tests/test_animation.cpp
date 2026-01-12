#include <vector>
#include <utility>
#include <cmath>

#include "math3d.h"
#include "renderer.h"
#include "canvas.h"
#include "lighting.h"
#include "animation.h"

/* ================= CONFIG ================= */

const int SCREEN_W = 800;
const int SCREEN_H = 800;

/* ================= SIMPLE CUBE ================= */

static vec3_t cube_vertices[8] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};

static const int cube_edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

/* ================= LIGHT SETUP ================= */

Light lights[2];

void setup_lights()
{
    lights[0].direction = vec3_t(1, -1, -1);
    lights[0].direction.normalize_fast();
    lights[0].intensity = 0.7f;

    lights[1].direction = vec3_t(-1, 0, -0.5f);
    lights[1].direction.normalize_fast();
    lights[1].intensity = 0.4f;
}

/* ================= MAIN ================= */

int main()
{
    setup_lights();

    /* Camera & projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(
        -1, 1, -1, 1, 1, 50);

    /* Bezier paths (shared t → sync) */
    vec3_t pathA0(-3, 0, -6);
    vec3_t pathA1(-3, 3, -6);
    vec3_t pathA2(3, 3, -6);
    vec3_t pathA3(3, 0, -6);

    vec3_t pathB0(3, 0, -8);
    vec3_t pathB1(3, -3, -8);
    vec3_t pathB2(-3, -3, -8);
    vec3_t pathB3(-3, 0, -8);

    float t = 0.0f;

    /* Create canvas */
    Canvas canvas(SCREEN_W, SCREEN_H);

    /* ================= RENDER LOOP ================= */
    int frame_count = 0;
    const int MAX_FRAMES = 100; // Limit frames for testing

    while (frame_count < MAX_FRAMES)
    {
        frame_count++;

        /* Shared time step */
        t += 0.005f;
        t = loop_time(t);

        /* -------- Object A (cube) -------- */
        vec3_t posA = bezier(pathA0, pathA1, pathA2, pathA3, t);
        mat4 modelA =
            multiply(
                mat4::translation(posA.x, posA.y, posA.z),
                mat4::rotation_xyz(0, t * 6.28f, 0));

        renderer_wireframe(
            canvas,
            cube_vertices, 8,
            cube_edges, 12,
            modelA, view, projection,
            SCREEN_W, SCREEN_H);

        /* -------- Object B (cube, phase-shifted path) -------- */
        vec3_t posB = bezier(pathB0, pathB1, pathB2, pathB3, t);
        mat4 modelB =
            multiply(
                mat4::translation(posB.x, posB.y, posB.z),
                mat4::rotation_xyz(t * 6.28f, 0, 0));

        renderer_wireframe(
            canvas,
            cube_vertices, 8,
            cube_edges, 12,
            modelB, view, projection,
            SCREEN_W, SCREEN_H);
    }

    return 0;
}
