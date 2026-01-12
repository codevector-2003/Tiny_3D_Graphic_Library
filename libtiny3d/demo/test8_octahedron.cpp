#include <iostream>
#include <windows.h>

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
    std::cout << "  TEST 8: Octahedron\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing 8-sided diamond (octahedron)...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 8 - Octahedron");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* OCTAHEDRON (6 vertices, 12 edges) */
    vec3_t vertices[6] = {
        {0.0f, 0.25f, 0.0f},  // top
        {0.0f, -0.25f, 0.0f}, // bottom
        {0.25f, 0.0f, 0.0f},  // right
        {-0.25f, 0.0f, 0.0f}, // left
        {0.0f, 0.0f, 0.25f},  // front
        {0.0f, 0.0f, -0.25f}  // back
    };

    int edges[12][2] = {
        // Top pyramid edges
        {0, 2},
        {0, 4},
        {0, 3},
        {0, 5},
        // Bottom pyramid edges
        {1, 2},
        {1, 4},
        {1, 3},
        {1, 5},
        // Middle square edges
        {2, 4},
        {4, 3},
        {3, 5},
        {5, 2}};

    std::cout << "Octahedron:\n";
    std::cout << "  6 vertices, 12 edges\n";
    std::cout << "  Rotating on Y axis\n\n";
    std::cout << "Press Ctrl+C or close window to exit\n\n";

    float angle = 0.0f;

    /* ================= RENDER LOOP ================= */
    while (display.is_open())
    {
        display.process_events();

        // Clear canvas
        for (int y = 0; y < canvas.height; y++)
            for (int x = 0; x < canvas.width; x++)
                canvas.pixels[y][x] = 0.0f;

        // Render octahedron
        mat4 model = multiply(
            mat4::translation(0, 0, -3),
            mat4::rotation_xyz(0, angle, 0));

        renderer_wireframe(
            canvas,
            vertices, 6,
            edges, 12,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        // Update rotation
        angle += 0.01f;
        if (angle > 6.28f)
            angle = 0.0f;

        Sleep(30);
    }

    std::cout << "\nTest 8 completed!\n";
    return 0;
}
