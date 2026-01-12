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
    std::cout << "  TEST 9: Cube at Different Angles\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing cube at various fixed angles...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 9 - Fixed Angles");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* Simple cube - larger size */
    vec3_t cube_vertices[8] = {
        {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}};

    int cube_edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

    std::cout << "Press keys to change angle:\n";
    std::cout << "  1 = 0 degrees\n";
    std::cout << "  2 = 45 degrees\n";
    std::cout << "  3 = 90 degrees\n";
    std::cout << "  4 = 135 degrees\n";
    std::cout << "  5 = 180 degrees\n";
    std::cout << "  A = Auto rotate (slow)\n";
    std::cout << "  ESC = Exit\n\n";

    float angle = 0.0f;
    bool auto_rotate = false;

    /* ================= RENDER LOOP ================= */
    while (display.is_open())
    {
        display.process_events();

        // Check for keyboard input
        if (GetAsyncKeyState('1') & 0x8000)
        {
            angle = 0.0f;
            auto_rotate = false;
            Sleep(200);
        }
        if (GetAsyncKeyState('2') & 0x8000)
        {
            angle = 0.785f;
            auto_rotate = false;
            Sleep(200);
        } // 45 deg
        if (GetAsyncKeyState('3') & 0x8000)
        {
            angle = 1.57f;
            auto_rotate = false;
            Sleep(200);
        } // 90 deg
        if (GetAsyncKeyState('4') & 0x8000)
        {
            angle = 2.356f;
            auto_rotate = false;
            Sleep(200);
        } // 135 deg
        if (GetAsyncKeyState('5') & 0x8000)
        {
            angle = 3.14159f;
            auto_rotate = false;
            Sleep(200);
        } // 180 deg
        if (GetAsyncKeyState('A') & 0x8000)
        {
            auto_rotate = true;
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        // Clear canvas
        for (int y = 0; y < canvas.height; y++)
            for (int x = 0; x < canvas.width; x++)
                canvas.pixels[y][x] = 0.0f;

        // Render cube at current angle
        mat4 rotation = mat4::rotation_xyz(0, angle, 0);
        mat4 translation = mat4::translation(0, 0, -5);
        mat4 model = multiply(translation, rotation);

        renderer_wireframe(
            canvas,
            cube_vertices, 8,
            cube_edges, 12,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        if (auto_rotate)
        {
            angle += 0.005f;
            if (angle > 6.28f)
                angle = 0.0f;
        }

        Sleep(30);
    }

    std::cout << "\nTest 9 completed!\n";
    return 0;
}
