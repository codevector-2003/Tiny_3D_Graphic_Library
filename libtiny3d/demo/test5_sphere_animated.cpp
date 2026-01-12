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
    std::cout << "  TEST 5: Animated 3D Sphere\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing 3D sphere rotation...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 5 - Rotating Sphere");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* Generate sphere vertices - larger size */
    const int lat_segments = 8;
    const int lon_segments = 12;
    const float radius = 3.0f;

    vec3_t sphere_vertices[lat_segments * lon_segments];
    int vertex_count = 0;

    for (int lat = 0; lat < lat_segments; lat++)
    {
        float theta = (lat * 3.14159f) / (lat_segments - 1);
        for (int lon = 0; lon < lon_segments; lon++)
        {
            float phi = (lon * 2.0f * 3.14159f) / lon_segments;

            float x = radius * sin(theta) * cos(phi);
            float y = radius * sin(theta) * sin(phi);
            float z = radius * cos(theta);

            sphere_vertices[vertex_count++] = {x, y, z};
        }
    }

    /* Generate sphere edges */
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

    std::cout << "Sphere: " << vertex_count << " vertices, " << edge_count << " edges\n";
    std::cout << "Rotating slowly on Y axis...\n\n";
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

        // Create rotation matrix and translation
        mat4 model = multiply(
            mat4::translation(0, 0, -10),
            mat4::rotation_xyz(0, angle, 0));

        // Render rotating sphere
        renderer_wireframe(
            canvas,
            sphere_vertices, vertex_count,
            sphere_edges, edge_count,
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

    std::cout << "\nTest 5 completed!\n";
    return 0;
}
