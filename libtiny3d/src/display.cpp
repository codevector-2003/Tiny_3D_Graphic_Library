#include "display.h"
#include <iostream>
#include <windows.h>

// Grayscale ramp for ASCII art
static const char *GRAYSCALE = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
static const int GRAYSCALE_LEN = 65;

ASCIIDisplay::ASCIIDisplay(int w, int h) : width(w), height(h)
{
    // Set console to UTF-8
    SetConsoleOutputCP(CP_UTF8);
}

ASCIIDisplay::~ASCIIDisplay()
{
}

void ASCIIDisplay::clear()
{
    // Clear console (Windows)
    system("cls");
}

void ASCIIDisplay::show(const Canvas &canvas)
{
    // Move cursor to top-left
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // Calculate proper sampling steps
    float step_x = (float)canvas.width / (float)width;
    float step_y = (float)canvas.height / (float)height;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Sample from canvas using proper interpolation
            int canvas_x = (int)(x * step_x);
            int canvas_y = (int)(y * step_y);

            // Clamp to canvas bounds
            if (canvas_x >= canvas.width)
                canvas_x = canvas.width - 1;
            if (canvas_y >= canvas.height)
                canvas_y = canvas.height - 1;

            float intensity = canvas.pixels[canvas_y][canvas_x];

            // Clamp and map to grayscale
            if (intensity < 0.0f)
                intensity = 0.0f;
            if (intensity > 1.0f)
                intensity = 1.0f;

            int index = (int)(intensity * (GRAYSCALE_LEN - 1));
            std::cout << GRAYSCALE[index];
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
}
