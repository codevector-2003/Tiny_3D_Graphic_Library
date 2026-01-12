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
    std::cout << "  TEST 11: Chess King Piece\n";
    std::cout << "========================================\n\n";
    std::cout << "Wireframe chess king rotating on Y axis\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 11 - Chess King");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* CHESS KING WIREFRAME */
    vec3_t king_vertices[] = {
        // Base platform (0-7) - octagon
        {0.6f, -1.0f, 0.0f},     // 0: front
        {0.42f, -1.0f, 0.42f},   // 1: front-right
        {0.0f, -1.0f, 0.6f},     // 2: right
        {-0.42f, -1.0f, 0.42f},  // 3: back-right
        {-0.6f, -1.0f, 0.0f},    // 4: back
        {-0.42f, -1.0f, -0.42f}, // 5: back-left
        {0.0f, -1.0f, -0.6f},    // 6: left
        {0.42f, -1.0f, -0.42f},  // 7: front-left

        // Lower ring (8-15) - octagon
        {0.5f, -0.7f, 0.0f},     // 8
        {0.35f, -0.7f, 0.35f},   // 9
        {0.0f, -0.7f, 0.5f},     // 10
        {-0.35f, -0.7f, 0.35f},  // 11
        {-0.5f, -0.7f, 0.0f},    // 12
        {-0.35f, -0.7f, -0.35f}, // 13
        {0.0f, -0.7f, -0.5f},    // 14
        {0.35f, -0.7f, -0.35f},  // 15

        // Mid section (16-23) - octagon
        {0.4f, -0.2f, 0.0f},     // 16
        {0.28f, -0.2f, 0.28f},   // 17
        {0.0f, -0.2f, 0.4f},     // 18
        {-0.28f, -0.2f, 0.28f},  // 19
        {-0.4f, -0.2f, 0.0f},    // 20
        {-0.28f, -0.2f, -0.28f}, // 21
        {0.0f, -0.2f, -0.4f},    // 22
        {0.28f, -0.2f, -0.28f},  // 23

        // Upper ring (24-31) - octagon
        {0.45f, 0.1f, 0.0f},    // 24
        {0.32f, 0.1f, 0.32f},   // 25
        {0.0f, 0.1f, 0.45f},    // 26
        {-0.32f, 0.1f, 0.32f},  // 27
        {-0.45f, 0.1f, 0.0f},   // 28
        {-0.32f, 0.1f, -0.32f}, // 29
        {0.0f, 0.1f, -0.45f},   // 30
        {0.32f, 0.1f, -0.32f},  // 31

        // Crown rim (32-39) - octagon
        {0.35f, 0.5f, 0.0f},    // 32
        {0.25f, 0.5f, 0.25f},   // 33
        {0.0f, 0.5f, 0.35f},    // 34
        {-0.25f, 0.5f, 0.25f},  // 35
        {-0.35f, 0.5f, 0.0f},   // 36
        {-0.25f, 0.5f, -0.25f}, // 37
        {0.0f, 0.5f, -0.35f},   // 38
        {0.25f, 0.5f, -0.25f},  // 39

        // Crown points (40-47) - alternating high/low
        {0.35f, 0.75f, 0.0f},   // 40: front (high)
        {0.25f, 0.6f, 0.25f},   // 41: front-right (low)
        {0.0f, 0.75f, 0.35f},   // 42: right (high)
        {-0.25f, 0.6f, 0.25f},  // 43: back-right (low)
        {-0.35f, 0.75f, 0.0f},  // 44: back (high)
        {-0.25f, 0.6f, -0.25f}, // 45: back-left (low)
        {0.0f, 0.75f, -0.35f},  // 46: left (high)
        {0.25f, 0.6f, -0.25f},  // 47: front-left (low)

        // Cross on top - vertical (48-49)
        {0.0f, 0.85f, 0.0f}, // 48: cross center
        {0.0f, 1.1f, 0.0f},  // 49: cross top

        // Cross horizontal (50-51)
        {-0.15f, 0.95f, 0.0f}, // 50: cross left
        {0.15f, 0.95f, 0.0f}   // 51: cross right
    };

    int king_edges[][2] = {
        // Base platform octagon
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 0},

        // Base to lower ring
        {0, 8},
        {1, 9},
        {2, 10},
        {3, 11},
        {4, 12},
        {5, 13},
        {6, 14},
        {7, 15},

        // Lower ring octagon
        {8, 9},
        {9, 10},
        {10, 11},
        {11, 12},
        {12, 13},
        {13, 14},
        {14, 15},
        {15, 8},

        // Lower ring to mid section
        {8, 16},
        {9, 17},
        {10, 18},
        {11, 19},
        {12, 20},
        {13, 21},
        {14, 22},
        {15, 23},

        // Mid section octagon
        {16, 17},
        {17, 18},
        {18, 19},
        {19, 20},
        {20, 21},
        {21, 22},
        {22, 23},
        {23, 16},

        // Mid to upper ring
        {16, 24},
        {17, 25},
        {18, 26},
        {19, 27},
        {20, 28},
        {21, 29},
        {22, 30},
        {23, 31},

        // Upper ring octagon
        {24, 25},
        {25, 26},
        {26, 27},
        {27, 28},
        {28, 29},
        {29, 30},
        {30, 31},
        {31, 24},

        // Upper ring to crown rim
        {24, 32},
        {25, 33},
        {26, 34},
        {27, 35},
        {28, 36},
        {29, 37},
        {30, 38},
        {31, 39},

        // Crown rim octagon
        {32, 33},
        {33, 34},
        {34, 35},
        {35, 36},
        {36, 37},
        {37, 38},
        {38, 39},
        {39, 32},

        // Crown rim to crown points
        {32, 40},
        {33, 41},
        {34, 42},
        {35, 43},
        {36, 44},
        {37, 45},
        {38, 46},
        {39, 47},

        // Crown points connections
        {40, 41},
        {41, 42},
        {42, 43},
        {43, 44},
        {44, 45},
        {45, 46},
        {46, 47},
        {47, 40},

        // Crown points to center cross
        {40, 48},
        {42, 48},
        {44, 48},
        {46, 48},

        // Cross vertical
        {48, 49},

        // Cross horizontal
        {50, 51},

        // Cross connections
        {48, 50},
        {48, 51}};

    int num_vertices = sizeof(king_vertices) / sizeof(king_vertices[0]);
    int num_edges = sizeof(king_edges) / sizeof(king_edges[0]);

    std::cout << "Chess King: " << num_vertices << " vertices, " << num_edges << " edges\n";
    std::cout << "Rotating on Y axis\n\n";
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
            mat4::translation(0, 0, -6),
            mat4::rotation_xyz(0, angle, 0));

        renderer_wireframe(
            canvas,
            king_vertices, num_vertices,
            king_edges, num_edges,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        // Update rotation - slow spin
        angle += 0.012f;
        if (angle > 6.28f)
            angle = 0.0f;

        Sleep(30);
    }

    std::cout << "\nTest 11 completed!\n";
    return 0;
}
