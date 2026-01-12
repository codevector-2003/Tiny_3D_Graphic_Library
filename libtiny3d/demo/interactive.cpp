#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

#define NOMINMAX
#include <windows.h>
#undef min
#undef max

#include "math3d.h"
#include "renderer.h"
#include "canvas.h"
#include "display.h"
#include "animation.h"
#include "lighting.h"

/* ================= CONFIG ================= */
const int SCREEN_W = 800;
const int SCREEN_H = 800;
const int CONSOLE_WIDTH = 120; // Console display width
const int CONSOLE_HEIGHT = 40; // Console display height

/* ================= CUBE GEOMETRY ================= */
static vec3_t cube_vertices[8] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};

static const int cube_edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

/* ================= PYRAMID GEOMETRY ================= */
static vec3_t pyramid_vertices[5] = {
    {0, 1.5f, 0},    // Apex
    {-1, -0.5f, -1}, // Base corners
    {1, -0.5f, -1},
    {1, -0.5f, 1},
    {-1, -0.5f, 1}};

static const int pyramid_edges[8][2] = {
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, // Apex to base
    {1, 2},
    {2, 3},
    {3, 4},
    {4, 1} // Base edges
};

/* ================= LIGHT SETUP ================= */
Light lights[2];

void setup_lights()
{
    lights[0].direction = vec3_t(1, -1, -1);
    lights[0].direction.normalize_fast();
    lights[0].intensity = 0.8f;

    lights[1].direction = vec3_t(-1, 0.5f, -0.5f);
    lights[1].direction.normalize_fast();
    lights[1].intensity = 0.5f;
}

/* ================= CLEAR CANVAS ================= */
void clear_canvas(Canvas &canvas)
{
    for (int y = 0; y < canvas.height; y++)
    {
        for (int x = 0; x < canvas.width; x++)
        {
            canvas.pixels[y][x] = 0.0f;
        }
    }
}

/* ================= MAIN ================= */
int main()
{
    std::cout << "========================================\n";
    std::cout << "  Tiny3D Graphics Library - Demo\n";
    std::cout << "========================================\n\n";
    std::cout << "Rendering animated 3D objects...\n";
    std::cout << "Press Ctrl+C to exit\n\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));

    setup_lights();

    /* Hide cursor */
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* Bezier paths for animation */
    vec3_t pathA0(-3, 0, -8);
    vec3_t pathA1(-3, 3, -8);
    vec3_t pathA2(3, 3, -8);
    vec3_t pathA3(3, 0, -8);

    vec3_t pathB0(3, 0, -10);
    vec3_t pathB1(3, -3, -10);
    vec3_t pathB2(-3, -3, -10);
    vec3_t pathB3(-3, 0, -10);

    /* Create canvas and display */
    Canvas canvas(SCREEN_W, SCREEN_H);
    ASCIIDisplay display(CONSOLE_WIDTH, CONSOLE_HEIGHT);

    float t = 0.0f;
    int frame = 0;

    /* ================= RENDER LOOP ================= */
    while (true)
    {
        auto frame_start = std::chrono::high_resolution_clock::now();

        clear_canvas(canvas);

        /* Update time */
        t += 0.01f;
        t = loop_time(t);

        /* -------- Object 1: Rotating Cube (following path A) -------- */
        vec3_t posA = bezier(pathA0, pathA1, pathA2, pathA3, t);
        mat4 modelA = multiply(
            mat4::translation(posA.x, posA.y, posA.z),
            mat4::rotation_xyz(t * 3.14f, t * 6.28f, 0));

        renderer_wireframe(
            canvas,
            cube_vertices, 8,
            cube_edges, 12,
            modelA, view, projection,
            SCREEN_W, SCREEN_H);

        /* -------- Object 2: Spinning Pyramid (following path B) -------- */
        vec3_t posB = bezier(pathB0, pathB1, pathB2, pathB3, t);
        mat4 modelB = multiply(
            mat4::translation(posB.x, posB.y, posB.z),
            mat4::rotation_xyz(0, t * 6.28f, t * 3.14f));

        renderer_wireframe(
            canvas,
            pyramid_vertices, 5,
            pyramid_edges, 8,
            modelB, view, projection,
            SCREEN_W, SCREEN_H);

        /* -------- Object 3: Static spinning cube at center -------- */
        mat4 modelC = multiply(
            mat4::translation(0, 0, -6),
            mat4::rotation_xyz(t * 2.0f, t * 1.5f, t * 1.0f));

        renderer_wireframe(
            canvas,
            cube_vertices, 8,
            cube_edges, 12,
            modelC, view, projection,
            SCREEN_W, SCREEN_H);

        /* Display the frame */
        display.show(canvas);

        /* Frame counter */
        frame++;
        if (frame % 30 == 0)
        {
            COORD coord = {0, (SHORT)(CONSOLE_HEIGHT + 1)};
            SetConsoleCursorPosition(console, coord);
            std::cout << "Frame: " << frame << "  Time: " << t << "        ";
        }

        /* Frame timing (~60 FPS) */
        auto frame_end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);

        int sleep_time = 16 - (int)elapsed.count();
        if (sleep_time > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
    }

    return 0;
}
