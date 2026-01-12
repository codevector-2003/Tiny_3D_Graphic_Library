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

    // Sample and display
    int step_x = canvas.width / width;
    int step_y = canvas.height / height;

    if (step_x < 1)
        step_x = 1;
    if (step_y < 1)
        step_y = 1;

    for (int y = 0; y < height && y * step_y < canvas.height; y++)
    {
        for (int x = 0; x < width && x * step_x < canvas.width; x++)
        {
            float intensity = canvas.pixels[y * step_y][x * step_x];

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
