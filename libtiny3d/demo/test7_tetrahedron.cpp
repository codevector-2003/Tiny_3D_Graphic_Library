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
    std::cout << "  TEST 7: Tetrahedron\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing 4-sided pyramid (tetrahedron)...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 7 - Tetrahedron");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* TETRAHEDRON (4 vertices, 6 edges) */
    vec3_t vertices[4] = {
        {0.0f, 0.3f, 0.0f},      // top
        {-0.25f, -0.15f, 0.25f}, // base front-left
        {0.25f, -0.15f, 0.25f},  // base front-right
        {0.0f, -0.15f, -0.25f}   // base back
    };

    int edges[6][2] = {
        {0, 1}, {0, 2}, {0, 3}, // edges from top to base
        {1, 2},
        {2, 3},
        {3, 1} // base triangle edges
    };

    std::cout << "Tetrahedron:\n";
    std::cout << "  4 vertices, 6 edges\n";
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

        // Render tetrahedron
        mat4 model = multiply(
            mat4::translation(0, 0, -3),
            mat4::rotation_xyz(0, angle, 0));

        renderer_wireframe(
            canvas,
            vertices, 4,
            edges, 6,
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

    std::cout << "\nTest 7 completed!\n";
    return 0;
}
