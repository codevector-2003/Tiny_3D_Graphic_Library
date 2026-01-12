#include <iostream>
#include <windows.h>
#include <cmath>

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
    std::cout << "  TEST 4: Static 3D Sphere\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing 3D sphere wireframe...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 4 - Static Sphere");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* Generate sphere vertices */
    const int lat_segments = 8;  // latitude lines
    const int lon_segments = 12; // longitude lines
    const float radius = 1.0f;

    vec3_t sphere_vertices[lat_segments * lon_segments];
    int vertex_count = 0;

    for (int lat = 0; lat < lat_segments; lat++)
    {
        float theta = (lat * 3.14159f) / (lat_segments - 1); // 0 to PI
        for (int lon = 0; lon < lon_segments; lon++)
        {
            float phi = (lon * 2.0f * 3.14159f) / lon_segments; // 0 to 2*PI

            float x = radius * sin(theta) * cos(phi);
            float y = radius * sin(theta) * sin(phi);
            float z = radius * cos(theta);

            sphere_vertices[vertex_count++] = {x, y, z};
        }
    }

    /* Generate sphere edges (latitude and longitude lines) */
    int sphere_edges[256][2];
    int edge_count = 0;

    // Latitude lines
    for (int lat = 0; lat < lat_segments; lat++)
    {
        for (int lon = 0; lon < lon_segments; lon++)
        {
            int current = lat * lon_segments + lon;
            int next = lat * lon_segments + ((lon + 1) % lon_segments);
            sphere_edges[edge_count][0] = current;
            sphere_edges[edge_count][1] = next;
            edge_count++;
        }
    }

    // Longitude lines
    for (int lon = 0; lon < lon_segments; lon++)
    {
        for (int lat = 0; lat < lat_segments - 1; lat++)
        {
            int current = lat * lon_segments + lon;
            int next = (lat + 1) * lon_segments + lon;
            sphere_edges[edge_count][0] = current;
            sphere_edges[edge_count][1] = next;
            edge_count++;
        }
    }

    std::cout << "Sphere Definition:\n";
    std::cout << "  Vertices: " << vertex_count << "\n";
    std::cout << "  Edges: " << edge_count << "\n";
    std::cout << "  Radius: " << radius << "\n";
    std::cout << "  Position: Center, z = -5\n\n";
    std::cout << "Drawing a STATIC sphere\n\n";
    std::cout << "Press Ctrl+C or close window to exit\n\n";

    /* Model transformation - static position */
    mat4 model = mat4::translation(0, 0, -5);

    /* ================= RENDER LOOP ================= */
    while (display.is_open())
    {
        display.process_events();

        // Clear canvas
        for (int y = 0; y < canvas.height; y++)
            for (int x = 0; x < canvas.width; x++)
                canvas.pixels[y][x] = 0.0f;

        // Render static sphere
        renderer_wireframe(
            canvas,
            sphere_vertices, vertex_count,
            sphere_edges, edge_count,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        Sleep(30);
    }

    std::cout << "\nTest 4 completed!\n";
    return 0;
}
