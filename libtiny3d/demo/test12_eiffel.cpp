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
    std::cout << "  TEST 12: Eiffel Tower (Detailed)\n";
    std::cout << "========================================\n\n";
    std::cout << "Wireframe Eiffel Tower rotating on Y axis\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 12 - Eiffel Tower");

    /* Camera & Projection */
    mat4 view = mat4::identity();
    mat4 projection = mat4::frustumAssymetric(-1, 1, -1, 1, 1, 50);

    /* EIFFEL TOWER WIREFRAME - Heavy details - LARGER SIZE */
    vec3_t tower_vertices[] = {
        // Base level - ground (0-3) - four corners
        {2.4f, -4.0f, 2.4f},   // 0: front-right leg base
        {-2.4f, -4.0f, 2.4f},  // 1: front-left leg base
        {-2.4f, -4.0f, -2.4f}, // 2: back-left leg base
        {2.4f, -4.0f, -2.4f},  // 3: back-right leg base

        // Base arches - lower connection (4-7)
        {1.2f, -3.2f, 2.0f},   // 4: front-right arch
        {-1.2f, -3.2f, 2.0f},  // 5: front-left arch
        {-1.2f, -3.2f, -2.0f}, // 6: back-left arch
        {1.2f, -3.2f, -2.0f},  // 7: back-right arch

        // Base cross-bracing mid (8-11)
        {1.8f, -2.6f, 1.8f},   // 8: front-right
        {-1.8f, -2.6f, 1.8f},  // 9: front-left
        {-1.8f, -2.6f, -1.8f}, // 10: back-left
        {1.8f, -2.6f, -1.8f},  // 11: back-right

        // First platform level (12-15) - main corners
        {1.6f, -1.6f, 1.6f},   // 12: front-right
        {-1.6f, -1.6f, 1.6f},  // 13: front-left
        {-1.6f, -1.6f, -1.6f}, // 14: back-left
        {1.6f, -1.6f, -1.6f},  // 15: back-right

        // First platform details (16-19) - inner frame
        {1.2f, -1.6f, 1.2f},   // 16: inner front-right
        {-1.2f, -1.6f, 1.2f},  // 17: inner front-left
        {-1.2f, -1.6f, -1.2f}, // 18: inner back-left
        {1.2f, -1.6f, -1.2f},  // 19: inner back-right

        // Cross-bracing to second level (20-23)
        {1.3f, -1.0f, 1.3f},   // 20: front-right
        {-1.3f, -1.0f, 1.3f},  // 21: front-left
        {-1.3f, -1.0f, -1.3f}, // 22: back-left
        {1.3f, -1.0f, -1.3f},  // 23: back-right

        // Second platform level (24-27)
        {1.1f, -0.4f, 1.1f},   // 24: front-right
        {-1.1f, -0.4f, 1.1f},  // 25: front-left
        {-1.1f, -0.4f, -1.1f}, // 26: back-left
        {1.1f, -0.4f, -1.1f},  // 27: back-right

        // Second platform details (28-31) - inner frame
        {0.8f, -0.4f, 0.8f},   // 28: inner front-right
        {-0.8f, -0.4f, 0.8f},  // 29: inner front-left
        {-0.8f, -0.4f, -0.8f}, // 30: inner back-left
        {0.8f, -0.4f, -0.8f},  // 31: inner back-right

        // Cross-bracing to third level (32-35)
        {0.9f, 0.2f, 0.9f},   // 32: front-right
        {-0.9f, 0.2f, 0.9f},  // 33: front-left
        {-0.9f, 0.2f, -0.9f}, // 34: back-left
        {0.9f, 0.2f, -0.9f},  // 35: back-right

        // Third platform level (36-39)
        {0.7f, 0.8f, 0.7f},   // 36: front-right
        {-0.7f, 0.8f, 0.7f},  // 37: front-left
        {-0.7f, 0.8f, -0.7f}, // 38: back-left
        {0.7f, 0.8f, -0.7f},  // 39: back-right

        // Upper tower section (40-43)
        {0.5f, 1.6f, 0.5f},   // 40: front-right
        {-0.5f, 1.6f, 0.5f},  // 41: front-left
        {-0.5f, 1.6f, -0.5f}, // 42: back-left
        {0.5f, 1.6f, -0.5f},  // 43: back-right

        // Upper narrowing (44-47)
        {0.36f, 2.4f, 0.36f},   // 44: front-right
        {-0.36f, 2.4f, 0.36f},  // 45: front-left
        {-0.36f, 2.4f, -0.36f}, // 46: back-left
        {0.36f, 2.4f, -0.36f},  // 47: back-right

        // Top observation deck (48-51)
        {0.24f, 3.2f, 0.24f},   // 48: front-right
        {-0.24f, 3.2f, 0.24f},  // 49: front-left
        {-0.24f, 3.2f, -0.24f}, // 50: back-left
        {0.24f, 3.2f, -0.24f},  // 51: back-right

        // Spire base (52-55)
        {0.16f, 3.8f, 0.16f},   // 52: front-right
        {-0.16f, 3.8f, 0.16f},  // 53: front-left
        {-0.16f, 3.8f, -0.16f}, // 54: back-left
        {0.16f, 3.8f, -0.16f},  // 55: back-right

        // Spire sections (56-59)
        {0.1f, 4.4f, 0.1f},   // 56: lower spire
        {-0.1f, 4.4f, 0.1f},  // 57
        {-0.1f, 4.4f, -0.1f}, // 58
        {0.1f, 4.4f, -0.1f},  // 59

        {0.06f, 5.0f, 0.06f},   // 60: mid spire
        {-0.06f, 5.0f, 0.06f},  // 61
        {-0.06f, 5.0f, -0.06f}, // 62
        {0.06f, 5.0f, -0.06f},  // 63

        // Spire tip (64)
        {0.0f, 5.6f, 0.0f}, // 64: antenna tip

        // Additional cross-bracing vertices (65-80)
        // Level 1 mid-bracing
        {0.0f, -2.6f, 2.0f},  // 65: front center
        {0.0f, -2.6f, -2.0f}, // 66: back center
        {2.0f, -2.6f, 0.0f},  // 67: right center
        {-2.0f, -2.6f, 0.0f}, // 68: left center

        // Level 2 mid-bracing
        {0.0f, -1.0f, 1.3f},  // 69: front center
        {0.0f, -1.0f, -1.3f}, // 70: back center
        {1.3f, -1.0f, 0.0f},  // 71: right center
        {-1.3f, -1.0f, 0.0f}, // 72: left center

        // Level 3 mid-bracing
        {0.0f, 0.2f, 0.9f},  // 73: front center
        {0.0f, 0.2f, -0.9f}, // 74: back center
        {0.9f, 0.2f, 0.0f},  // 75: right center
        {-0.9f, 0.2f, 0.0f}, // 76: left center

        // Upper section centers
        {0.0f, 1.6f, 0.0f}, // 77: upper platform center
        {0.0f, 2.4f, 0.0f}, // 78: mid-upper center
        {0.0f, 3.2f, 0.0f}, // 79: top deck center
        {0.0f, 3.8f, 0.0f}  // 80: spire base center
    };

    int tower_edges[][2] = {
        // Four main legs from ground to first platform
        {0, 8},
        {1, 9},
        {2, 10},
        {3, 11},
        {8, 12},
        {9, 13},
        {10, 14},
        {11, 15},

        // Ground level base connections
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},

        // Base arches
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
        {4, 8},
        {5, 9},
        {6, 10},
        {7, 11},

        // Cross-bracing at base level
        {8, 9},
        {9, 10},
        {10, 11},
        {11, 8},
        {8, 65},
        {9, 65},
        {10, 66},
        {11, 66},
        {8, 67},
        {11, 67},
        {9, 68},
        {10, 68},
        {65, 66},
        {67, 68},

        // First platform structure
        {12, 13},
        {13, 14},
        {14, 15},
        {15, 12},
        {16, 17},
        {17, 18},
        {18, 19},
        {19, 16},
        {12, 16},
        {13, 17},
        {14, 18},
        {15, 19},

        // Diagonal bracing on first platform
        {12, 17},
        {13, 16},
        {14, 19},
        {15, 18},

        // First to second platform legs
        {12, 20},
        {13, 21},
        {14, 22},
        {15, 23},
        {20, 24},
        {21, 25},
        {22, 26},
        {23, 27},

        // Cross-bracing to second level
        {20, 21},
        {21, 22},
        {22, 23},
        {23, 20},
        {20, 69},
        {21, 69},
        {22, 70},
        {23, 70},
        {20, 71},
        {23, 71},
        {21, 72},
        {22, 72},
        {69, 70},
        {71, 72},

        // Second platform structure
        {24, 25},
        {25, 26},
        {26, 27},
        {27, 24},
        {28, 29},
        {29, 30},
        {30, 31},
        {31, 28},
        {24, 28},
        {25, 29},
        {26, 30},
        {27, 31},

        // Diagonal bracing on second platform
        {24, 29},
        {25, 28},
        {26, 31},
        {27, 30},

        // Second to third platform legs
        {24, 32},
        {25, 33},
        {26, 34},
        {27, 35},
        {32, 36},
        {33, 37},
        {34, 38},
        {35, 39},

        // Cross-bracing to third level
        {32, 33},
        {33, 34},
        {34, 35},
        {35, 32},
        {32, 73},
        {33, 73},
        {34, 74},
        {35, 74},
        {32, 75},
        {35, 75},
        {33, 76},
        {34, 76},
        {73, 74},
        {75, 76},

        // Third platform structure
        {36, 37},
        {37, 38},
        {38, 39},
        {39, 36},
        {36, 77},
        {37, 77},
        {38, 77},
        {39, 77},

        // Third platform to upper section
        {36, 40},
        {37, 41},
        {38, 42},
        {39, 43},
        {40, 41},
        {41, 42},
        {42, 43},
        {43, 40},
        {40, 78},
        {41, 78},
        {42, 78},
        {43, 78},

        // Upper section to top deck
        {40, 44},
        {41, 45},
        {42, 46},
        {43, 47},
        {44, 45},
        {45, 46},
        {46, 47},
        {47, 44},

        // Top deck structure
        {44, 48},
        {45, 49},
        {46, 50},
        {47, 51},
        {48, 49},
        {49, 50},
        {50, 51},
        {51, 48},
        {48, 79},
        {49, 79},
        {50, 79},
        {51, 79},

        // Spire base
        {48, 52},
        {49, 53},
        {50, 54},
        {51, 55},
        {52, 53},
        {53, 54},
        {54, 55},
        {55, 52},
        {52, 80},
        {53, 80},
        {54, 80},
        {55, 80},

        // Spire sections
        {52, 56},
        {53, 57},
        {54, 58},
        {55, 59},
        {56, 57},
        {57, 58},
        {58, 59},
        {59, 56},

        {56, 60},
        {57, 61},
        {58, 62},
        {59, 63},
        {60, 61},
        {61, 62},
        {62, 63},
        {63, 60},

        // Spire to tip
        {60, 64},
        {61, 64},
        {62, 64},
        {63, 64},

        // Additional diagonal bracing throughout
        {16, 20},
        {17, 21},
        {18, 22},
        {19, 23},
        {28, 32},
        {29, 33},
        {30, 34},
        {31, 35},
        {12, 14},
        {13, 15},
        {24, 26},
        {25, 27},
        {36, 38},
        {37, 39},
        {40, 42},
        {41, 43},
        {44, 46},
        {45, 47},
        {48, 50},
        {49, 51},
        {52, 54},
        {53, 55},
        {56, 58},
        {57, 59}};

    int num_vertices = sizeof(tower_vertices) / sizeof(tower_vertices[0]);
    int num_edges = sizeof(tower_edges) / sizeof(tower_edges[0]);

    std::cout << "Eiffel Tower: " << num_vertices << " vertices, " << num_edges << " edges\n";
    std::cout << "Rotating on Y axis\n";
    std::cout << "Height: 9.6 units, Base: 4.8x4.8 units (2x larger)\n\n";
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

        // Create model matrix - rotation around Y axis (moved farther back)
        mat4 model = multiply(
            mat4::translation(0, 0, -15),
            mat4::rotation_xyz(0, angle, 0));

        renderer_wireframe(
            canvas,
            tower_vertices, num_vertices,
            tower_edges, num_edges,
            model, view, projection,
            SCREEN_W, SCREEN_H);

        display.show(canvas);
        display.process_events();

        // Update rotation - slow spin
        angle += 0.008f;
        if (angle > 6.28f)
            angle = 0.0f;

        Sleep(30);
    }

    std::cout << "\nTest 12 completed!\n";
    return 0;
}
