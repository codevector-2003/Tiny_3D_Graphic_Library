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
    std::cout << "  TEST 6: Simple Polyhedra\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing tetrahedron and octahedron...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 6 - Polyhedra");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* TETRAHEDRON (4 vertices, 6 edges) - LARGER SIZE */
    vec3_t tetra_vertices[4] = {
        {0.0f, 0.6f, 0.0f},   // top
        {-0.5f, -0.3f, 0.5f}, // base front-left
        {0.5f, -0.3f, 0.5f},  // base front-right
        {0.0f, -0.3f, -0.5f}  // base back
    };

    int tetra_edges[6][2] = {
        {0, 1}, {0, 2}, {0, 3}, // edges from top
        {1, 2},
        {2, 3},
        {3, 1} // base edges
    };

    /* OCTAHEDRON (6 vertices, 12 edges) - LARGER SIZE */
    vec3_t octa_vertices[6] = {
        {0.0f, 0.5f, 0.0f},  // top
        {0.0f, -0.5f, 0.0f}, // bottom
        {0.5f, 0.0f, 0.0f},  // right
        {-0.5f, 0.0f, 0.0f}, // left
        {0.0f, 0.0f, 0.5f},  // front
        {0.0f, 0.0f, -0.5f}  // back
    };

    int octa_edges[12][2] = {
        // Top pyramid
        {0, 2},
        {0, 4},
        {0, 3},
        {0, 5},
        // Bottom pyramid
        {1, 2},
        {1, 4},
        {1, 3},
        {1, 5},
        // Middle square
        {2, 4},
        {4, 3},
        {3, 5},
        {5, 2}};

    std::cout << "Shapes:\n";
    std::cout << "  Left: Tetrahedron (4 vertices, 6 edges)\n";
    std::cout << "  Right: Octahedron (6 vertices, 12 edges)\n\n";
    std::cout << "Both rotating on Y axis\n\n";
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

        // Render tetrahedron on left
        mat4 model_left = multiply(
            mat4::translation(-0.6f, 0, -3),
            mat4::rotation_xyz(0, angle, 0));

        renderer_wireframe(
            canvas,
            tetra_vertices, 4,
            tetra_edges, 6,
            model_left, view, projection,
            SCREEN_W, SCREEN_H);

        // Render octahedron on right
        mat4 model_right = multiply(
            mat4::translation(0.6f, 0, -3),
            mat4::rotation_xyz(0, angle, 0));

        renderer_wireframe(
            canvas,
            octa_vertices, 6,
            octa_edges, 12,
            model_right, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        // Update rotation
        angle += 0.01f;
        if (angle > 6.28f)
            angle = 0.0f;

        Sleep(30);
    }

    std::cout << "\nTest 6 completed!\n";
    return 0;
}
