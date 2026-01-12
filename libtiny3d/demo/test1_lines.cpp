#include <iostream>
#include <windows.h>
#include <cmath>

#include "canvas.h"
#include "window_display.h"

/* ================= CONFIG ================= */
const int SCREEN_W = 600;
const int SCREEN_H = 600;

int main()
{
    std::cout << "========================================\n";
    std::cout << "  TEST 1: Basic Line Drawing\n";
    std::cout << "========================================\n\n";
    std::cout << "Testing canvas and line drawing...\n\n";

    /* Create canvas and window */
    Canvas canvas(SCREEN_W, SCREEN_H);
    WindowDisplay display(SCREEN_W, SCREEN_H, "Test 1 - Line Drawing");

    std::cout << "Drawing animated circle (ball):\n";
    std::cout << "  - Moving ball bouncing around screen\n";
    std::cout << "  - Circle drawn with line segments\n\n";
    std::cout << "Press Ctrl+C or close window to exit\n\n";

    float ball_x = 300.0f;
    float ball_y = 300.0f;
    float ball_vx = 3.0f;
    float ball_vy = 2.0f;
    float ball_radius = 50.0f;

    /* ================= RENDER LOOP ================= */
    while (display.is_open())
    {
        display.process_events();

        // Clear canvas
        for (int y = 0; y < canvas.height; y++)
            for (int x = 0; x < canvas.width; x++)
                canvas.pixels[y][x] = 0.0f;

        // Update ball position
        ball_x += ball_vx;
        ball_y += ball_vy;

        // Bounce off walls
        if (ball_x - ball_radius < 0 || ball_x + ball_radius > SCREEN_W)
            ball_vx = -ball_vx;
        if (ball_y - ball_radius < 0 || ball_y + ball_radius > SCREEN_H)
            ball_vy = -ball_vy;

        // Draw circle using line segments
        int segments = 32;
        for (int i = 0; i < segments; i++)
        {
            float angle1 = (i * 2.0f * 3.14159f) / segments;
            float angle2 = ((i + 1) * 2.0f * 3.14159f) / segments;

            float x1 = ball_x + ball_radius * cos(angle1);
            float y1 = ball_y + ball_radius * sin(angle1);
            float x2 = ball_x + ball_radius * cos(angle2);
            float y2 = ball_y + ball_radius * sin(angle2);

            draw_line_f(canvas, x1, y1, x2, y2, 1.0f, 1.0f);
        }

        // Draw center cross
        draw_line_f(canvas, ball_x - 5, ball_y, ball_x + 5, ball_y, 1.0f, 1.0f);
        draw_line_f(canvas, ball_x, ball_y - 5, ball_x, ball_y + 5, 1.0f, 1.0f);

        display.show(canvas);
        display.process_events();

        Sleep(30);
    }

    std::cout << "\nTest 1 completed!\n";
    return 0;
}
