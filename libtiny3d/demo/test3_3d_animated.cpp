#include <iostream>
#include <windows.h>
#include <chrono>

#include "math3d.h"
#include "renderer.h"
#include "canvas.h"
#include "window_display.h"

/* ================= CONFIG ================= */
const int SCREEN_W = 600;
const int SCREEN_H = 600;

int main()
{
    std::cout << "========================================\n";
    std::cout << "  TEST 3: Animated 3D Cube\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing 3D rotation and animation...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 3 - Rotating Cube");

    /* Camera & Projection - balanced view */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1.0f, 1.0f, -1.0f, 1.0f, 1, 50);

    /* Define a small cube - 0.5 units */
    vec3_t cube_vertices[8] = {
        {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}};

    int cube_edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}, // Back face
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7} // Connecting edges
    };

    std::cout << "Drawing a ROTATING cube\n";
    std::cout << "  Canvas: " << SCREEN_W << "x" << SCREEN_H << "\n";
    std::cout << "  Rotation speed: VERY SLOW\n\n";
    std::cout << "Press Ctrl+C or close window to exit\n\n";

    float angle = 0.0f;

    /* ================= RENDER LOOP ================= */
    while (display.is_open())
    {
        // Process events FIRST
        display.process_events();

        // Clear canvas using memset for reliability
        for (int y = 0; y < canvas.height; y++)
        {
            memset(canvas.pixels[y], 0, canvas.width * sizeof(float));
        }

        // Create rotation matrix and combine with translation - Y axis only
        mat4 model = multiply(
            mat4::translation(0, 0, -20),
            mat4::rotation_xyz(0, angle, 0));

        // Render rotating cube
        renderer_wireframe(
            canvas,
            cube_vertices, 8,
            cube_edges, 12,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        // Show the frame
        display.show(canvas);

        // Process events again after rendering
        display.process_events();

        // Update angle - VERY SLOW
        angle += 0.005f;
        if (angle > 6.28f)
            angle = 0.0f;

        Sleep(30);
    }

    std::cout << "\nTest 3 completed!\n";
    return 0;
}
