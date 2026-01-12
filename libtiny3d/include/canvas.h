#ifndef CANVAS_H
#define CANVAS_H

struct Canvas
{
    int width;
    int height;
    float **pixels;

    // Constructor
    Canvas(int w, int h);

    // Destructor
    ~Canvas();
};

// Draw a floating-point pixel using bilinear filtering
void set_pixel_f(Canvas &c, float x, float y, float intensity);
void draw_line_f(Canvas &c, float x0, float y0, float x1, float y1, float intensity, float thickness);

#endif
