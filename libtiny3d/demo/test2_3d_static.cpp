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
    std::cout << "  TEST 2: 3D Objects (No Animation)\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing 3D projection and wireframe rendering...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 2 - Static 3D Cube");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* Define a simple cube */
    vec3_t cube_vertices[8] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};

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

    std::cout << "Cube Definition:\n";
    std::cout << "  Vertices: 8\n";
    std::cout << "  Edges: 12\n";
    std::cout << "  Position: Center, z = -6\n\n";
    std::cout << "Drawing a STATIC cube (no rotation)\n\n";
    std::cout << "Press Ctrl+C or close window to exit\n\n";

    /* Model transformation - static position */
    mat4 model = mat4::translation(0, 0, -6);

    /* ================= RENDER LOOP ================= */
    while (display.is_open())
    {
        display.process_events();

        // Clear canvas
        for (int y = 0; y < canvas.height; y++)
            for (int x = 0; x < canvas.width; x++)
                canvas.pixels[y][x] = 0.0f;

        // Render static cube
        renderer_wireframe(
            canvas,
            cube_vertices, 8,
            cube_edges, 12,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        Sleep(30);
    }

    std::cout << "\nTest 2 completed!\n";
    return 0;
}
