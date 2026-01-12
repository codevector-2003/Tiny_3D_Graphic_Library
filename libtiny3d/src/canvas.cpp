#include "canvas.h"
#include <cmath>
#include <algorithm>

// --------------------
// Canvas Constructor
// --------------------
Canvas::Canvas(int w, int h)
{
    width = w;
    height = h;

    pixels = new float *[height];

    for (int y = 0; y < height; y++)
    {
        pixels[y] = new float[width];
        for (int x = 0; x < width; x++)
        {
            pixels[y][x] = 0.0f;
        }
    }
}

// --------------------
// Canvas Destructor
// --------------------
Canvas::~Canvas()
{
    for (int y = 0; y < height; y++)
    {
        delete[] pixels[y];
    }
    delete[] pixels;
}

// ------------------------------------------------
// Floating-point pixel with bilinear interpolation
// ------------------------------------------------
void set_pixel_f(Canvas &c, float x, float y, float intensity)
{
    int x0 = (int)x;
    int y0 = (int)y;

    float dx = x - x0;
    float dy = y - y0;

    float w00 = (1.0f - dx) * (1.0f - dy);
    float w10 = dx * (1.0f - dy);
    float w01 = (1.0f - dx) * dy;
    float w11 = dx * dy;

    if (x0 >= 0 && x0 < c.width && y0 >= 0 && y0 < c.height)
        c.pixels[y0][x0] += intensity * w00;

    if (x0 + 1 >= 0 && x0 + 1 < c.width && y0 >= 0 && y0 < c.height)
        c.pixels[y0][x0 + 1] += intensity * w10;

    if (x0 >= 0 && x0 < c.width && y0 + 1 >= 0 && y0 + 1 < c.height)
        c.pixels[y0 + 1][x0] += intensity * w01;

    if (x0 + 1 >= 0 && x0 + 1 < c.width && y0 + 1 >= 0 && y0 + 1 < c.height)
        c.pixels[y0 + 1][x0 + 1] += intensity * w11;
}

void draw_line_f(Canvas &c, float x0, float y0, float x1, float y1, float intensity, float thickness)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    float steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0)
    {
        set_pixel_f(c, x0, y0, intensity);
        return;
    }

    float x_inc = dx / steps;
    float y_inc = dy / steps;

    float len = std::sqrt(dx * dx + dy * dy);
    float dir_x = dx / len;
    float dir_y = dy / len;

    float perp_x = -dir_y;
    float perp_y = dir_x;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++)
    {
        for (float t = -thickness / 2; t <= thickness / 2; t += 1.0f)
        {
            float px = x + t * perp_x;
            float py = y + t * perp_y;
            set_pixel_f(c, px, py, intensity);
        }
        set_pixel_f(c, x, y, intensity);
        x += x_inc;
        y += y_inc;
    }
}