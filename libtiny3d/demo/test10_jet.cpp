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
    std::cout << "  TEST 10: F-22 Raptor Fighter Jet\n";
    std::cout << "========================================\n\n";
    std::cout << "Wireframe fighter jet rotating on Z axis\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 10 - F-22 Raptor");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* F-22 RAPTOR WIREFRAME - Detailed design */
    vec3_t jet_vertices[] = {
        // Nose cone - detailed (0-4)
        {0.0f, 0.0f, 2.4f},   // 0: nose tip
        {0.0f, 0.15f, 2.0f},  // 1: nose top-mid
        {0.0f, -0.15f, 2.0f}, // 2: nose bottom-mid
        {0.2f, 0.0f, 2.0f},   // 3: nose right-mid
        {-0.2f, 0.0f, 2.0f},  // 4: nose left-mid

        // Front fuselage (5-8)
        {0.3f, 0.2f, 1.6f},   // 5: top-right
        {-0.3f, 0.2f, 1.6f},  // 6: top-left
        {0.3f, -0.2f, 1.6f},  // 7: bottom-right
        {-0.3f, -0.2f, 1.6f}, // 8: bottom-left

        // Cockpit canopy - detailed (9-14)
        {0.0f, 0.45f, 1.2f},  // 9: canopy peak front
        {0.0f, 0.5f, 0.6f},   // 10: canopy peak mid
        {0.0f, 0.4f, 0.0f},   // 11: canopy peak rear
        {0.25f, 0.3f, 0.6f},  // 12: canopy right
        {-0.25f, 0.3f, 0.6f}, // 13: canopy left
        {0.0f, 0.25f, -0.2f}, // 14: cockpit rear center

        // Mid fuselage (15-18)
        {0.4f, 0.25f, 0.4f},  // 15: top-right
        {-0.4f, 0.25f, 0.4f}, // 16: top-left
        {0.4f, -0.2f, 0.4f},  // 17: bottom-right
        {-0.4f, -0.2f, 0.4f}, // 18: bottom-left

        // Rear fuselage (19-22)
        {0.3f, 0.2f, -1.2f},   // 19: top-right
        {-0.3f, 0.2f, -1.2f},  // 20: top-left
        {0.3f, -0.1f, -1.2f},  // 21: bottom-right
        {-0.3f, -0.1f, -1.2f}, // 22: bottom-left

        // Main delta wings (23-28)
        {2.4f, 0.0f, 1.0f},   // 23: right wing tip
        {-2.4f, 0.0f, 1.0f},  // 24: left wing tip
        {1.8f, 0.0f, 0.2f},   // 25: right wing mid
        {-1.8f, 0.0f, 0.2f},  // 26: left wing mid
        {1.2f, 0.0f, -0.6f},  // 27: right wing back
        {-1.2f, 0.0f, -0.6f}, // 28: left wing back

        // Vertical stabilizers - detailed (29-38)
        {0.4f, 0.95f, -1.0f},  // 29: right fin top front
        {0.4f, 0.9f, -1.4f},   // 30: right fin top rear
        {0.35f, 0.2f, -1.0f},  // 31: right fin base front
        {0.35f, 0.2f, -1.4f},  // 32: right fin base rear
        {0.45f, 0.6f, -1.2f},  // 33: right fin mid
        {-0.4f, 0.95f, -1.0f}, // 34: left fin top front
        {-0.4f, 0.9f, -1.4f},  // 35: left fin top rear
        {-0.35f, 0.2f, -1.0f}, // 36: left fin base front
        {-0.35f, 0.2f, -1.4f}, // 37: left fin base rear
        {-0.45f, 0.6f, -1.2f}, // 38: left fin mid

        // Horizontal stabilizers (39-40)
        {0.8f, 0.0f, -1.6f},  // 39: right stabilizer
        {-0.8f, 0.0f, -1.6f}, // 40: left stabilizer

        // Engine nozzles (41-44)
        {0.25f, 0.0f, -1.8f},  // 41: right nozzle
        {-0.25f, 0.0f, -1.8f}, // 42: left nozzle
        {0.2f, 0.0f, -2.0f},   // 43: right exhaust
        {-0.2f, 0.0f, -2.0f}   // 44: left exhaust
    };

    int jet_edges[][2] = {
        // Nose cone - detailed
        {0, 1},
        {0, 2},
        {0, 3},
        {0, 4}, // tip to mid points
        {1, 3},
        {3, 2},
        {2, 4},
        {4, 1}, // nose mid ring
        {1, 5},
        {3, 5},
        {2, 7},
        {4, 6}, // nose to front fuselage

        // Front fuselage cross-section
        {5, 6},
        {6, 8},
        {8, 7},
        {7, 5}, // front square

        // Cockpit canopy - detailed structure
        {5, 9},
        {6, 9}, // front to canopy front peak
        {9, 10},
        {10, 11}, // canopy spine
        {5, 12},
        {6, 13}, // fuselage to canopy sides
        {12, 10},
        {13, 10}, // canopy sides to peak
        {12, 11},
        {13, 11}, // canopy sides to rear
        {11, 14}, // canopy rear to cockpit back

        // Front to mid fuselage connection
        {5, 15},
        {6, 16},
        {7, 17},
        {8, 18},

        // Mid fuselage cross-section
        {15, 16},
        {16, 18},
        {18, 17},
        {17, 15},
        {14, 15},
        {14, 16}, // cockpit to mid fuselage

        // Mid to rear fuselage
        {15, 19},
        {16, 20},
        {17, 21},
        {18, 22},

        // Rear fuselage cross-section
        {19, 20},
        {20, 22},
        {22, 21},
        {21, 19},

        // Main delta wings - detailed
        {15, 23},
        {23, 25},
        {25, 27}, // right wing leading edge
        {16, 24},
        {24, 26},
        {26, 28}, // left wing leading edge
        {27, 19},
        {28, 20}, // wing back to rear fuselage
        {17, 27},
        {18, 28}, // wing trailing edge
        {23, 7},
        {24, 8}, // wing tip to front bottom

        // Vertical stabilizers - detailed
        {31, 29},
        {29, 30},
        {30, 32},
        {32, 31}, // right fin outline
        {31, 33},
        {33, 29},
        {33, 30},
        {33, 32}, // right fin cross-braces
        {19, 31},
        {19, 32}, // right fin to fuselage
        {36, 34},
        {34, 35},
        {35, 37},
        {37, 36}, // left fin outline
        {36, 38},
        {38, 34},
        {38, 35},
        {38, 37}, // left fin cross-braces
        {20, 36},
        {20, 37}, // left fin to fuselage
        {29, 34},
        {30, 35}, // fins top connection

        // Horizontal stabilizers
        {21, 39},
        {22, 40}, // stabilizers to fuselage
        {39, 40}, // stabilizers connection
        {32, 39},
        {37, 40}, // stabilizers to fin base

        // Engine nozzles
        {21, 41},
        {22, 42}, // fuselage to nozzles
        {41, 43},
        {42, 44}, // nozzles to exhaust
        {39, 41},
        {40, 42} // stabilizers to nozzles
    };

    int num_vertices = sizeof(jet_vertices) / sizeof(jet_vertices[0]);
    int num_edges = sizeof(jet_edges) / sizeof(jet_edges[0]);

    std::cout << "F-22 Raptor: " << num_vertices << " vertices, " << num_edges << " edges\n";
    std::cout << "Rotating on Y axis (showcasing features)\n\n";
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

        // Create model matrix - rotation around Y axis
        mat4 model = multiply(
            mat4::translation(0, 0, -5),
            mat4::rotation_xyz(0, angle, 0));

        renderer_wireframe(
            canvas,
            jet_vertices, num_vertices,
            jet_edges, num_edges,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        // Update rotation - slow barrel roll
        angle += 0.015f;
        if (angle > 6.28f)
            angle = 0.0f;

        Sleep(30);
    }

    std::cout << "\nTest 10 completed!\n";
    return 0;
}
